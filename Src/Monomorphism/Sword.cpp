/*================================================================
Filename: Sword.cpp
Date: 2017.11.3
Created by AirGuanZ
================================================================*/
#include <limits>

#include <glm\gtc\matrix_transform.hpp>

#include "Include\Actor.h"
#include "Include\Sword.h"

using namespace std;
using namespace glm;
using namespace OWE;

//Sword的使用冷却时间
namespace
{
    constexpr double SWORD_BUSY_TIME = 800.0;
    constexpr float SWORD_LASY_DELTA_NONVAL = numeric_limits<float>::quiet_NaN();
    const std::string vtxSrc =
        R"__(#version 430 core
        uniform mat3 posTrans;
        in vec2 pos;
        out vec2 texCoord;
        void main(void)
        {
            gl_Position = vec4((posTrans * vec3(pos, 1.0)).xy, 0.0, 1.0);
            texCoord = pos;
        }
        )__";
    const std::string fragSrc =
        R"__(#version 430 core
        uniform sampler2D tex;
        in vec2 texCoord;
        void main(void)
        {
	        gl_FragColor = texture(tex, texCoord);
        }
        )__";

    VertexBuffer<vec2> vtxBuf;

    Shader shader;
    Shader::UniformMgrPtr uniforms;
    Shader::AttribMgrPtr attribs;

    UniformVariable<mat3> uniformPosTrans;
    UniformVariable<Texture2DView> uniformTex;

    void _InitializeSwordShader(void)
    {
        if(shader.IsAvailable())
            return;

        //初始化顶点缓存
        vec2 vtxBufData[6];
        Utility::GenBoxVertices(vec2(0.0f), vec2(1.0f), vtxBufData);
        vtxBuf.Initialize(6, vtxBufData);

        //初始化渲染shader
        string err;
        if(shader.Initialize(err, VSSrc(vtxSrc), FSSrc(fragSrc)) != Shader::InitState::Success)
            throw FatalError("Failed to initialize shader for sword: " + err);
        uniforms = shader.GetUniformMgrPtr();
        attribs = shader.GetAttribMgrPtr();

        uniformPosTrans = uniforms->GetUniform<mat3>("posTrans");
        uniformTex = uniforms->GetUniform<Texture2DView>("tex");

        attribs->GetAttrib<vec2>("pos").SetBuffer(vtxBuf);
    }
}

Sword::Sword(const std::string &texFilename, const glm::vec2 &texSize)
{
    _InitializeSwordShader();

    if(!LoadTexture2DFromFile(texFilename, Texture2D::Desc(), tex_))
        throw FatalError("Failed to load texture from file: " + texFilename);

    texSize_ = texSize;
    busy_ = -1.0f;

    lastDelta_ = SWORD_LASY_DELTA_NONVAL;
    lastPos_ = vec2(0.0f);
}

Sword::~Sword(void)
{
    //do nothing
}

void Sword::Restart(void)
{
    busy_ = SWORD_BUSY_TIME;
    lastDelta_ = SWORD_LASY_DELTA_NONVAL;
    lastPos_ = vec2(0.0f);
}

void Sword::Update(double time)
{
    if(busy_ > 0.0f)
        busy_ -= time;
}

void Sword::Draw(const Actor &actor, const ScreenScale &scale)
{
    const Actor::WeaponTrans &trans = actor.GetWeaponTrans();
    mat3 posTrans = Transform::Translate(trans.pos) *
        ;
}

std::vector<OWE::BoundingArea> Sword::GetDamageAreas(void)
{
    return { };
}

bool Sword::Busy(void) const
{
    return busy_ > 0.0;
}
