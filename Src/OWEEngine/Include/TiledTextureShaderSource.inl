/*================================================================
Filename: TiledTextureShaderSource.inl
Date: 2017.10.17
Created by AirGuanZ
================================================================*/

const std::string tiledTexVtxShaderSrc_Basic =
R"___(
#version 430 core

in vec2 inPos;

uniform mat3 vtxTransMat;
uniform mat3 texTransMat;

varying vec2 texCoord;

void main(void)
{
    gl_Position = vec4((transMat * vec3(inPos, 1.0)).xy, 0.0, 1.0);
	texCoord = (texTransMat * vec3(inPos, 1.0)).xy;
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
