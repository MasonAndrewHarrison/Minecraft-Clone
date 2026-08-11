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
#define VSYNC_INTERVAL 0

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

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);  

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

    ChunkDoList genList = initCircleChunkDoList(0, 0, 30);
    ChunkDoList rebuildList = initCircleChunkDoList(0, 0, 25);
    ChunkDoList renderList;

    World* world = initWorld(coolerMapGeneration);

    genChunks(world, &genList);
    rebuildChunks(world, &rebuildList, true);
    

    double lastFPSTime = glfwGetTime();
    double lastDeltaTime = glfwGetTime();
    int frameCount = 0;

    while (!glfwWindowShouldClose(state->window)) {

        frameCount++;
        double now = glfwGetTime();
        if (now - lastFPSTime >= 1.0) {
            double fps = frameCount / (now - lastFPSTime);
            double frameTimeMs = 1000.0 / fps;

            char title[64];
            snprintf(title, sizeof(title), "Minecraft Clone | %.1f FPS | %.2f ms", fps, frameTimeMs);
            glfwSetWindowTitle(state->window, title);

            frameCount = 0;
            lastFPSTime = now;
        }

        state->deltaTime = glfwGetTime() - lastDeltaTime;
        lastDeltaTime = glfwGetTime();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.3f, 0.6f, 0.9f, 1.0f);

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

        int currentX = state->cam->eye[0]/16;
        int currentY = state->cam->eye[2]/16;


        
        renderList = initCircleChunkDoList(currentX, currentY, 20);
        genList = initCircleChunkDoList(currentX, currentY, 25);
        genChunks(world, &genList);
        rebuildChunks(world, &renderList, true);

        if (state->wireFrame == 1){renderChunksWireFrame(world, &renderList);}
        else {renderChunks(world, &renderList);}

        glfwSwapBuffers(state->window);
        glfwPollEvents();
    }

    destroyWorld(world);
    destroyTexture(&atlas);
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}