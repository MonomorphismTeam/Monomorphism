#version 430 core

uniform sampler2D basic;
uniform sampler2D light;

in vec2 texCoord;

void main(void)
{
    gl_FragColor = texture(basic, texCoord) *
                   texture(light, texCoord);
}
