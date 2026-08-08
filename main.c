#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include "shader.h"
#include <cglm/cglm.h>
#include "entities/camera.h"
#include "texture.h"
#include "entities/mesh.h"
#include "handler.h"


#define WIDTH          1920
#define HEIGHT         1080
#define VSYNC_INTERVAL 1

int main(void) {

    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Minecraft Clone", NULL, NULL);
    
    if (!window) { 
        glfwTerminate(); 
        return -1; 
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(VSYNC_INTERVAL);
    glewExperimental = GL_TRUE;
    glewInit();
    printf("%s\n", glGetString(GL_VERSION));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Texture atlas = loadTexture("textures/atlas.png");

    unsigned int shader = createShader("shaders/vertex.glsl", "shaders/fragment.glsl");
    glUseProgram(shader);

    mat4 proj  = GLM_MAT4_IDENTITY_INIT;
    mat4 view  = GLM_MAT4_IDENTITY_INIT;
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    mat4 pv;
    mat4 mvp;

    glm_perspective(glm_rad(45.0f), (float)WIDTH/HEIGHT, 0.1f, 100.0f, proj);

    int timeLoc       = glGetUniformLocation(shader, "uTime");
    int resolutionLoc = glGetUniformLocation(shader, "uResolution");
    int stretchLoc    = glGetUniformLocation(shader, "uStretch");
    int angleLoc      = glGetUniformLocation(shader, "uAngle");
    int MVPLoc        = glGetUniformLocation(shader, "uMVP");
    int textureLoc    = glGetUniformLocation(shader, "uTexture");

    float startTime = (float)glfwGetTime();

    camera mainCamera = cameraInit(WIDTH, HEIGHT);
    glfwSetWindowUserPointer(window, &mainCamera);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);


    Chunk* chunk = malloc(sizeof(float)*256*256);
    createChunk(chunk);

    while (!glfwWindowShouldClose(window)) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        inputHandler(window, &mainCamera);

        float currentTime = (float)glfwGetTime() - startTime;
        glUniform1f(timeLoc,       currentTime);
        glUniform2f(resolutionLoc, (float)WIDTH, (float)HEIGHT);
        glUniform2f(stretchLoc,    3.0f, 7.0f);
        glUniform1f(angleLoc,      currentTime);
        glUniform1i(textureLoc, 0);
        bindTexture(&atlas, 0);

        glm_lookat(mainCamera.eye, mainCamera.center, mainCamera.up, view);
        glm_mat4_mul(proj,  view,  pv);

        glm_mat4_identity(model);
        glm_mat4_mul(pv, model, mvp);
        glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, (float*)mvp);

        drawMesh(&chunk->top);
        drawMesh(&chunk->front);
        drawMesh(&chunk->bottom);
        drawMesh(&chunk->back);
        drawMesh(&chunk->left);
        drawMesh(&chunk->right);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //destroyMesh(chunk);
    destroyTexture(&atlas);
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}