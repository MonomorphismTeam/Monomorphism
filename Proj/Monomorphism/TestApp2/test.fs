#version 430 core

varying vec3 colorF;

void main(void)
{
    gl_FragColor = vec4(colorF, 1.0);
}