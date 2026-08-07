#include <cglm/cglm.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "mesh.h"



Mesh createMesh(float* vertices, int vertexCount, unsigned int* indices, int indexCount){

    Mesh mesh;
    mesh.indexCount = indexCount;

    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);

    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 8 * sizeof(float), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));

    glGenBuffers(1, &mesh.ibo   );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    return mesh;
}

Mesh createCube(vec3 position){

    float vertices[] = {
    // x,   y,   z,   r,   g,   b,   u,         v
    // front
    -0.5f,-0.5f, 0.5f,  0.0f,0.0f,0.0f,  0.333333f,0.0f,
    0.5f,-0.5f, 0.5f,  0.0f,0.0f,0.0f,  0.666667f,0.0f,
    0.5f, 0.5f, 0.5f,  0.0f,0.0f,0.0f,  0.666667f,1.0f,
    -0.5f, 0.5f, 0.5f,  0.0f,0.0f,0.0f,  0.333333f,1.0f,
    // back (reverted — this was never actually broken)
    -0.5f,-0.5f,-0.5f,  0.0f,0.0f,0.0f,  0.333333f,0.0f,
    0.5f,-0.5f,-0.5f,  0.0f,0.0f,0.0f,  0.666667f,0.0f,
    0.5f, 0.5f,-0.5f,  0.0f,0.0f,0.0f,  0.666667f,1.0f,
    -0.5f, 0.5f,-0.5f,  0.0f,0.0f,0.0f,  0.333333f,1.0f,
    // left (actually fixed now — v tracks y, was tracking z)
    -0.5f,-0.5f, 0.5f,  0.0f,0.0f,0.0f,  0.333333f,0.0f,
    -0.5f, 0.5f, 0.5f,  0.0f,0.0f,0.0f,  0.333333f,1.0f,
    -0.5f, 0.5f,-0.5f,  0.0f,0.0f,0.0f,  0.666667f,1.0f,
    -0.5f,-0.5f,-0.5f,  0.0f,0.0f,0.0f,  0.666667f,0.0f,
    // right (unchanged — this swap was correct)
    0.5f,-0.5f, 0.5f,  0.0f,0.0f,0.0f,  0.333333f,0.0f,
    0.5f, 0.5f, 0.5f,  0.0f,0.0f,0.0f,  0.333333f,1.0f,
    0.5f, 0.5f,-0.5f,  0.0f,0.0f,0.0f,  0.666667f,1.0f,
    0.5f,-0.5f,-0.5f,  0.0f,0.0f,0.0f,  0.666667f,0.0f,
    // top
    -0.5f, 0.5f, 0.5f,  0.0f,0.0f,0.0f,  0.0f,     0.0f,
    0.5f, 0.5f, 0.5f,  0.0f,0.0f,0.0f,  0.333333f,0.0f,
    0.5f, 0.5f,-0.5f,  0.0f,0.0f,0.0f,  0.333333f,1.0f,
    -0.5f, 0.5f,-0.5f,  0.0f,0.0f,0.0f,  0.0f,     1.0f,
    // bottom (reverted — this was never actually broken either)
    -0.5f,-0.5f, 0.5f,  0.0f,0.0f,0.0f,  0.666667f,0.0f,
    0.5f,-0.5f, 0.5f,  0.0f,0.0f,0.0f,  1.0f,     0.0f,
    0.5f,-0.5f,-0.5f,  0.0f,0.0f,0.0f,  1.0f,     1.0f,
    -0.5f,-0.5f,-0.5f,  0.0f,0.0f,0.0f,  0.666667f,1.0f,
    };

    unsigned int indices[] = {
    0, 1, 2,   2, 3, 0,
    4, 5, 6,   6, 7, 4,
    8, 9,10,  10,11, 8,
    12,13,14,  14,15,12,
    16,17,18,  18,19,16,
    20,21,22,  22,23,20,
    };

    return createMesh(vertices, 24, indices, 36);

}

void drawMesh(Mesh* mesh) {
    glBindVertexArray(mesh->vao);
    glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, NULL);
}

void drawLines(Mesh* mesh){
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBindVertexArray(mesh->vao);
    glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, NULL);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void destroyMesh(Mesh* mesh) {
    glDeleteBuffers(1, &mesh->vbo);
    glDeleteBuffers(1, &mesh->ibo);
    glDeleteVertexArrays(1, &mesh->vao);
}