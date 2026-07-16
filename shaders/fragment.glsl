#version 330 core

layout(location = 0) out vec4 color;
in vec2 uvCoord;

uniform float uTime;
uniform float uAngle;
uniform vec2 uResolution;
uniform vec2 uStretch;

void main() {

    vec2 offset = uvCoord - vec2(0.5, 0.5);
    offset.x *= uResolution.x / uResolution.y; 

    float c = cos(uAngle);
    float s = sin(uAngle);

    offset = vec2(offset.x * c - offset.y * s, offset.x * s + offset.y * c);
    offset *= uStretch;

    float r = exp(-dot(offset * vec2(1.0, sin(uTime)+1.5), offset * vec2(1.0, 1.75)));
    float g = exp(-dot(offset * vec2(2.0, 1.0), offset * vec2(cos(uTime)*0.5 + 0.75, 1.0)));
    float b = exp(-dot(offset * vec2(1.5, 1.5), offset * vec2(1.5, 1.5)));


    float alpha = max(r, max(g, b));

    color = vec4(1, g, b, 1);
}
