#ifndef MESH_H
#define MESH_H

#include "cglm/cglm.h"

typedef struct Mesh {
  unsigned int vao;
  unsigned int vbo;
  unsigned int ibo;
  int indexCount;
} Mesh;

typedef enum blockType {
  GRASS,
  WOOD,
  DIRT,
} blockType;

static const float CUBE_VERTICES[] = {
  // Position (X,Y,Z)      | Normal (X,Y,Z)     | UV (U,V)
  // --- Front Face ---
  -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 0.0f,   0.333333f, 0.0f,
  0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 0.0f,   0.666667f, 0.0f,
  0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 0.0f,   0.666667f, 1.0f,
  -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 0.0f,   0.333333f, 1.0f,

  // --- Back Face ---
  -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.333333f, 0.0f,
  0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.666667f, 0.0f,
  0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.666667f, 1.0f,
  -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.333333f, 1.0f,

  // --- Left Face ---
  -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 0.0f,   0.333333f, 0.0f,
  -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 0.0f,   0.333333f, 1.0f,
  -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.666667f, 1.0f,
  -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.666667f, 0.0f,

  // --- Right Face ---
  0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 0.0f,   0.333333f, 0.0f,
  0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 0.0f,   0.333333f, 1.0f,
  0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.666667f, 1.0f,
  0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.666667f, 0.0f,

  // --- Top Face ---
  -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 0.0f,   0.0f,      0.0f,
  0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 0.0f,   0.333333f, 0.0f,
  0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.333333f, 1.0f,
  -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.0f,      1.0f,

  // --- Bottom Face ---
  -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 0.0f,   0.666667f, 0.0f,
  0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 0.0f,   1.0f,      0.0f,
  0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   1.0f,      1.0f,
  -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.666667f, 1.0f,
};

#define CUBE_BYTES sizeof(CUBE_VERTICES)
#define CUBE_COUNT (sizeof(CUBE_VERTICES) / sizeof(float))
#define CUBE_VERTEX_LENGTH 8
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

Mesh createMesh(float *vertices, int vertexCount, const unsigned int *indices,
                int indexCount);
Mesh createCube(vec3 position, blockType type);
void drawMesh(Mesh *mesh);
void drawLines(Mesh *mesh);
void destroyMesh(Mesh *mesh);

#endif