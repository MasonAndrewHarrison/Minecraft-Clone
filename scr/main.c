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
#define PRELOADED_RADIUS 30
#define UNBUILD_RADIUS 250
#define RENDER_RADIUS 40
#define SPAWN_POSITION (vec3){573.0f, -6.0f, 9.0f}
#define INFINITE_WORLD_GEN true

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

    uint16_t spawnX = floorf(SPAWN_POSITION[0]/16.0f);
    uint16_t spawnY = floorf(SPAWN_POSITION[2]/16.0f);
    ChunkDoList genList = initCircleChunkDoList(spawnX, spawnY, PRELOADED_RADIUS);
    ChunkDoList rebuildList = initCircleChunkDoList(spawnX, spawnY, PRELOADED_RADIUS);
    ChunkDoList renderList;

    World* world = initWorld(coolerMapGeneration);

    struct timespec timePause = {0, 0};
    genChunks(world, &genList, timePause);
    rebuildChunks(world, &rebuildList, true);


    GenThreadArgs genArgs = { 
        .world = world, 
        .appState = state,
        .radius = RENDER_RADIUS+5,
    };
    atomic_init(&genArgs.running, 1);
    pthread_t genThread;
    pthread_create(&genThread, NULL, genThreadFn, &genArgs);


    double lastFPSTime = glfwGetTime();
    double lastDeltaTime = glfwGetTime();
    int frameCount = 0;

    glm_vec3_copy(SPAWN_POSITION, state->cam->eye);
    glm_vec3_copy(SPAWN_POSITION, state->cam->eye);

    int lastX = (int)state->cam->eye[0];
    int lastZ = (int)state->cam->eye[1];
    int lastY = (int)state->cam->eye[2];

    while (!glfwWindowShouldClose(state->window)) {

        int currentX = (int)state->cam->eye[0];
        int currentZ = (int)state->cam->eye[1];
        int currentY = (int)state->cam->eye[2];

        frameCount++;
        double now = glfwGetTime();

        if (now - lastFPSTime >= 1.0) {
            double fps = frameCount / (now - lastFPSTime);
            double frameTimeMs = 1000.0 / fps;

            char title[64];
            snprintf(title, sizeof(title), "Minecraft Clone | %.1f FPS | %.2f ms | Position (%d, %d, %d)",
                    fps, frameTimeMs, currentX, currentY, currentZ);
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
        
        renderList = initCircleChunkDoList(floorf(currentX/16.0f), floorf(currentY/16.0f), RENDER_RADIUS);

        if (movedToDifferentChunk(state->cam, &lastX, &lastY, &lastZ) && INFINITE_WORLD_GEN){
            rebuildChunks(world, &renderList, true);
            atomic_store(&genArgs.paused, 0);
        } else {
            atomic_store(&genArgs.paused, 1);
        }
        
        if (state->wireFrame == 1){renderChunksWireFrame(world, &renderList);}
        else {renderChunks(world, &renderList);}

        glfwSwapBuffers(state->window);
        glfwPollEvents();
    }

    atomic_store(&genArgs.running, 0);
    pthread_join(genThread, NULL);
    destroyWorld(world);
    destroyTexture(&atlas);
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}