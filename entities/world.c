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
    uint8_t height = cheapFastHash(x, y, 0, seed) % 3 + CHUNK_HEIGHT/2 + 25;

    if (z > height){
        type = AIR; 
    }
    else if (z < height){
        type = DIRT;
    }
    return type;
}

static float smoothstep(float t){
    return t * t * (3.0f - 2.0f * t);
}

static float valueNoise2D(float x, float y, unsigned int seed){
    int x0 = (int)floorf(x);
    int y0 = (int)floorf(y);
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    float sx = smoothstep(x - (float)x0);
    float sy = smoothstep(y - (float)y0);

    float n00 = cheapFastHash((uint8_t)x0, (uint8_t)y0, 0, seed) / 255.0f;
    float n10 = cheapFastHash((uint8_t)x1, (uint8_t)y0, 0, seed) / 255.0f;
    float n01 = cheapFastHash((uint8_t)x0, (uint8_t)y1, 0, seed) / 255.0f;
    float n11 = cheapFastHash((uint8_t)x1, (uint8_t)y1, 0, seed) / 255.0f;

    float nx0 = n00 + sx * (n10 - n00);
    float nx1 = n01 + sx * (n11 - n01);

    return nx0 + sy * (nx1 - nx0);   // 0.0 .. 1.0
}

blockType coolerMapGeneration(int const x, int const y, int const z, unsigned int const seed){
    blockType type = GRASS;

    float frequency = 0.05f;
    float noise = valueNoise2D(x * frequency, y * frequency, seed);

    uint8_t height = (uint8_t)(noise * 20.0f) + CHUNK_HEIGHT/2 - 20; 

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
    pthread_mutex_init(&world->hashInsertLock, NULL);
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

typedef struct genThreadInput{
    World* world;
    int x;
    int y;
} genThreadInput;

static void* genChunkThread(void* arg){

    genThreadInput* input = arg;
    World* world = input->world;
    int x = input->x;
    int y = input->y;

    ChunkNode* newNode = setNode(createChunk(x, y, world->mapGeneration, world->seed));
    unsigned int bucketIndex = hashChunkCoord(world, x, y);

    pthread_mutex_lock(&world->hashInsertLock);
    if (world->chunkArray[bucketIndex] == NULL){
        world->chunkArray[bucketIndex] = newNode;
    }
    else{
        newNode->next = world->chunkArray[bucketIndex];
        world->chunkArray[bucketIndex] = newNode;
    }

    world->numOfChunks++;
    pthread_mutex_unlock(&world->hashInsertLock);
    return NULL;
    
}

static void* rebuildChunkThreadSafe(void* arg){
    AdjecentChunks* adjectChunks = arg;
    return rebuildChunk(adjectChunks);
}

/*
 * This uses multithreading to use the createChunk(..) function for each chunk in the renderList
 * and rebuilds each chunk using the rebuildChunk(..) function. As each thread is joined or finshes
 * it job it binds it to the GPU using the bindChunk(..) function.
*/

void genChunks(World* world, int* renderList, int TotalChunks){
    
    pthread_t* threads = malloc(TotalChunks * sizeof(pthread_t));
    genThreadInput* inputs = malloc(TotalChunks * sizeof(genThreadInput));

    for (int i = 0; i < TotalChunks; i++){
        inputs[i].world = world;
        inputs[i].x = renderList[i*2];
        inputs[i].y = renderList[i*2+1];
        pthread_create(&threads[i], NULL, genChunkThread, &inputs[i]);
    }

    for (int i = 0; i < TotalChunks; i++){
        pthread_join(threads[i], NULL);
    }

    free(inputs);

    Chunk* savedChunkPtr[TotalChunks];
    AdjecentChunks adjectChunks[TotalChunks];

    for (int i = 0; i < TotalChunks;i++){
        int x = renderList[i*2];
        int y = renderList[i*2+1];
        savedChunkPtr[i] = getChunk(world, x, y);
        adjectChunks[i].center = savedChunkPtr[i];
        adjectChunks[i].left = getChunk(world, x-1, y);
        adjectChunks[i].right = getChunk(world, x+1, y);
        adjectChunks[i].front = getChunk(world, x, y+1);
        adjectChunks[i].back = getChunk(world, x, y-1);
        pthread_create(&threads[i], NULL, rebuildChunkThreadSafe, &adjectChunks[i]);
    }

    for (int i = 0; i < TotalChunks; i++){
        void* result = NULL;
        pthread_join(threads[i], &result);
        bindChunk(savedChunkPtr[i], (VertexChunk*)result);
    }

    free(threads);
}

void static freeNode(ChunkNode* node){

    if(node->next != NULL){
        freeNode(node->next);
    }
    free(node);
}

void destroyWorld(World* world){

    pthread_mutex_destroy(&world->hashInsertLock);

    ChunkNode** chunkArray = world->chunkArray;
    for (int i = 0; i < WORLD_CAPACITY; i++){

        ChunkNode* node = chunkArray[i];
        if(node != NULL){
            freeNode(node);
        }
    }
    free(world);
}