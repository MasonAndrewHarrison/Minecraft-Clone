#version 330 core

layout(location = 0) out vec4 color;
in vec3 vColor;


in vec2 uvCoord;

uniform sampler2D uTexture;


void main() {
    color = texture(uTexture, uvCoord);
    //color = vec4(1.0, 1.0, 0.0, 1.0);
}