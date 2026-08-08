#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>


#define MAX_SHADER_ERROR 1024

char* readFile(const char* path) {

    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        printf("error: could not open file %s\n", path);
        return NULL;
    }


    fseek(file, 0, SEEK_END);   
    long length = ftell(file);  
    rewind(file);          

    char* buffer = malloc(length + 1);
    if (buffer == NULL) {
        printf("error: malloc failed for file %s\n", path);
        fclose(file);
        return NULL;
    }


    fread(buffer, 1, length, file);
    buffer[length] = '\0';          

    fclose(file);
    return buffer;
}

unsigned int compileShader(unsigned int type, const char* source){
    
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE){
        char message[MAX_SHADER_ERROR];
        glGetShaderInfoLog(id, MAX_SHADER_ERROR, NULL, message);
        printf("%s", message);
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int createShader(const char* vertexDirectory, const char* fragmentDirectory) {

    char* vertexShader = readFile(vertexDirectory);
    char* fragmentShader = readFile(fragmentDirectory);

    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    free(vertexShader);
    free(fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}