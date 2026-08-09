#include "world.h"

inline static uint8_t cheapFastHash(uint8_t const x, uint8_t const y, uint8_t const z, uint8_t const seed){
    uint8_t h = seed;
    h ^= x * 0x9Fu;
    h ^= y * 0xA3u;
    h ^= z * 0xC1u;

    h ^= h >> 3;
    h *= 0x9Fu;
    h ^= h >> 5;
    h *= 0xA3u;
    h ^= h >> 4;

    return h;  
}

blockType defaultMapGeneration(int const x, int const y, int const z, unsigned int const seed){
    blockType type = GRASS;
    uint8_t height = cheapFastHash(x, y, 0, seed) % 3 + 118;

    if (z > height){
        type = AIR; 
    }
    else if (z < height){
        type = DIRT;
    }
    return type;
}

blockType coolerMapGeneration(int const x, int const y, int const z, unsigned int const seed){
    blockType type = GRASS;
    uint8_t height = x* y/50 + 100;


    if (z > height){
        type = AIR; 
    }
    else if (z < height){
        type = DIRT;
    }
    return type;
}

static ChunkNode* setNode(Chunk* chunk){
    ChunkNode* node = malloc(sizeof(ChunkNode));
    node->xKey = chunk->x;
    node->yKey = chunk->y;
    node->chunk = chunk;
    node->next = NULL;
    return node;
}


World* initWorld(blockType(*mapGenFunct)(int, int, int, unsigned int)){
    World* world = malloc(sizeof(World));
    world->numOfChunks = 0;
    world->mapGeneration = mapGenFunct;
    world->seed = 0x23243;
    for (int i = 0; i < WORLD_CAPACITY; i++){
        world->chunkArray[i] = NULL;
    }
    return world;
}

unsigned int hashChunkCoord(World* world, int x, int y) {
    unsigned int h = (unsigned int)x * 0x9E3779B1u;
    h ^= (unsigned int)y * 0x85EBCA6Bu;

    h ^= h >> 16;
    h *= 0x85EBCA6Bu;
    h ^= h >> 13;
    h *= 0xC2B2AE35u;
    h ^= h >> 16;

    return h % WORLD_CAPACITY;
}


void genChunk(World* world, int x, int y){
    ChunkNode* newNode = setNode(createChunk(x, y, world->mapGeneration, world->seed));
    unsigned int bucketIndex = hashChunkCoord(world, x, y);

    if (world->chunkArray[bucketIndex] == NULL){
        world->chunkArray[bucketIndex] = newNode;
    }
    else{
        newNode->next = world->chunkArray[bucketIndex];
        world->chunkArray[bucketIndex] = newNode;
        
    }

    world->numOfChunks++;
    
}

Chunk* getChunk(World* world, int x, int y){
    ChunkNode* current = world->chunkArray[hashChunkCoord(world, x, y)]; 
    while (current != NULL){
        if (current->xKey == x && current->yKey == y){
            return current->chunk;
        }
        current = current->next;
    }
    return NULL;
}

void renderChunks(World* world, int* renderList, int TotalChunks){

    for (int i = 0; i < TotalChunks;i++){
        int x = renderList[i*2];
        int y = renderList[i*2+1];
        Chunk* chunk = getChunk(world, x, y);
        drawChunk(chunk);
    }
}

void renderChunksWireFrame(World* world, int* renderList, int TotalChunks){

    for (int i = 0; i < TotalChunks;i++){
        int x = renderList[i*2];
        int y = renderList[i*2+1];
        Chunk* chunk = getChunk(world, x, y);
        drawChunkWireFrame(chunk);
    }
}

void genChunks(World* world, int* renderList, int TotalChunks){
    for (int i = 0; i < TotalChunks;i++){
        int x = renderList[i*2];
        int y = renderList[i*2+1];
        genChunk(world, x, y);
    }
}

void static freeNode(ChunkNode* node){

    if(node->next != NULL){
        freeNode(node->next);
    }
    free(node);
}

void destroyWorld(World* world){

    ChunkNode** chunkArray = world->chunkArray;
    for (int i = 0; i < WORLD_CAPACITY; i++){

        ChunkNode* node = chunkArray[i];
        if(node != NULL){
            freeNode(node);
        }
    }
    free(world);
}