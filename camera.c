#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include "camera.h"


camera cameraInit(int width, int height){

    camera cam = {
        .eye    = {0.0f, 0.0f, 3.0f},
        .center = {0.0f, 0.0f, 0.0f},
        .up     = {0.0f, 1.0f, 0.0f},
        .yaw = 0.0f,
        .pitch = 0.0f,
        .lastX = width /2.0f,
        .lastY = height / 2.0f,
        .firstMouse = 1,
    };

    return cam;
}

void moveCamera(vec3 move, camera* cam){
    glm_vec3_add(cam->eye, move, cam->eye);
    glm_vec3_add(cam->center, move, cam->center);
}

void updateCamera(GLFWwindow* window, camera* cam){

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        vec3 move = {0.0f, 0.0f, -0.1f};
        moveCamera(move, cam);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        vec3 move = {0.0f, 0.0f, 0.1f};
        moveCamera(move, cam);
    }
        
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        vec3 move = {-0.1f, 0.0f, -0.0f};
        moveCamera(move, cam);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        vec3 move = {0.1f, 0.0f, -0.0f};
        moveCamera(move, cam);
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
        vec3 move = {0.0f, 0.1f, -0.0f};
        moveCamera(move, cam);
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
        vec3 move = {0.0f, -0.1f, -0.0f};
        moveCamera(move, cam);
    }
}

void mouseCallback(GLFWwindow*  window, double xpos, double ypos){

    camera* cam = glfwGetWindowUserPointer(window);
    


}