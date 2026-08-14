#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
out vec3 vNormal;
out vec2 uvCoord;

uniform mat4 uMVP;


void main() {
    gl_Position = uMVP * vec4(position, 1.0);
    vNormal = aNormal;
    uvCoord = aTexCoord/16;


}
