#ifndef STATE_H
#define STATE_H

#include "../entities/camera.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../entities/mesh.h"

typedef struct State{
    int wireFrame;
    int needOfUpdate;
    camera* cam;
    GLFWwindow* window;
    double deltaTime;
    bool tabWasPressed;
    bool infiniteWorldGen;
    bool nearMaxOutOnRam;
    bool placeBlock;
    bool deleteBlock;
    bool leftWasClicked;
    bool rightWasClicked;
    blockType blockToPlace;
} State;

State* initState(GLFWwindow* window, const GLFWvidmode* mode, bool startWithInfiniteWorldGen);
double getSystemMemoryPercent();
#endif