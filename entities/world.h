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


typedef struct ChunkDoList{
    int* positionList;
    int size;
} ChunkDoList;

ChunkDoList initChunkDoList(int width, int length);
ChunkDoList initCircleChunkDoList(int16_t centerX, int16_t centerY, int radius);
World* initWorld(blockType(*mapGeneration)(int16_t, int16_t, int16_t, uint8_t));
void genChunks(World* world, ChunkDoList* genList);
void renderChunks(World* world, ChunkDoList* renderList);
ChunkDoList removeChunkDoList(ChunkDoList const target, ChunkDoList const cutter);
void renderChunksWireFrame(World* world, ChunkDoList* renderList);
void destroyWorld(World* world);
blockType defaultMapGeneration(int16_t const x, int16_t const y, int16_t const z, uint8_t const seed);
blockType coolerMapGeneration(int16_t const x, int16_t const y, int16_t const z, uint8_t const seed);
void rebuildChunks(World* world, ChunkDoList* genList, bool unbuildOnly);

#endif