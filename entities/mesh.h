#ifndef MESH_H
#define MESH_H

#include "cglm/cglm.h"

typedef struct Mesh {
  unsigned int vao;
  unsigned int vbo;
  unsigned int ibo;
  int indexCount;
} Mesh;

Mesh createMesh(float *vertices, int vertexCount, unsigned int *indices,
                int indexCount);
Mesh createCube(vec3 position);
void drawMesh(Mesh *mesh);
void drawLines(Mesh *mesh);
void destroyMesh(Mesh *mesh);

#endif