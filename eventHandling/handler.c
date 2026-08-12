#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../entities/camera.h"
#include <cglm/cglm.h>
#include "state.h"


#define forwardSpeed 50.0f
#define strifeSpeed 30.0f
#define upSpeed 10.0f


void inputHandler(State* state){

    double deltaTime = state->deltaTime;

    if (glfwGetKey(state->window, GLFW_KEY_SPACE) == GLFW_PRESS){
        deltaTime *= 20;
    }

    if (glfwGetKey(state->window, GLFW_KEY_W) == GLFW_PRESS){
        moveLeftRight(state->cam, forwardSpeed * deltaTime);
    }

    if (glfwGetKey(state->window, GLFW_KEY_S) == GLFW_PRESS){
        moveLeftRight(state->cam, (-1.0f)*forwardSpeed * deltaTime);
    }
        
    if (glfwGetKey(state->window, GLFW_KEY_A) == GLFW_PRESS){
        moveForwardBackwards(state->cam, strifeSpeed * deltaTime);
    }

    if (glfwGetKey(state->window, GLFW_KEY_D) == GLFW_PRESS){
        moveForwardBackwards(state->cam, (-1.0f)*strifeSpeed * deltaTime);
    }

    if (glfwGetKey(state->window, GLFW_KEY_E) == GLFW_PRESS){
        moveUpDown(state->cam, upSpeed * deltaTime);
    }

    if (glfwGetKey(state->window, GLFW_KEY_Q) == GLFW_PRESS){ 
        moveUpDown(state->cam, (-1.0f)*upSpeed * deltaTime);
    }

    if (glfwGetKey(state->window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(state->window, GLFW_TRUE);
    }

    if (glfwGetKey(state->window, GLFW_KEY_R) == GLFW_PRESS){
        state->wireFrame=1;
    }else {state->wireFrame=0;}

    if (glfwGetKey(state->window, GLFW_KEY_F) == GLFW_PRESS){
        state->needOfUpdate = 1;
    }
}