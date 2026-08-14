#ifndef HANDLER_H
#define HANDLER_H

#include "../entities/camera.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "state.h"

void inputHandler(State* state);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

#endif