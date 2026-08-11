#version 330 core

layout(location = 0) out vec4 color;
in vec3 vNormal;
in vec2 uvCoord;

uniform sampler2D uTexture;


void main(){
    vec4 texColor = texture(uTexture, uvCoord);

    float brightness;
    if (vNormal.y > 0.5)       brightness = 1.0;  
    else if (vNormal.y < -0.5) brightness = 1.0;   
    else if (vNormal.x < -0.5) brightness = 0.95;   
    else                       brightness = 0.75;

    color = vec4(texColor.rgb * brightness, texColor.a);
}