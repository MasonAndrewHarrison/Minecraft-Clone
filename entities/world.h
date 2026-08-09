#ifndef WORLD_H 
#define WORLD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "mesh.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>


typedef struct node{
    int xKey;
    int yKey;
    Chunk* chunk;
    struct node* next;
} ChunkNode;

#define WORLD_CAPACITY 2

typedef struct World{
    int numOfChunks;
    ChunkNode* chunkArray[WORLD_CAPACITY];
} World;


World* initWorld();
Chunk* getChunk(World* world, int x, int y);
void genChunk(World* world, int x, int y);
void renderChunks(World* world, int* renderList, int TotalChunks);
void genChunks(World* world, int* renderList, int TotalChunks);
void renderChunksWireFrame(World* world, int* renderList, int TotalChunks);
void destroyWorld(World* world);

#endif