#version 430 core

uniform sampler2D tex;

varying vec2 texCoord;

void main(void)
{
    vec4 tx = texture2D(tex, texCoord);
    //if(tx.a < 0.1)
    //    discard;
    gl_FragColor = tx;
    //gl_FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}