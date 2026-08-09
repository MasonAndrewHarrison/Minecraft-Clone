#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

typedef struct camera {
  vec3 eye;
  vec3 center;
  vec3 up;
  float yaw;
  float pitch;
  float lastX;
  float lastY;
  int firstMouse;
} camera;

camera* cameraInit(int height, int width);
void moveLeftRight(camera *cam, float forwardSpeed);
void moveForwardBackwards(camera *cam, float strifeSpeed);
void moveUpDown(camera *cam, float upSpeed);
void moveCamera(vec3 move, camera *cam);
void updateCameraLocation(GLFWwindow *window, camera *cam);
void changeAngle(camera *cam, float pitch, float yaw);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);

#endif