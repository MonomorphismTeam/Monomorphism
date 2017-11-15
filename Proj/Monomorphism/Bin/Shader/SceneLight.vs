#version 430 core

in vec2 pos;
in vec2 uv;

out vec2 texCoord;

void main(void)
{
    gl_Position = vec4(pos, 0.0, 1.0);
    texCoord = uv;
}
