#include <cglm/cglm.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "mesh.h"
#include <string.h>

Mesh createMesh(const int16_t* vertices, int vertexCount, const unsigned int* indices, int indexCount){

    Mesh mesh;
    mesh.indexCount = indexCount;

    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);

    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 8 * sizeof(int16_t), vertices, GL_STATIC_DRAW);   // int16_t here now

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_SHORT, GL_FALSE, sizeof(int16_t) * 8, (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_SHORT, GL_FALSE, sizeof(int16_t) * 8, (void*)(sizeof(int16_t) * 3));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_SHORT, GL_FALSE, sizeof(int16_t) * 8, (void*)(sizeof(int16_t) * 6));

    glGenBuffers(1, &mesh.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    return mesh;
}

void vertexChunkToChunk(VertexChunk* const vertexChunk, Chunk* chunk){
    chunk->mesh = createMesh(vertexChunk->vertices, SIZE_OF_FACE * vertexChunk->numberOfFaces,
                              vertexChunk->indices, SIZE_OF_FACES_INDEX * vertexChunk->numberOfFaces);
}

void drawMesh(Mesh* mesh) {
    glBindVertexArray(mesh->vao);
    glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, NULL);
}

void drawChunk(Chunk* chunk){
    if (chunk->mesh.indexCount != 0){
        drawMesh(&chunk->mesh);
    }
    
}

void drawChunkWireFrame(Chunk* chunk){
    if (chunk->mesh.indexCount != 0){
        drawLines(&chunk->mesh);
    }
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

void destroyChunk(Chunk* chunk){
    destroyMesh(&chunk->mesh);
    free(chunk->blockState);
    free(chunk);
}

static blockType* createBlockState(int xOffset, int yOffset, int* totalBlocks, 
    blockType(*mapGeneration)(int16_t const x, int16_t const y, int16_t const z, uint8_t const seed), uint8_t seed){

    blockType* blockTypeState = malloc(sizeof(blockType) * CHUNK_SIZE_CUBED * CHUNK_HEIGHT);
    *totalBlocks = 0;

    for (int z = 0; z < CHUNK_HEIGHT; z++){
        for (int j = 0; j < CHUNK_SIZE_CUBED; j++){
            int localX = j % CHUNK_SIZE;
            int localY = j / CHUNK_SIZE;  

            blockType type = mapGeneration(localX + xOffset*CHUNK_SIZE, localY + yOffset*CHUNK_SIZE, z, seed);
            blockTypeState[blockIndex(localX, localY, z)] = type;
            if (type != AIR){
                (*totalBlocks)++;
            }
        }
    }

    return blockTypeState;
}

/*
 *  This function need to have the rebuildChunk(..) function to be run on each
 *  chunk for the chunks generated to be fully build. After the Chunk is rebuild
 *  it also needs to be binded useing the bindChunk(..) function.
 */
Chunk* createChunk(int x, int y, blockType(*mapGeneration)(int16_t, int16_t, int16_t, uint8_t), uint8_t seed){

    Chunk* chunk = malloc(sizeof(Chunk));
    chunk->blockState = createBlockState(x, y, &chunk->totalBlocks, mapGeneration, seed);
    chunk->x=x;
    chunk->y=y;
    chunk->isBuild = false;
    
    return chunk;
}

VertexChunk* initVertexChunk(int totalBlocks){
    VertexChunk* vertexChunk = malloc(sizeof(VertexChunk));
    vertexChunk->vertices = malloc(6*totalBlocks * SIZE_OF_FACE * sizeof(float));
    vertexChunk->indices = malloc(6*totalBlocks * SIZE_OF_FACES_INDEX * sizeof(unsigned int));
    return vertexChunk;
} 

/*
 * Notes this does not free blockState because it is later
 * used by the Chunk structs later.
 */
void freeVertexChunk(VertexChunk* vertexChunk){

    for (int dir = TOP; dir <= BACK; dir++){
        free(vertexChunk->vertices);
        free(vertexChunk->indices);
    }
    free(vertexChunk);
}

static uint8_t checkVisiblity(blockType* blockState, int16_t x, int16_t y, int16_t z, uint8_t* dirList){
    if(blockState[blockIndex(x, y, z)] == AIR){
        return 0;
    }

    dirList[FRONT] = blockState[blockIndex(x, y+1, z)] == AIR || y == CHUNK_SIZE-1;
    dirList[BACK] = blockState[blockIndex(x, y-1, z)] == AIR || y == 0;
    dirList[LEFT] = blockState[blockIndex(x-1, y, z)] == AIR || x == 0;
    dirList[RIGHT] = blockState[blockIndex(x+1, y, z)] == AIR || x == CHUNK_SIZE-1;
    dirList[TOP] = blockState[blockIndex(x, y, z+1)] == AIR || z == CHUNK_HEIGHT-1;
    dirList[BOTTOM] = blockState[blockIndex(x, y, z-1)] == AIR || z == 0;

    return 1;
}

/*
 * This is espected to be run after checkVisiblity. Also check if it is 
 * the bottom block and won't render it.
 */
static uint8_t checkVisiblityBetweenChunks(int16_t x, int16_t y, int16_t z, uint8_t* dirList, AdjecentChunks* adjecentChunks){

    if (z == 0){
        return 0;
    }
    if (y == CHUNK_SIZE-1){
        Chunk* frontAdjecentChunk = adjecentChunks->front;
        if (frontAdjecentChunk != NULL){
            blockType* adjecentBlockStateFront = frontAdjecentChunk->blockState;
            uint8_t parallelBlockVisible = adjecentBlockStateFront[blockIndex(x, 0, z)] == AIR;
            dirList[FRONT] = parallelBlockVisible;
        }
    }
    else if (y == 0){
        Chunk* backAdjecentChunk = adjecentChunks->back;
        if (backAdjecentChunk != NULL){
            blockType* adjecentBlockStateBack = backAdjecentChunk->blockState;
            uint8_t parallelBlockVisible = adjecentBlockStateBack[blockIndex(x, CHUNK_SIZE-1, z)] == AIR;
            dirList[BACK] = parallelBlockVisible;
        }
    }

    if (x == CHUNK_SIZE-1){
        Chunk* rightAdjecentChunk = adjecentChunks->right;
        if (rightAdjecentChunk != NULL){
            blockType* adjecentBlockStateRight = rightAdjecentChunk->blockState;
            uint8_t parallelBlockVisible = adjecentBlockStateRight[blockIndex(0, y, z)] == AIR;
            dirList[RIGHT] = parallelBlockVisible;
        }
    }
    else if (x == 0){
        Chunk* leftAdjecentChunk = adjecentChunks->left;
        if (leftAdjecentChunk != NULL){
            blockType* adjecentBlockStateLeft = leftAdjecentChunk->blockState;
            uint8_t parallelBlockVisible = adjecentBlockStateLeft[blockIndex(CHUNK_SIZE-1, y, z)] == AIR;
            dirList[LEFT] = parallelBlockVisible;
        }
    }

    return 1;
}

void static setFaceType(int16_t* vertices, blockType type, CubeDirection dir, int corner){

    if (type == GRASS){
        vertices[6] = CUBE_UV_GRASS[corner * 2 + (dir*8)];
        vertices[7] = CUBE_UV_GRASS[corner * 2 + 1 + (dir*8)];
    }
    else if (type == WOOD){
        vertices[6] = CUBE_UV_WOOD[corner * 2 + (dir*8)];
        vertices[7] = CUBE_UV_WOOD[corner * 2 + 1 + (dir*8)];
    }
    else if (type == DIRT){
        vertices[6] = CUBE_UV_DIRT[corner * 2 + (dir*8)];
        vertices[7] = CUBE_UV_DIRT[corner * 2 + 1 + (dir*8)];
    }
    else{
        vertices[6] = CUBE_UV_DEFAULT[corner * 2 + (dir*8)];
        vertices[7] = CUBE_UV_DEFAULT[corner * 2 + 1 + (dir*8)];
    } 
}

void static appendFaceToVertexChunk(VertexChunk* vertexChunk, int cursorIndex, ivec3 position, blockType type, CubeDirection dir){

    unsigned int* destIndex = &vertexChunk->indices[cursorIndex * SIZE_OF_FACES_INDEX];
    memcpy(destIndex, &CUBE_INDICES_TEMPLATE, sizeof(unsigned int)*SIZE_OF_FACES_INDEX);   

    for (int i = 0; i < 6; i++){
        destIndex[i] += cursorIndex * 4;
    }

    int16_t* destVertex = &vertexChunk->vertices[cursorIndex * SIZE_OF_FACE];
    memcpy(destVertex, &CUBE_VERTICES[dir * SIZE_OF_FACE], sizeof(float)*SIZE_OF_FACE);

    for (int i = 0; i < 4; i++){
        destVertex[i*8] += position[0];
        destVertex[i*8+1] += position[1];
        destVertex[i*8+2] += position[2];
        setFaceType(&destVertex[i*8], type, dir, i);

    }
}

static void appendCubeToVertexChunk(VertexChunk* vertexChunk, int* cursorIndex, ivec3 position, blockType type, uint8_t* dirList){
    for(int dir = FRONT; dir < BOTTOM+1; dir++){
        if(dirList[dir] == 1){
            appendFaceToVertexChunk(vertexChunk, *cursorIndex, position, type, dir);
            (*cursorIndex)++;
        }
    }
}

VertexChunk* rebuildChunk(AdjecentChunks* adjectChunks){

    Chunk* chunk = adjectChunks->center;

    VertexChunk* vertexChunk = initVertexChunk(chunk->totalBlocks);

    int totalFaces = 0;
    for (int16_t z = 0; z < CHUNK_HEIGHT; z++){
        for (int16_t j = 0; j < CHUNK_SIZE_CUBED; j++){
            int16_t localX = j % CHUNK_SIZE;
            int16_t localY = j / CHUNK_SIZE;
            int16_t x = localX - (CHUNK_SIZE/2);
            int16_t y = localY - (CHUNK_SIZE/2);  
            blockType type;
            type = chunk->blockState[blockIndex(localX, localY, z)];

            uint8_t dirList[6];
            uint8_t isVisible = checkVisiblity(chunk->blockState, localX, localY, z, dirList);
            uint8_t isVisibleBetweenChunks = checkVisiblityBetweenChunks(
                localX, localY, z, dirList, adjectChunks);

            if (isVisible && isVisibleBetweenChunks){
                appendCubeToVertexChunk(vertexChunk, &totalFaces, 
                    (   ivec3){x + (chunk->x*16), z-((int)CHUNK_HEIGHT/2), y + (chunk->y*16)}, type, dirList);
            }
        }
    }

    vertexChunk->numberOfFaces = totalFaces;
    vertexChunk->blockState = chunk->blockState;

    return vertexChunk;
}

void bindChunk(Chunk* chunk, VertexChunk* vertexChunk){
    vertexChunkToChunk(vertexChunk, chunk);
    freeVertexChunk(vertexChunk);
}