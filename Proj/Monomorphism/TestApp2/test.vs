#version 430 core

in vec2 pos;
in vec3 color;

uniform mat3 transMat;

varying vec3 colorF;

void main(void)
{
    vec3 pos_ = transMat * vec3(pos, 1.0);
    gl_Position = vec4(pos_.xy, 0.0, 1.0);
    colorF = color;
}