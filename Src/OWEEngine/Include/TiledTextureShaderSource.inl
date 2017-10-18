/*================================================================
Filename: TiledTextureShaderSource.inl
Date: 2017.10.17
Created by AirGuanZ
================================================================*/

const std::string tiledTexVtxShaderSrc_Basic =
R"___(
#version 430 core

in vec2 inPos;
in vec2 inTexCoord;

uniform mat3 transMat;

varying vec2 texCoord;

void main(void)
{
    gl_Position = vec4((transMat * vec3(inPos, 1.0)).xy, 0.0, 1.0);
	texCoord = inTexCoord;
}
)___";

const std::string tiledTexFragShaderSrc_Basic =
R"___(
#version 430 core

in vec2 texCoord;

uniform sampler2D tex;

void main(void)
{
	gl_FragColor = texture(tex, texCoord);
}
)___";
