#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "entities/camera.h"
#include <cglm/cglm.h>
#include "state.h"


#define forwardSpeed 0.5f
#define strifeSpeed 0.3f
#define upSpeed 0.1f
#define mouseSpeed 0.001f


void inputHandler(GLFWwindow* window, camera* cam, State* state){


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        moveLeftRight(cam, forwardSpeed);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        moveLeftRight(cam, (-1.0f)*forwardSpeed);
    }
        
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        moveForwardBackwards(cam, strifeSpeed);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        moveForwardBackwards(cam, (-1.0f)*strifeSpeed);
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
        moveUpDown(cam, upSpeed);
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){ 
        moveUpDown(cam, (-1.0f)*upSpeed);
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
        state->wireFrame=1;
    }else {state->wireFrame=0;}
}