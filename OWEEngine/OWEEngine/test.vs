#version 430 core

uniform vec3 offset;
in vec4 position;

void main(void)
{
    gl_Position = position + vec4(offset, 0.0);
}