#include "world.h"



ChunkNode* setNode(Chunk* chunk){
    ChunkNode* node = malloc(sizeof(ChunkNode));
    node->xKey = chunk->x;
    node->yKey = chunk->y;
    node->chunk = chunk;
    return node;
}


World* initWorld(){
    World* world = malloc(sizeof(World));
    world->capacity = 128;
    world->numOfChunks = 0;
    world->chunkArray = malloc(sizeof(ChunkNode)*world->capacity);
    return world;
}

unsigned int hashChunkCoord(World* world, int x, int y) {
    unsigned int h = (unsigned int)x * 0x9E3779B1u;
    h ^= (unsigned int)y * 0x85EBCA6Bu;
    h ^= h >> 15;
    h *= 0x27D4EB2Fu;
    h ^= h >> 15;
    return h % world->capacity;
}

void addChunk(World* world, Chunk* chunk){
    ChunkNode* node = setNode(chunk);
    world->chunkArray[hashChunkCoord(world, chunk->x, chunk->y)] = node;
}

void genChunk(World* world, int x, int y){
    Chunk* chunk = createChunk(x, y);
    ChunkNode* node = setNode(chunk);
    world->chunkArray[hashChunkCoord(world, x, y)] = node;
}

Chunk* getChunk(World* world, int x, int y){
    return world->chunkArray[hashChunkCoord(world, x, y)]->chunk;
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