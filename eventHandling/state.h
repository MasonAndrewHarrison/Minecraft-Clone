#ifndef STATE_H
#define STATE_H

#include "../entities/camera.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef struct State{
    int wireFrame;
    int needOfUpdate;
    camera* cam;
    GLFWwindow* window;
    double deltaTime;
    bool tabWasPressed;
    bool infiniteWorldGen;
    bool nearMaxOutOnRam;
} State;

State* initState(GLFWwindow* window, const GLFWvidmode* mode, bool startWithInfiniteWorldGen);
double getSystemMemoryPercent();
#endif