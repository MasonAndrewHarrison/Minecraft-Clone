#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include "shader.h"
#include <cglm/cglm.h>
#include "camera.h"
#include "mesh.h"

#define WIDTH          1920
#define HEIGHT         1080
#define VSYNC_INTERVAL 1

int main(void) {
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Caustic Gaussian Demo", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(VSYNC_INTERVAL);
    glewExperimental = GL_TRUE;
    glewInit();
    printf("%s\n", glGetString(GL_VERSION));

    float positions[] = {
        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
    };

    unsigned int indices[] = {
        0, 1, 2,  2, 3, 0,
        4, 5, 6,  6, 7, 4,
        0, 3, 7,  7, 4, 0,
        1, 2, 6,  6, 5, 1,
        3, 2, 6,  6, 7, 3,
        0, 1, 5,  5, 4, 0,
    };

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


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

    float startTime = (float)glfwGetTime();

    camera mainCamera = cameraInit(WIDTH, HEIGHT);
    Mesh cube = createMesh(positions, 8 * 3, indices, 36);

    vec3 cubePositions[11] = {
        { 0.0f,  0.0f,  0.0f},
        { 1.0f,  0.0f,  -2.0f},
        {-1.0f,  0.0f,  0.0f},
        { 0.0f,  1.0f,  4.0f},
        { 0.0f, -2.0f,  3.0f},
        { 2.0f,  0.0f,  0.0f},
        {-2.0f,  0.0f,  1.0f},
        { 0.0f,  2.0f,  -1.0f},
        { 1.0f,  1.0f,  0.0f},
        {-1.0f, -1.0f,  0.0f},
        {-1.0f, -1.0f,  3.0f},
    };

    while (!glfwWindowShouldClose(window)) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        updateCameraLocation(window, &mainCamera);
        updateCameraDirection(&mainCamera, 0, 0);

        float currentTime = (float)glfwGetTime() - startTime;
        glUniform1f(timeLoc,       currentTime);
        glUniform2f(resolutionLoc, (float)WIDTH, (float)HEIGHT);
        glUniform2f(stretchLoc,    3.0f, 7.0f);
        glUniform1f(angleLoc,      currentTime);

        glm_lookat(mainCamera.eye, mainCamera.center, mainCamera.up, view);
        glm_mat4_mul(proj,  view,  pv);

        for (int i = 0; i < 11; i++) {
            glm_mat4_identity(model);
            glm_translate(model, cubePositions[i]);
            glm_mat4_mul(pv, model, mvp);
            glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, (float*)mvp);
            drawMesh(&cube);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    destroyMesh(&cube);
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}