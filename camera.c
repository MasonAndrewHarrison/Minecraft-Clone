#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include "camera.h"
#include <math.h> 

#define forwardSpeed 0.5f
#define strifeSpeed 0.2f

camera cameraInit(int width, int height){

    camera cam = {
        .eye    = {0.0f, 0.0f, 0.0f},
        .center = {3.0f, 0.0f, 0.0f},
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
        vec3 move =  {cos(cam->yaw) * forwardSpeed, 0.0f, sin(cam->yaw) * forwardSpeed};
        moveCamera(move, cam);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        vec3 move = {-cos(cam->yaw) * forwardSpeed, 0.0f, -sin(cam->yaw) * forwardSpeed};
        moveCamera(move, cam);
    }
        
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        vec3 move = {-cos(cam->yaw + 3.14f * 0.5f) * strifeSpeed, 0.0f, -sin(cam->yaw + 3.14f * 0.5f) * strifeSpeed};
        moveCamera(move, cam);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        vec3 move = {cos(cam->yaw + 3.14f * 0.5f) * strifeSpeed, 0.0f, sin(cam->yaw + 3.14f * 0.5f) * strifeSpeed};
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

void setPitchYaw(camera* cam){
    float dx = cam->center[0] - cam->eye[0];
    float dy = cam->center[1] - cam->eye[1];
    float dz = cam->center[2] - cam->eye[2];

    float radius = sqrtf(dx*dx + dy*dy + dz*dz);

    if (radius < 1e-6f) {
        cam->pitch = 0.0f;
        cam->yaw = 0.0f;
        return;
    }

    cam->pitch = acosf(dy / radius);  
    cam->yaw   = atan2f(dz, dx);   
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos){

    camera* cam = glfwGetWindowUserPointer(window);

    printf("%f, %f\n", cam->pitch, cam->yaw);

    if(cam->firstMouse){

        setPitchYaw(cam);
        cam->firstMouse = 0;
        cam->lastX = xpos;
        cam->lastY = ypos;
        return;
    }

    float dxpos = xpos - cam->lastX;
    float dypos = ypos - cam->lastY;

    cam->lastX = xpos;
    cam->lastY = ypos;

    float radius = glm_vec3_distance(cam->center, cam->eye);

    cam->yaw += dxpos/1000;
    cam->pitch += dypos/1000;

    if (cam->pitch > 3.1415f){ cam->pitch = 3.1415f;}
    if (cam->pitch < 0.0f){ cam->pitch = 0.00001f;}

    cam->center[0] = radius * sin(cam->pitch) * cos(cam->yaw) + cam->eye[0];
    cam->center[2] = radius * sin(cam->pitch) * sin(cam->yaw) + cam->eye[2];
    cam->center[1] = radius * cos(cam->pitch) + cam->eye[1];


}
