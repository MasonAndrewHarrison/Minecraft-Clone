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

#define WORLD_CAPACITY 2

typedef struct World{
    int numOfChunks;
    unsigned int seed;
    ChunkNode* chunkArray[WORLD_CAPACITY];
    blockType(*mapGeneration)(int, int, int, unsigned int);
    pthread_mutex_t hashInsertLock;
} World;


World* initWorld(blockType(*mapGeneration)(int, int, int, unsigned int));
Chunk* getChunk(World* world, int x, int y);
void renderChunks(World* world, int* renderList, int TotalChunks);
void genChunks(World* world, int* renderList, int TotalChunks);
void renderChunksWireFrame(World* world, int* renderList, int TotalChunks);
void destroyWorld(World* world);
blockType defaultMapGeneration(int const x, int const y, int const z, unsigned int const seed);
blockType coolerMapGeneration(int const x, int const y, int const z, unsigned int const seed);

#endif