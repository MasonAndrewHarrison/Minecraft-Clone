#ifndef MESH_H
#define MESH_H

#include "cglm/cglm.h"

typedef struct Mesh {
  unsigned int vao;
  unsigned int vbo;
  unsigned int ibo;
  int indexCount;
} Mesh;


typedef uint8_t blockType;
  #define DEFAULT 0
  #define GRASS 1
  #define WOOD 2
  #define DIRT 3
  #define AIR 4
  


#define ATLAS_STEP 1.0f/16.0f

static const float CUBE_VERTICES[] = {
  // Position (X,Y,Z)      | Normal (X,Y,Z)     | UV (U,V)
  // --- Front Face ---
  -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
  0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
  0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
  -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,

  // --- Back Face ---
  -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
  0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
  0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
  -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,

  // --- Left Face ---
  -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
  -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
  -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
  -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,

  // --- Right Face ---
  0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
  0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
  0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
  0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,

  // --- Top Face ---
  -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
  0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
  0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
  -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,

  // --- Bottom Face ---
  -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
  0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
  0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
  -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
};

static const float CUBE_UV_GRASS[] = {
  // --- Front Face --- 
  ATLAS_STEP*1, ATLAS_STEP*15,
  ATLAS_STEP*2, ATLAS_STEP*15,
  ATLAS_STEP*2, ATLAS_STEP*16,
  ATLAS_STEP*1, ATLAS_STEP*16,

  // --- Back Face ---
  ATLAS_STEP*1, ATLAS_STEP*15,
  ATLAS_STEP*2, ATLAS_STEP*15,
  ATLAS_STEP*2, ATLAS_STEP*16,
  ATLAS_STEP*1, ATLAS_STEP*16,

  // --- Left Face --- 
  ATLAS_STEP*2, ATLAS_STEP*15,
  ATLAS_STEP*2, ATLAS_STEP*16,
  ATLAS_STEP*1, ATLAS_STEP*16,
  ATLAS_STEP*1, ATLAS_STEP*15,

  // --- Right Face ---
  ATLAS_STEP*2, ATLAS_STEP*15,
  ATLAS_STEP*2, ATLAS_STEP*16,
  ATLAS_STEP*1, ATLAS_STEP*16,
  ATLAS_STEP*1, ATLAS_STEP*15,

  // --- Top Face ---
  ATLAS_STEP*0, ATLAS_STEP*15,
  ATLAS_STEP*1, ATLAS_STEP*15,
  ATLAS_STEP*1, ATLAS_STEP*16,
  ATLAS_STEP*0, ATLAS_STEP*16,

  // --- Bottom Face --- 
  ATLAS_STEP*2, ATLAS_STEP*15,
  ATLAS_STEP*3, ATLAS_STEP*15,
  ATLAS_STEP*3, ATLAS_STEP*16,
  ATLAS_STEP*2, ATLAS_STEP*16,
};

static const float CUBE_UV_WOOD[] = {
  // --- Front Face --- 
  ATLAS_STEP*2, ATLAS_STEP*14,
  ATLAS_STEP*3, ATLAS_STEP*14,
  ATLAS_STEP*3, ATLAS_STEP*15,
  ATLAS_STEP*2, ATLAS_STEP*15,

  // --- Back Face ---
  ATLAS_STEP*2, ATLAS_STEP*14,
  ATLAS_STEP*3, ATLAS_STEP*14,
  ATLAS_STEP*3, ATLAS_STEP*15,
  ATLAS_STEP*2, ATLAS_STEP*15,

  // --- Left Face --- 
  ATLAS_STEP*3, ATLAS_STEP*14,
  ATLAS_STEP*3, ATLAS_STEP*15,
  ATLAS_STEP*2, ATLAS_STEP*15,
  ATLAS_STEP*2, ATLAS_STEP*14,

  // --- Right Face ---
  ATLAS_STEP*3, ATLAS_STEP*14,
  ATLAS_STEP*3, ATLAS_STEP*15,
  ATLAS_STEP*2, ATLAS_STEP*15,
  ATLAS_STEP*2, ATLAS_STEP*14,

  // --- Top Face ---
  ATLAS_STEP*4, ATLAS_STEP*14,
  ATLAS_STEP*4, ATLAS_STEP*15,
  ATLAS_STEP*3, ATLAS_STEP*15,
  ATLAS_STEP*3, ATLAS_STEP*14,

  // --- Bottom Face --- 
  ATLAS_STEP*4, ATLAS_STEP*14,
  ATLAS_STEP*4, ATLAS_STEP*15,
  ATLAS_STEP*3, ATLAS_STEP*15,
  ATLAS_STEP*3, ATLAS_STEP*14,
};

static const float CUBE_UV_DIRT[] = {
  // --- Front Face --- 
  ATLAS_STEP*2, ATLAS_STEP*15,
  ATLAS_STEP*3, ATLAS_STEP*15,
  ATLAS_STEP*3, ATLAS_STEP*16,
  ATLAS_STEP*2, ATLAS_STEP*16,

  // --- Back Face ---
  ATLAS_STEP*2, ATLAS_STEP*15,
  ATLAS_STEP*3, ATLAS_STEP*15,
  ATLAS_STEP*3, ATLAS_STEP*16,
  ATLAS_STEP*2, ATLAS_STEP*16,

  // --- Left Face --- 
  ATLAS_STEP*2, ATLAS_STEP*15,
  ATLAS_STEP*3, ATLAS_STEP*15,
  ATLAS_STEP*3, ATLAS_STEP*16,
  ATLAS_STEP*2, ATLAS_STEP*16,

  // --- Right Face ---
  ATLAS_STEP*2, ATLAS_STEP*15,
  ATLAS_STEP*3, ATLAS_STEP*15,
  ATLAS_STEP*3, ATLAS_STEP*16,
  ATLAS_STEP*2, ATLAS_STEP*16,

  // --- Top Face ---
  ATLAS_STEP*2, ATLAS_STEP*15,
  ATLAS_STEP*3, ATLAS_STEP*15,
  ATLAS_STEP*3, ATLAS_STEP*16,
  ATLAS_STEP*2, ATLAS_STEP*16,

  // --- Bottom Face --- 
  ATLAS_STEP*2, ATLAS_STEP*15,
  ATLAS_STEP*3, ATLAS_STEP*15,
  ATLAS_STEP*3, ATLAS_STEP*16,
  ATLAS_STEP*2, ATLAS_STEP*16,
};

static const float CUBE_UV_DEFAULT[] = {
  // --- Front Face --- 
  ATLAS_STEP*15, ATLAS_STEP*15,
  ATLAS_STEP*16, ATLAS_STEP*15,
  ATLAS_STEP*16, ATLAS_STEP*16,
  ATLAS_STEP*15, ATLAS_STEP*16,

  // --- Back Face ---
  ATLAS_STEP*15, ATLAS_STEP*15,
  ATLAS_STEP*16, ATLAS_STEP*15,
  ATLAS_STEP*16, ATLAS_STEP*16,
  ATLAS_STEP*15, ATLAS_STEP*16,

  // --- Left Face --- 
  ATLAS_STEP*15, ATLAS_STEP*15,
  ATLAS_STEP*16, ATLAS_STEP*15,
  ATLAS_STEP*16, ATLAS_STEP*16,
  ATLAS_STEP*15, ATLAS_STEP*16,

  // --- Right Face ---
  ATLAS_STEP*15, ATLAS_STEP*15,
  ATLAS_STEP*16, ATLAS_STEP*15,
  ATLAS_STEP*16, ATLAS_STEP*16,
  ATLAS_STEP*15, ATLAS_STEP*16,

  // --- Top Face ---
  ATLAS_STEP*15, ATLAS_STEP*15,
  ATLAS_STEP*16, ATLAS_STEP*15,
  ATLAS_STEP*16, ATLAS_STEP*16,
  ATLAS_STEP*15, ATLAS_STEP*16,

  // --- Bottom Face --- 
  ATLAS_STEP*15, ATLAS_STEP*15,
  ATLAS_STEP*16, ATLAS_STEP*15,
  ATLAS_STEP*16, ATLAS_STEP*16,
  ATLAS_STEP*15, ATLAS_STEP*16,
};

#define CUBE_BYTES sizeof(CUBE_VERTICES)
#define CUBE_COUNT (sizeof(CUBE_VERTICES) / sizeof(float))
#define CUBE_VERTEX_LENGTH 8
#define CUBE_VERTEX_BYTES (sizeof(float) * CUBE_VERTEX_LENGTH)
#define CUBE_VERTEX_COUNT (CUBE_COUNT / CUBE_VERTEX_LENGTH)

static const unsigned int CUBE_INDICES[] = {
  0, 1, 2,   2, 3, 0,
  4, 5, 6,   6, 7, 4,
  8, 9,10,  10,11, 8,
  12,13,14,  14,15,12,
  16,17,18,  18,19,16,
  20,21,22,  22,23,20,
};

#define CUBE_INDEX_COUNT 36

typedef enum CubeDirection{
  FRONT = 0,
  BACK = 1,
  LEFT = 2,
  RIGHT = 3,
  TOP = 4,
  BOTTOM = 5,
} CubeDirection;


#define CHUNK_SIZE 16
#define CHUNK_HEIGHT 256
#define CHUNK_SIZE_CUBED (CHUNK_SIZE * CHUNK_SIZE)


typedef struct VertexChunk{
  float* vertices[6];
  unsigned int* indices[6];
  int sizeOfChunkLength;
  blockType* blockState;
} VertexChunk;


typedef struct Chunk{
  Mesh top;
  Mesh bottom;
  Mesh right;
  Mesh left;
  Mesh front;
  Mesh back;
  int x;
  int y;
  blockType* blockState;
} Chunk;


Mesh createMesh(float *vertices, int vertexCount, const unsigned int *indices,
                int indexCount);
Mesh createCube(vec3 position, blockType type);
void drawMesh(Mesh *mesh);
void drawChunk(Chunk* chunk);
void drawLines(Mesh *mesh);
void destroyMesh(Mesh *mesh);
void destroyChunk(Chunk* chunk);
void appendCube(float* vertices, unsigned int* indices, int cubeIndex, vec3 position, blockType);
void appendCubeToVertexChunk(VertexChunk* vertexChunk, int cubeIndex, vec3 position, blockType type);
void vertexChunkToChunk(VertexChunk* const vertexChunk, Chunk* chunk);
VertexChunk* createVertexChunk(int x, int y);
Chunk* createChunk(int x, int y);

#endif