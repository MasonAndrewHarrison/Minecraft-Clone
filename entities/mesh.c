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

void setBlockType(float* vertices, int cubeIndex, blockType type){

    if (type == GRASS){
        vertices[cubeIndex * CUBE_VERTEX_LENGTH+6] = CUBE_UV_GRASS[cubeIndex * 2];
        vertices[cubeIndex * CUBE_VERTEX_LENGTH+7] = CUBE_UV_GRASS[cubeIndex * 2 + 1];
    }
    else if (type == WOOD){
        vertices[cubeIndex * CUBE_VERTEX_LENGTH+6] = CUBE_UV_WOOD[cubeIndex * 2];
        vertices[cubeIndex * CUBE_VERTEX_LENGTH+7] = CUBE_UV_WOOD[cubeIndex * 2 + 1];
    }
    else if (type == DIRT){
        vertices[cubeIndex * CUBE_VERTEX_LENGTH+6] = CUBE_UV_DIRT[cubeIndex * 2];
        vertices[cubeIndex * CUBE_VERTEX_LENGTH+7] = CUBE_UV_DIRT[cubeIndex * 2 + 1];
    }
    else{
        vertices[cubeIndex * CUBE_VERTEX_LENGTH+6] = CUBE_UV_DEFAULT[cubeIndex * 2];
        vertices[cubeIndex * CUBE_VERTEX_LENGTH+7] = CUBE_UV_DEFAULT[cubeIndex * 2 + 1];
    } 
}

void static appendCubeToVertexChunkDir(VertexChunk* vertexChunk, int cubeIndex, vec3 position, blockType type, CubeDirection dir){

    float* dest = &vertexChunk->vertices[dir][cubeIndex * CUBE_COUNT];

    int faceStartVertex = 4 * dir;
    int faceEndVertex   = 4 * (dir + 1);
    size_t faceBytes = 4 * CUBE_VERTEX_LENGTH * sizeof(float);

    memcpy(&dest[faceStartVertex * CUBE_VERTEX_LENGTH],
        &CUBE_VERTICES[faceStartVertex * CUBE_VERTEX_LENGTH],
        faceBytes);

    for (int i = faceStartVertex; i < faceEndVertex; i++){

        float* vertexPos = &dest[i*CUBE_VERTEX_LENGTH];
        glm_vec3_add(vertexPos, position, vertexPos);
        setBlockType(dest, i, type);
    }

    unsigned int* indices = vertexChunk->indices[dir];

    for (int i = 6 * dir; i < 6 * (dir+1); i++){
        indices[i + cubeIndex * CUBE_INDEX_COUNT] = CUBE_INDICES[i] + cubeIndex * CUBE_VERTEX_COUNT;
    }
}

void appendCubeToVertexChunk(VertexChunk* vertexChunk, int cubeIndex, vec3 position, blockType type){
    for(int dir = FRONT; dir < BOTTOM+1; dir++){
        appendCubeToVertexChunkDir(vertexChunk, cubeIndex, position, type, dir);
    }
    
}

void drawMesh(Mesh* mesh) {
    glBindVertexArray(mesh->vao);
    glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, NULL);
}

void drawChunk(Chunk* chunk){
    drawMesh(&chunk->top);
    drawMesh(&chunk->front);
    drawMesh(&chunk->bottom);
    drawMesh(&chunk->back);
    drawMesh(&chunk->left);
    drawMesh(&chunk->right);
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

static inline int blockIndex(int x, int y, int z) {
    return x + y * CHUNK_SIZE + z * CHUNK_SIZE_CUBED;
}

void destroyChunk(Chunk* chunk){
    destroyMesh(&chunk->back);
    destroyMesh(&chunk->front);
    destroyMesh(&chunk->top);
    destroyMesh(&chunk->bottom);
    destroyMesh(&chunk->left);
    destroyMesh(&chunk->right);
    free(chunk->blockState);
    free(chunk);
}

inline void static _createChunkElement(VertexChunk* const vertexChunk, int const chunkSize, CubeDirection const dir, Mesh* element){
    *element = createMesh(vertexChunk->vertices[dir], CUBE_VERTEX_COUNT * chunkSize, 
    vertexChunk->indices[dir], CUBE_INDEX_COUNT * chunkSize);
}

void vertexChunkToChunk(VertexChunk* const vertexChunk, Chunk* chunk){
    int size = vertexChunk->sizeOfChunkLength;
    _createChunkElement(vertexChunk, size, TOP, &chunk->top);
    _createChunkElement(vertexChunk, size, BOTTOM, &chunk->bottom);
    _createChunkElement(vertexChunk, size, LEFT, &chunk->left);
    _createChunkElement(vertexChunk, size, RIGHT, &chunk->right);
    _createChunkElement(vertexChunk, size, FRONT, &chunk->front);
    _createChunkElement(vertexChunk, size, BACK, &chunk->back);
}

VertexChunk* createVertexChunk(int xOffset, int yOffset){

    int chunkSize = CHUNK_SIZE_CUBED * CHUNK_HEIGHT;
    blockType* blockTypeState = malloc(sizeof(blockType) * CHUNK_SIZE_CUBED * CHUNK_HEIGHT);

    VertexChunk* vertexChunk = malloc(sizeof(VertexChunk));
    for (int dir = FRONT; dir <= BOTTOM; dir++){
        vertexChunk->vertices[dir] = malloc(chunkSize * CUBE_COUNT * sizeof(float));
        vertexChunk->indices[dir] = malloc(chunkSize * CUBE_INDEX_COUNT * sizeof(unsigned int));
    }

    int totalIndex = 0;
    for (int z = 0; z < CHUNK_HEIGHT; z++){
        for (int j = 0; j < CHUNK_SIZE_CUBED; j++){
            int localX = j % CHUNK_SIZE;
            int localY = j / CHUNK_SIZE;
            int x = localX - (CHUNK_SIZE/2);
            int y = localY - (CHUNK_SIZE/2);  
            blockType type;
            type = rand() % 3 + 1;

            if (rand() % 10 <= z-118){
                blockTypeState[blockIndex(localX, localY, z)] = AIR;
            }
            else{
                blockTypeState[blockIndex(localX, localY, z)] = type;
                appendCubeToVertexChunk(vertexChunk, totalIndex, (vec3){x + (xOffset*16), z-(CHUNK_HEIGHT/2), y + (yOffset*16)}, type);
                totalIndex++;
            }
        }
    }

    vertexChunk->sizeOfChunkLength = totalIndex;
    vertexChunk->blockState = blockTypeState;

    return vertexChunk;
}

Chunk* createChunk(int x, int y){

    VertexChunk* vertexChunk = createVertexChunk(x, y);
    Chunk* chunk = malloc(sizeof(Chunk));
    chunk->blockState = vertexChunk->blockState;
    chunk->x=x;
    chunk->y=y;
    vertexChunkToChunk(vertexChunk, chunk);

    for (int dir = TOP; dir <= BACK; dir++){
        free(vertexChunk->vertices[dir]);
        free(vertexChunk->indices[dir]);
    }
    free(vertexChunk);
    return chunk;
}