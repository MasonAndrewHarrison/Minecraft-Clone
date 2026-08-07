#include <stdio.h>
#include <GL/glew.h>


#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG  
#include "vendor/stb/stb_image.h"
#include "texture.h"

    

Texture loadTexture(const char* path) {

    Texture tex = {0};

    stbi_set_flip_vertically_on_load(1); 
    unsigned char* data = stbi_load(path, &tex.width, &tex.height, &tex.channels, 0);
    if (!data) {
        printf("failed to load texture: %s\n", path);
        return tex;
    }

    GLenum format = (tex.channels == 4) ? GL_RGBA : GL_RGB;

    glGenTextures(1, &tex.id);
    glBindTexture(GL_TEXTURE_2D, tex.id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, format, tex.width, tex.height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return tex;
}

void bindTexture(Texture* tex, unsigned int unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, tex->id);
}

void destroyTexture(Texture* tex) {
    glDeleteTextures(1, &tex->id);
}