#ifndef MESH_H
#define MESH_H

#include "cglm/cglm.h"
#include <stdlib.h>

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


static const int16_t CUBE_VERTICES[] = {
  // Position (X,Z,Y)      | Normal (X,Z,Y)     | UV (U,V)
  // Front Face  
0, 0,  1,   0, 0, 1,   0, 0,
1, 0,  1,   0, 0, 1,   0, 0,
1,  1,  1,   0, 0, 1,   0, 0,
0,  1,  1,   0, 0, 1,   0, 0,

  // Back Face
0, 0, 0,   0, 0, -1,   0, 0,
0,  1, 0,   0, 0, -1,   0, 0,
1,  1, 0,   0, 0, -1,   0, 0,
1, 0, 0,   0, 0, -1,   0, 0,

  // Left Face 
0, 0,  1,   -1, 0, 0,   0, 0,
0,  1,  1,   -1, 0, 0,   0, 0,
0,  1, 0,   -1, 0, 0,   0, 0,
0, 0, 0,   -1, 0, 0,   0, 0,

  // Right Face
1, 0,  1,   1, 0, 0,   0, 0,
1, 0, 0,   1, 0, 0,   0, 0,
1,  1, 0,   1, 0, 0,   0, 0,
1,  1,  1,   1, 0, 0,   0, 0,

  // Top Face
0,  1,  1,   0, 1, 0,   0, 0,
1,  1,  1,   0, 1, 0,   0, 0,
1,  1, 0,   0, 1, 0,   0, 0,
0,  1, 0,   0, 1, 0,   0, 0,

  // Bottom Face
0, 0,  1,   0, -1, 0,   0, 0,
0, 0, 0,   0, -1, 0,   0, 0,
1, 0, 0,   0, -1, 0,   0, 0,
1, 0,  1,   0, -1, 0,   0, 0,
};

static const int16_t CUBE_UV_GRASS[] = {
  // Front
1, 15,
2, 15,
2, 16,
1, 16,

  // Back
1, 15,
1, 16,
2, 16,
2, 15,

  // Left
2, 15,
2, 16,
1, 16,
1, 15,

  // Right 
2, 15,
1, 15,
1, 16,
2, 16,

  // Top 
0, 15,
1, 15,
1, 16,
0, 16,

  // Bottom
2, 15,
2, 16,
3, 16,
3, 15,
};

static const int16_t CUBE_UV_WOOD[] = {
  // Front 
  2, 14,
  3, 14,
  3, 15,
  2, 15,

    // Back
  2, 14,
  2, 15,
  3, 15,
  3, 14,

    // Left
  3, 14,
  3, 15,
  2, 15,
  2, 14,

    // Right
  3, 14,
  2, 14,
  2, 15,
  3, 15,

    // Top 
  4, 14,
  4, 15,
  3, 15,
  3, 14,

    // Bottom 
  4, 14,
  3, 14,
  3, 15,
  4, 15,
};

static const int16_t CUBE_UV_DIRT[] = {
  // Front Face 
  2, 15,
  3, 15,
  3, 16,
  2, 16,

  // Back Face
  2, 15,
  3, 15,
  3, 16,
  2, 16,

  // Left Face
  2, 15,
  3, 15,
  3, 16,
  2, 16,

  // Right Face
  2, 15,
  3, 15,
  3, 16,
  2, 16,

  // Top Face
  2, 15,
  3, 15,
  3, 16,
  2, 16,

  // Bottom Face
  2, 15,
  3, 15,
  3, 16,
  2, 16,
};

static const int16_t CUBE_UV_DEFAULT[] = {
  // Front Face
  15, 15,
  16, 15,
  16, 16,
  15, 16,

  // Back Face
  15, 15,
  16, 15,
  16, 16,
  15, 16,

  // Left Face
  15, 15,
  16, 15,
  16, 16,
  15, 16,

  // Right Face
  15, 15,
  16, 15,
  16, 16,
  15, 16,

  // Top Face
  15, 15,
  16, 15,
  16, 16,
  15, 16,

  // Bottom Face
  15, 15,
  16, 15,
  16, 16,
  15, 16,
};

#define CUBE_BYTES sizeof(CUBE_VERTICES)
#define CUBE_COUNT (int)(sizeof(CUBE_VERTICES) / sizeof(float))
#define CUBE_VERTEX_LENGTH 8
#define CUBE_VERTEX_BYTES (sizeof(float) * CUBE_VERTEX_LENGTH)
#define CUBE_VERTEX_COUNT (CUBE_COUNT / CUBE_VERTEX_LENGTH)

static const unsigned int CUBE_INDICES_TEMPLATE[] = {
  0, 1, 2,   2, 3, 0,
};

#define CUBE_INDEX_COUNT 36
#define SIZE_OF_FACE 32
#define SIZE_OF_FACES_INDEX 6

typedef enum CubeDirection{
  FRONT = 0,
  BACK = 1,
  LEFT = 2,
  RIGHT = 3,
  TOP = 4,
  BOTTOM = 5,
} CubeDirection;

#define CHUNK_SIZE 16
#define CHUNK_HEIGHT 64
#define CHUNK_SIZE_CUBED (CHUNK_SIZE * CHUNK_SIZE)


typedef struct VertexChunk{
  int16_t* vertices;
  unsigned int* indices;
  int numberOfFaces;
  blockType* blockState;
} VertexChunk;


typedef struct Chunk{
  Mesh mesh;
  int x;
  int y;
  blockType* blockState;
  int totalBlocks;
  bool isBuild;
} Chunk;

typedef struct AdjecentChunks{
    Chunk* center;
    Chunk* left;
    Chunk* right;
    Chunk* front;
    Chunk* back;
} AdjecentChunks;


Mesh createMesh(const int16_t* vertices, int vertexCount, const unsigned int* indices, int indexCount);
Mesh createCube(vec3 position, blockType type);
void drawMesh(Mesh *mesh);
void drawChunk(Chunk* chunk);
void drawLines(Mesh *mesh);
void drawChunkWireFrame(Chunk* chunk);
void destroyMesh(Mesh *mesh);
void destroyChunk(Chunk* chunk);
void appendCube(float* vertices, unsigned int* indices, int cubeIndex, vec3 position, blockType);
void vertexChunkToChunk(VertexChunk* const vertexChunk, Chunk* chunk);
Chunk* createChunk(int x, int y, blockType(*mapGeneration)(int16_t, int16_t, int16_t, uint8_t), uint8_t seed);
VertexChunk* rebuildChunk(AdjecentChunks* adjecentChunks);
void bindChunk(Chunk* chunk, VertexChunk* vertexChunk);

static inline int blockIndex(int x, int y, int z) {
    return x + y * CHUNK_SIZE + z * CHUNK_SIZE_CUBED;
}

#endif