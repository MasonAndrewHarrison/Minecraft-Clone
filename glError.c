#include "glError.h"
#include <stdio.h>

void glClearError() {
    while (glGetError() != GL_NO_ERROR);
}

void glCheckError(const char* file, int line) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        switch(error) {
            case GL_INVALID_ENUM:
                printf("[%s:%d] GL_INVALID_ENUM\n", file, line);
                break;
            case GL_INVALID_VALUE:
                printf("[%s:%d] GL_INVALID_VALUE\n", file, line);
                break;
            case GL_INVALID_OPERATION:
                printf("[%s:%d] GL_INVALID_OPERATION\n", file, line);
                break;
            case GL_OUT_OF_MEMORY:
                printf("[%s:%d] GL_OUT_OF_MEMORY\n", file, line);
                break;
            default:
                printf("[%s:%d] Error: %u\n", file, line, error);
                break;
        }
    }
}