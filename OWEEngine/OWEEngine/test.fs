#version 430 core

uniform vec4 color_;
out vec4 color;

void main(void)
{
    color = color_;
}