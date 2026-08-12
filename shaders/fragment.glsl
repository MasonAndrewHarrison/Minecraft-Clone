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

    float near = 0.1; 
    float far = 500.0; 

    float ndc = gl_FragCoord.z * 2.0 - 1.0;
    float linearDepth = (2.0 * near * far) / (far + near - ndc * (far - near));

    vec3 skyColor = vec3(0.3f, 0.6f, 0.9f);
    float depth = (linearDepth / far);

    vec3 rgbColor = mix((texColor.rgb * brightness), skyColor, depth);

    color = vec4( rgbColor, texColor.a);

}