#version 430 core

uniform sampler2D tex;

varying vec2 uv;

void main(void)
{
    gl_FragColor = texture2D(tex, uv);
}