#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include "camera.h"
#include <math.h> 
#include "mesh.h"

#define mouseSpeed 0.001f

camera* cameraInit(int width, int height){

    camera* cam = malloc(sizeof(camera));
    glm_vec3_copy((vec3){0.0f, 0.0f, 0.0f}, cam->eye);
    glm_vec3_copy((vec3){3.0f, 0.0f, 0.0f}, cam->center);
    glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, cam->up);
    cam->yaw = 0.0f;
    cam->pitch = 1.570796f;
    cam->lastX = width/2.0f;
    cam->lastY = height/2.0f;
    cam->firstMouse = 1;

    return cam;
}

void teleportCamera(camera* cam, vec3 position){
    glm_vec3_copy(position, cam->eye);
    glm_vec3_copy(position, cam->center);
    glm_vec3_add(cam->eye, (vec3){0.0f, 0.0f, 1.0f}, cam->eye);
    correctsPitchYaw(cam);
}

void moveCamera(vec3 move, camera* cam){
    glm_vec3_add(cam->eye, move, cam->eye);
    glm_vec3_add(cam->center, move, cam->center);
}

void moveLeftRight(camera* cam, float forwardSpeed){
    vec3 move =  {cos(cam->yaw) * forwardSpeed, 0.0f, sin(cam->yaw) * forwardSpeed};
    moveCamera(move, cam);
}

void moveForwardBackwards(camera* cam, float strifeSpeed){
    vec3 move = {-cos(cam->yaw + 3.14f * 0.5f) * strifeSpeed, 0.0f, -sin(cam->yaw + 3.14f * 0.5f) * strifeSpeed};
    moveCamera(move, cam);
}

void moveUpDown(camera* cam, float upSpeed){
    vec3 move = {0.0f, upSpeed, -0.0f};
    moveCamera(move, cam);
}

void correctsPitchYaw(camera* cam){
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

void changeAngle(camera* cam, float pitch, float yaw){

    float radius = glm_vec3_distance(cam->center, cam->eye);

    cam->yaw = yaw;
    cam->pitch = pitch;

    if (cam->pitch > 3.1415f){ cam->pitch = 3.1415f;}
    if (cam->pitch < 0.0f){ cam->pitch = 0.00001f;}

    cam->center[0] = radius * sin(cam->pitch) * cos(cam->yaw) + cam->eye[0];
    cam->center[2] = radius * sin(cam->pitch) * sin(cam->yaw) + cam->eye[2];
    cam->center[1] = radius * cos(cam->pitch) + cam->eye[1];  
}


bool movedToDifferentChunk(camera* cam, int* lastX, int* lastY, int* lastZ){

    int currentX = floorf(cam->eye[0] / CHUNK_SIZE);
    int currentY = floorf(cam->eye[2] / CHUNK_SIZE);
    int currentZ = floorf(cam->eye[1] / CHUNK_SIZE);

    *lastX = (int)floor((double)(*lastX) / CHUNK_SIZE);
    *lastY = (int)floor((double)(*lastY) / CHUNK_SIZE);
    *lastZ = (int)floor((double)(*lastZ) / CHUNK_SIZE);

    if (*lastX==currentX && *lastY==currentY && *lastZ==currentZ){
        return false;
    } else {return true;}

}