#version 430 core

in vec4 position;
in vec2 uv_in;

uniform vec2 offset;

varying vec2 uv;

void main(void)
{
    uv = uv_in;
    gl_Position = position + vec4(offset, 0.0f, 0.0f);
}