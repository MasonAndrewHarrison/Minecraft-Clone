#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 aColor;
out vec3 vColor;
out vec2 uvCoord;

uniform mat4 uMVP;

void main() {
    gl_Position = uMVP * vec4(position, 1.0);
    uvCoord = position.xy * 0.5 + 0.5;
    vColor = aColor;
}
