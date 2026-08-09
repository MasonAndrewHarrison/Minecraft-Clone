#include "world.h"



ChunkNode* setNode(Chunk* chunk){
    ChunkNode* node = malloc(sizeof(ChunkNode));
    node->xKey = chunk->x;
    node->yKey = chunk->y;
    node->chunk = chunk;
    node->next = NULL;
    return node;
}


World* initWorld(){
    World* world = malloc(sizeof(World));
    world->numOfChunks = 0;
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
    ChunkNode* newNode = setNode(createChunk(x, y));
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