#ifndef HANDLER_H
#define HANDLER_H

#include "entities/camera.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "state.h"

void inputHandler(GLFWwindow *window, camera *cam, State* state);

#endif