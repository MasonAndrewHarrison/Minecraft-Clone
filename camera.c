#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include "camera.h"
#include <math.h>

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

void updateCameraLocation(GLFWwindow* window, camera* cam){

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

void mouseCallback(GLFWwindow *window, double xpos, double ypos){

    camera* cam = glfwGetWindowUserPointer(window);

    double dxpos = xpos - cam->lastX;
    double dypos = ypos - cam->lastY;

    cam->lastX = xpos;
    cam->lastY = ypos;

    vec2 eyeVector = {cam->eye[0] - cam->center[0], cam->eye[2] - cam->center[2]};
    float radius = glm_vec2_distance((vec2){0.0f, 0.0f}, eyeVector);
    cam->yaw += dxpos/1000;

    eyeVector[0] = radius * cos(cam->yaw);
    eyeVector[1] = radius * sin(cam->yaw);

    cam->eye[0] = eyeVector[0] + cam->center[0];
    cam->eye[2] = eyeVector[1] + cam->center[2];

}
