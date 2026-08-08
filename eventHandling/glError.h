#ifndef GL_ERROR_H
#define GL_ERROR_H

#include <GL/glew.h>

void glClearError();
void glCheckError(const char *file, int line);

#define GL_CALL(x)                                                             \
  glClearError();                                                              \
  x;                                                                           \
  glCheckError(__FILE__, __LINE__);

#endif