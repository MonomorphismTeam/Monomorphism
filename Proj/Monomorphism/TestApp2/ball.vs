#version 430 core

in vec2 position;
in vec2 uv;

uniform mat3 modelMat;
uniform mat3 projMat;

varying vec2 texCoord;

void main(void)
{
    //vec3 pos = projMat * modelMat * vec3(position, 1.0f);
    //gl_Position = vec4(pos.xy, 0.0f, 1.0f);
    gl_Position = vec4(position / 20.0f, 0.0f, 1.0f);
    texCoord = uv;
}