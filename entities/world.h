#ifndef WORLD_H 
#define WORLD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "mesh.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>


typedef struct node{
    int xKey;
    int yKey;
    Chunk* chunk;
    struct node* next;
} ChunkNode;

#define WORLD_CAPACITY 128

typedef struct World{
    int numOfChunks;
    unsigned int seed;
    ChunkNode* chunkArray[WORLD_CAPACITY];
    blockType(*mapGeneration)(int16_t, int16_t, int16_t, uint8_t);
    pthread_mutex_t hashInsertLock;
} World;


World* initWorld(blockType(*mapGeneration)(int16_t, int16_t, int16_t, uint8_t));
Chunk* getChunk(World* world, int x, int y);
void renderChunks(World* world, int* renderList, int TotalChunks);
void genChunks(World* world, int* renderList, int TotalChunks);
void renderChunksWireFrame(World* world, int* renderList, int TotalChunks);
void destroyWorld(World* world);
blockType defaultMapGeneration(int16_t const x, int16_t const y, int16_t const z, uint8_t const seed);
blockType coolerMapGeneration(int16_t const x, int16_t const y, int16_t const z, uint8_t const seed);
void rebuildChunks(World* world, int* renderList, int TotalChunks);

#endif