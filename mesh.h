#ifndef MESH_H
#define MESH_H

typedef struct Mesh {
  unsigned int vao;
  unsigned int vbo;
  unsigned int ibo;
  int indexCount;
} Mesh;

Mesh createMesh(float *vertices, int vertexCount, unsigned int *indices,
                int indexCount);
void drawMesh(Mesh *mesh);
void destroyMesh(Mesh *mesh);

#endif