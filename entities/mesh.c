#include <cglm/cglm.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "mesh.h"
#include <string.h>

Mesh createMesh(float* vertices, int vertexCount, const unsigned int* indices, int indexCount){

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

Mesh createCube(vec3 position, blockType type){

    float vertices[CUBE_COUNT];
    memcpy(vertices, CUBE_VERTICES, CUBE_BYTES);

    for (int i = 0; i < CUBE_VERTEX_COUNT; i++){
        float* vertexPos = &vertices[i * CUBE_VERTEX_LENGTH];
        glm_vec3_add(vertexPos, position, vertexPos);
        
        if (type == GRASS){
            vertices[i * CUBE_VERTEX_LENGTH+6] = CUBE_UV_GRASS[i * 2];
            vertices[i * CUBE_VERTEX_LENGTH+7] = CUBE_UV_GRASS[i * 2 + 1];
        }
        else if (type == WOOD){
            vertices[i * CUBE_VERTEX_LENGTH+6] = CUBE_UV_WOOD[i * 2];
            vertices[i * CUBE_VERTEX_LENGTH+7] = CUBE_UV_WOOD[i * 2 + 1];
        }
        else if (type == DIRT){
            vertices[i * CUBE_VERTEX_LENGTH+6] = CUBE_UV_DIRT[i * 2];
            vertices[i * CUBE_VERTEX_LENGTH+7] = CUBE_UV_DIRT[i * 2 + 1];
        }
        else{
            vertices[i * CUBE_VERTEX_LENGTH+6] = CUBE_UV_DEFAULT[i * 2];
            vertices[i * CUBE_VERTEX_LENGTH+7] = CUBE_UV_DEFAULT[i * 2 + 1];
        }
        
    }


    return createMesh(vertices, CUBE_VERTEX_COUNT, CUBE_INDICES, CUBE_INDEX_COUNT);
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