#include "world.h"


ChunkDoList initChunkDoList(int width, int length, int offsetX, int offsetY){

    ChunkDoList doList;
    doList.size = width * length;
    doList.positionList = malloc(sizeof(int)*doList.size*2);

    for (int i = 0; i < length * width; i++){
        int localX = i % width;
        int localY = i / width;  
        doList.positionList[i*2] = localX - (int)width/2 + offsetX;
        doList.positionList[i*2+1] = localY - (int)length/2 + offsetY;
    }

    return doList;
}


ChunkDoList initCircleChunkDoList(int16_t centerX, int16_t centerY, int radius){

    ChunkDoList doList;
    int maxSize = (radius*2+1) * (radius*2+1);     
    doList.positionList = malloc(sizeof(int) * maxSize * 2);

    int count = 0;   

    for (int y = -radius; y <= radius; y++){
        for (int x = -radius; x <= radius; x++){
            if (x*x + y*y <= radius*radius){   
                doList.positionList[count*2]   = centerX + x;
                doList.positionList[count*2+1] = centerY + y;
                count++;
            }
        }
    }

    doList.size = count;
    return doList;
}


void removeChunkDoList(ChunkDoList* target, ChunkDoList* const cutter){


    free(target->positionList);
    target->positionList = malloc(sizeof(int) * target->size * 2);

    int targetX;
    int targetY;
    int newTargetSize = 0;

    int toolX;
    int toolY;

    for (int i = 0; i < target->size; i++){

        targetX = target->positionList[i*2];
        targetY = target->positionList[i*2+1];
        bool isInCutter = false;

        for (int j = 0; j < cutter->size; j++){
            toolX = cutter->positionList[j*2];
            toolY = cutter->positionList[j*2+1];

            if (targetX == toolX && targetY == toolY){
                isInCutter = true;
            }
            
        }
        if (isInCutter == false){
            target->positionList[newTargetSize*2] = targetX;
            target->positionList[newTargetSize*2+1] = targetY;
            newTargetSize++;
        }
    }

    target->size = newTargetSize;

}


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

blockType defaultMapGeneration(int16_t const x, int16_t const y, int16_t const z, uint8_t const seed){
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

float valueNoise2D(float x, float y, unsigned int seed){
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

    return nx0 + sy * (nx1 - nx0); 
}

blockType coolerMapGeneration(int16_t const x, int16_t const y, int16_t const z, uint8_t const seed){
    blockType type = DIRT;

    float frequency1 = 0.005f;
    float frequency2 = 0.01f;
    float frequency3 = 0.05f;
    float noise1 = valueNoise2D(x * frequency1, y * frequency1, seed);
    float noise2 = valueNoise2D(x * frequency2, y * frequency2, seed);
    float noise3 = valueNoise2D(x * frequency3, y * frequency3, seed);

    uint8_t height = 
        (uint8_t)(noise1 * 35.0f) + \
        (uint8_t)(noise2 * 55.0f) + \
        (uint8_t)((noise3) * 12.0f) + \
        CHUNK_HEIGHT/2 - 35; 

    if (height > 45){
        height = (height-45)/3 + 45;
    }
    else if (height < 25){
        height = (height-25)/4 + 25;
    }
    if (z > height){
        return AIR;
    }
    else if (z == height){
        return GRASS;
    }
    else if (z <= height-3){
        return STONE;
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


World* initWorld(blockType(*mapGenFunct)(int16_t, int16_t, int16_t, uint8_t)){
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

uint8_t chunkExist(World* world, int x, int y){
    ChunkNode* current = world->chunkArray[hashChunkCoord(world, x, y)]; 
    while (current != NULL){
        if (current->xKey == x && current->yKey == y){
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void renderChunks(World* world, ChunkDoList* renderList){

    for (int i = 0; i < renderList->size;i++){
        int x = renderList->positionList[i*2];
        int y = renderList->positionList[i*2+1];
        Chunk* chunk = getChunk(world, x, y);
        if (chunk != NULL){
            drawChunk(chunk);
        }
    }
}

void renderChunksWireFrame(World* world, ChunkDoList* renderList){

    for (int i = 0; i < renderList->size;i++){
        int x = renderList->positionList[i*2];
        int y = renderList->positionList[i*2+1];
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

/*
 * This uses multithreading to use the createChunk(..) function for each chunk in the renderList
 * and rebuildChunks(..) is expect to be run after to have chunks that are fully useable.
*/

void genChunks(World* world, ChunkDoList* genList, struct timespec pauseTime){
    
    pthread_t* threads = malloc(genList->size * sizeof(pthread_t));
    int threadsUsed = 0;
    genThreadInput* inputs = malloc(genList->size * sizeof(genThreadInput));

    for (int i = 0; i < genList->size; i++){
        inputs[i].world = world;
        inputs[i].x = genList->positionList[i*2];
        inputs[i].y = genList->positionList[i*2+1];
        if (chunkExist(world, inputs[i].x, inputs[i].y) == false){
            pthread_create(&threads[threadsUsed], NULL, genChunkThread, &inputs[i]);
            threadsUsed++;
        }

        if (pauseTime.tv_nsec > 0 || pauseTime.tv_sec > 0) {
            nanosleep(&pauseTime, NULL);
        }
    }

    for (int i = 0; i < threadsUsed; i++){
        pthread_join(threads[i], NULL);
    }

    free(inputs);
    free(threads);
}


static void* rebuildChunkThreadSafe(void* arg){
    AdjecentChunks* adjectChunks = arg;
    return rebuildChunk(adjectChunks);
}

/*
 * This rebuilds each chunk using the rebuildChunk(..) function. As each thread is joined or finshes
 * it job it binds it to the GPU using the bindChunk(..) function.
 */
void rebuildChunks(World* world, ChunkDoList* genList, bool unbuildOnly){

    pthread_t* threads = malloc(genList->size * sizeof(pthread_t));
    Chunk* savedChunkPtr[genList->size];
    AdjecentChunks adjectChunks[genList->size];

    for (int i = 0; i < genList->size;i++){

        int x = genList->positionList[i*2];
        int y = genList->positionList[i*2+1];
        savedChunkPtr[i] = getChunk(world, x, y);

        if (savedChunkPtr[i] == NULL){ continue; }
        if (unbuildOnly && savedChunkPtr[i]->isBuild == true){ continue; }

        adjectChunks[i].center = savedChunkPtr[i];
        adjectChunks[i].left = getChunk(world, x-1, y);
        adjectChunks[i].right = getChunk(world, x+1, y);
        adjectChunks[i].front = getChunk(world, x, y+1);
        adjectChunks[i].back = getChunk(world, x, y-1);
        pthread_create(&threads[i], NULL, rebuildChunkThreadSafe, &adjectChunks[i]);
    }

    for (int i = 0; i < genList->size; i++){

        if (savedChunkPtr[i] == NULL){ continue; }
        if (unbuildOnly && savedChunkPtr[i]->isBuild == true){ continue; }

        void* result = NULL;
        pthread_join(threads[i], &result);
        bindChunk(savedChunkPtr[i], (VertexChunk*)result);
        savedChunkPtr[i]->isBuild = true;
    }

    free(threads);
}

void static freeNode(ChunkNode* node){

    if(node->next != NULL){
        freeNode(node->next);
    }
    destroyChunk(node->chunk);
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


void* genThreadFn(void* arg){
    GenThreadArgs* args = arg;
    struct timespec timePause = {0, 100000};
    struct timespec pollInterval = {0, 10 * 1000000};
    int x;
    int y;
    while (atomic_load(&args->running)){

        if (atomic_load(&args->paused)){
            nanosleep(&pollInterval, NULL);
            continue;  
        }

        x = (int)floorf(args->appState->cam->eye[0] / 16.0f);
        y = (int)floorf(args->appState->cam->eye[2] / 16.0f);

        ChunkDoList genList = initCircleChunkDoList(x, y, args->radius);

        genChunks(args->world, &genList, timePause);
        nanosleep(&timePause, NULL);
    }
    return NULL;
}






bool static isInList(Chunk* chunk, ChunkDoList* list){
    for (int i = 0; i < list->size; i++){
        if ((list->positionList[i*2] == chunk->x) && (list->positionList[i*2+1] == chunk->y)){ return true;}
    }
    return false;
}

static ChunkNode* freeNodeChecked(ChunkNode* node, ChunkDoList* const exclusion){

    if (node == NULL) return NULL;

    node->next = freeNodeChecked(node->next, exclusion);  

    if (isInList(node->chunk, exclusion) == false){
        ChunkNode* rest = node->next;  
        destroyChunk(node->chunk);
        
        printf("FREE CHUNK %d, %d\n", node->chunk->x, node->chunk->y);
        free(node);
        return rest;               
    }

    return node; 
}


void checkEntireHash(World* world, ChunkDoList* const exclusion){
    ChunkNode** chunkArray = world->chunkArray;
    for (int i = 0; i < WORLD_CAPACITY; i++){
        chunkArray[i] = freeNodeChecked(chunkArray[i], exclusion);
    }
}

void* freeChunksThreadFn(void *arg){

    FreeThreadArgs* args = arg;
    struct timespec pollInterval = {0, 10 * 1000};
    
    while (atomic_load(&args->running)){

        if (atomic_load(&args->paused)){
            nanosleep(&pollInterval, NULL);
            continue;  
        }
        int x = (int)floorf(args->appState->cam->eye[0] / 16.0f);
        int y = (int)floorf(args->appState->cam->eye[2] / 16.0f);

        ChunkDoList exclusionList = initCircleChunkDoList(x, y, args->radius);  
        checkEntireHash(args->world, &exclusionList);

    }
    return NULL;
}



blockType getWorldBlock(World* world, int x, int y, int z){
    int chunkX = (int)floorf((float)x/16);
    int chunkY = (int)floorf((float)y/16);
    Chunk* chunk = getChunk(world, chunkX, chunkY);
    if (chunk == NULL) return AIR;  
    if (chunk->blockState == NULL) return AIR;

    int localX = ((x % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;
    int localY = ((y % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;
    int blockPos = blockIndex(localX, localY, z);
    if (blockPos > CHUNK_SIZE_CUBED * CHUNK_HEIGHT) {return AIR;}
    return chunk->blockState[blockPos];
}


RaycastHit raycastBlock(World* world, camera* cam, float maxDistance){
    RaycastHit result = {0};  
    vec3 front;
    glm_vec3_sub(cam->center, cam->eye, front);
    glm_vec3_normalize(front);

    float step = 0.01f; 
    float lastX = cam->eye[0];
    float lastY = cam->eye[2];
    float lastZ = cam->eye[1];  

    for (float t = 0.0f; t < maxDistance; t += step){

        float px = cam->eye[0] + front[0] * t;
        float py = cam->eye[2] + front[2] * t;
        float pz = cam->eye[1] + front[1] * t;

        int bx = (int)floorf(px);
        int by = (int)floorf(py);
        int bz = (int)floorf(pz);

        blockType type = getWorldBlock(world, bx, by, bz);

        if (type != AIR){
            result.hit = 1;
            result.breakX = bx;
            result.breakY = by;
            result.breakZ = bz;
            result.placeX = (int)floorf(lastX);
            result.placeY = (int)floorf(lastY);
            result.placeZ = (int)floorf(lastZ);
            return result; 
        }

        lastX = px;
        lastY = py;
        lastZ = pz;
    }

    return result;
}

void setWorldBlock(World* world, int x, int y, int z, blockType type){
    int chunkX = (int)floorf((float)x/16);
    int chunkY = (int)floorf((float)y/16);
    Chunk* chunk = getChunk(world, chunkX, chunkY);
    if (chunk == NULL) return;  
    if (chunk->blockState == NULL) return;

    int localX = ((x % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;
    int localY = ((y % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;
    int blockPos = blockIndex(localX, localY, z);
    if (blockPos > CHUNK_SIZE_CUBED * CHUNK_HEIGHT) {return;}
    chunk->blockState[blockPos] = type;
}