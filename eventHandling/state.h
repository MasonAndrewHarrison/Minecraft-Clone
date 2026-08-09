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
} State;

#endif