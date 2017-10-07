#version 430 core

in vec4 position;
in vec2 uv_in;

varying vec2 uv;

void main(void)
{
    uv = uv_in;
    gl_Position = position;
}