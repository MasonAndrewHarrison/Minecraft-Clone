#ifndef WORLD_H 
#define WORLD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "mesh.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>


typedef struct ChunkNode{
    int xKey;
    int yKey;
    Chunk* chunk;

} ChunkNode;

typedef struct World{
    int numOfChunks;
    int capacity;
    ChunkNode** chunkArray;
} World;


World* initWorld();
void addChunk(World* world, Chunk* chunk);
Chunk* getChunk(World* world, int x, int y);
void genChunk(World* world, int x, int y);
void renderChunks(World* world, int* renderList, int TotalChunks);
void genChunks(World* world, int* renderList, int TotalChunks);
void renderChunksWireFrame(World* world, int* renderList, int TotalChunks);

#endif