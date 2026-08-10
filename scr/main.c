#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include "../shaders/shader.h"
#include <cglm/cglm.h>
#include "../entities/camera.h"
#include "../textures/texture.h"
#include "../eventHandling/handler.h"
#include "../eventHandling/state.h"
#include "../entities/world.h"


#define WIDTH          1920
#define HEIGHT         1080
#define VSYNC_INTERVAL 1

int main(void) {

    
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    State* state = malloc(sizeof(State));
    state->wireFrame = 0;
    state->needOfUpdate = 0;
    state->window = glfwCreateWindow(WIDTH, HEIGHT, "Minecraft Clone", NULL, NULL);
    state->cam = cameraInit(WIDTH, HEIGHT);

    
    if (!state->window) { 
        glfwTerminate(); 
        return -1; 
    }

    glfwMakeContextCurrent(state->window);
    glfwSwapInterval(VSYNC_INTERVAL);
    glewExperimental = GL_TRUE;
    glewInit();
    printf("%s\n", glGetString(GL_VERSION));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Texture atlas = loadTexture("textures/images/atlas.png");

    unsigned int shader = createShader("shaders/vertex.glsl", "shaders/fragment.glsl");
    glUseProgram(shader);

    mat4 proj  = GLM_MAT4_IDENTITY_INIT;
    mat4 view  = GLM_MAT4_IDENTITY_INIT;
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    mat4 pv;
    mat4 mvp;

    glm_perspective(glm_rad(45.0f), (float)WIDTH/HEIGHT, 0.1f, 1000.0f, proj);

    int timeLoc       = glGetUniformLocation(shader, "uTime");
    int resolutionLoc = glGetUniformLocation(shader, "uResolution");
    int stretchLoc    = glGetUniformLocation(shader, "uStretch");
    int angleLoc      = glGetUniformLocation(shader, "uAngle");
    int MVPLoc        = glGetUniformLocation(shader, "uMVP");
    int textureLoc    = glGetUniformLocation(shader, "uTexture");

    float startTime = (float)glfwGetTime();

    glfwSetWindowUserPointer(state->window, state->cam);
    glfwSetInputMode(state->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(state->window, mouseCallback);

    unsigned int length = 25;
    unsigned int width = 25;
    unsigned int worldSize = length*width;
    
    int renderList[worldSize*2];

    for (int i = 0; i < length * width; i++){
        int localX = i % width;
        int localY = i / width;  
        renderList[i*2] = localX - (int)width/2;
        renderList[i*2+1] = localY - (int)length/2;
    }

    World* world = initWorld(coolerMapGeneration);

    genChunks(world, renderList, worldSize);
    rebuildChunks(world, renderList, worldSize);

    while (!glfwWindowShouldClose(state->window)) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        inputHandler(state);

        float currentTime = (float)glfwGetTime() - startTime;
        glUniform1f(timeLoc,       currentTime);
        glUniform2f(resolutionLoc, (float)WIDTH, (float)HEIGHT);
        glUniform2f(stretchLoc,    3.0f, 7.0f);
        glUniform1f(angleLoc,      currentTime);
        glUniform1i(textureLoc, 0);
        bindTexture(&atlas, 0);

        glm_lookat(state->cam->eye, state->cam->center, state->cam->up, view);
        glm_mat4_mul(proj,  view,  pv);

        glm_mat4_identity(model);
        glm_mat4_mul(pv, model, mvp);
        glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, (float*)mvp);

        if (state->wireFrame == 1){renderChunksWireFrame(world, renderList, worldSize);}
        else {renderChunks(world, renderList, worldSize);}

        glfwSwapBuffers(state->window);
        glfwPollEvents();
    }

    destroyWorld(world);
    destroyTexture(&atlas);
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}