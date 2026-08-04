#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>

typedef struct Texture {
  unsigned int id;
  int width;
  int height;
  int channels;
} Texture;

Texture loadTexture(const char *path);
void bindTexture(Texture *tex, unsigned int unit);
void destroyTexture(Texture *tex);

#endif