/*================================================================
Filename: ImmediateRenderer.cpp
Date: 2017.10.22
Created by AirGuanZ
================================================================*/
#include <string>

#include "Include\FatalError.h"
#include "Include\ImmediateRenderer.h"
#include "Include\Shader.h"
#include "Include\Transform2D.h"
#include "Include\VerticesGen.h"
#include "Include\VertexBuffer.h"

__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(_ImmediateRendererAux)

namespace
{
    //(0, 0) - (1, 1)的顶点数据
    //同时用于顶点位置和纹理坐标
    VertexBuffer<glm::vec2> vtxBuf;

    //基本渲染模式
    Shader shaderBasic;
    Shader::AttribMgrPtr attribsBasic;
    Shader::UniformMgrPtr uniformsBasic;
    UniformVariable<glm::mat3> posTransBasic;
    UniformVariable<glm::mat3> texTransBasic;
    UniformVariable<Texture2DView> texBasic;

    //带alpha test的渲染
    Shader shaderAlphaTest;
    Shader::AttribMgrPtr attribsAlphaTest;
    Shader::UniformMgrPtr uniformsAlphaTest;
    UniformVariable<glm::mat3> posTransAlphaTest;
    UniformVariable<glm::mat3> texTransAlphaTest;
    UniformVariable<Texture2DView> texAlphaTest;
    UniformVariable<GLfloat> minAlphaAlphaTest;

    void _InitVtxBuf(void)
    {
        assert(!vtxBuf.IsAvailable());
        glm::vec2 data[6];
        Utility::GenBoxVertices(glm::vec2(0.0f), glm::vec2(1.0f), data);
        vtxBuf.Initialize(6, data);
    }

    void _InitShaderBasic(void)
    {
        assert(!shaderBasic.IsAvailable() && vtxBuf.IsAvailable());
        const std::string vtxSrc =
            R"__(#version 430 core
            uniform mat3 posTrans;
            uniform mat3 texTrans;
            in vec2 pos;
            out vec2 texCoord;
            void main(void)
            {
                gl_Position = vec4((posTrans * vec3(pos, 1.0)).xy, 0.0, 1.0);
                texCoord = (texTrans * vec3(pos, 1.0)).xy;
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
        std::string err;
        if(shaderBasic.Initialize(err, VSSrc(vtxSrc), FSSrc(fragSrc))
            != Shader::InitState::Success)
            throw FatalError("Failed to initialize basic shader for immediate renderer" + err);
        
        attribsBasic = shaderBasic.GetAttribMgrPtr();
        uniformsBasic = shaderBasic.GetUniformMgrPtr();

        attribsBasic->GetAttrib<glm::vec2>("pos").SetBuffer(vtxBuf);
        posTransBasic = uniformsBasic->GetUniform<glm::mat3>("posTrans");
        texTransBasic = uniformsBasic->GetUniform<glm::mat3>("texTrans");
        texBasic = uniformsBasic->GetUniform<Texture2DView>("tex");
    }

    void _InitShaderAlphaTest(void)
    {
        assert(!shaderAlphaTest.IsAvailable() && vtxBuf.IsAvailable());
        const std::string vtxSrc =
            R"__(#version 430 core
            uniform mat3 posTrans;
            uniform mat3 texTrans;
            in vec2 pos;
            out vec2 texCoord;
            void main(void)
            {
                gl_Position = vec4((posTrans * vec3(pos, 1.0)).xy, 0.0, 1.0);
                texCoord = (texTrans * vec3(pos, 1.0)).xy;
            }
            )__";
        const std::string fragSrc =
            R"__(#version 430 core
            uniform sampler2D tex;
            uniform float minAlpha;
            in vec2 texCoord;
            void main(void)
            {
                vec4 c = texture(tex, texCoord);
                if(c.a < minAlpha)
                    discard;
	            gl_FragColor = c;
            }
            )__";
        std::string err;
        if(shaderAlphaTest.Initialize(err, VSSrc(vtxSrc), FSSrc(fragSrc))
            != Shader::InitState::Success)
            throw FatalError("Failed to initialize alpha-tested shader for immediate renderer" + err);

        attribsAlphaTest = shaderAlphaTest.GetAttribMgrPtr();
        uniformsAlphaTest = shaderAlphaTest.GetUniformMgrPtr();

        attribsAlphaTest->GetAttrib<glm::vec2>("pos").SetBuffer(vtxBuf);
        posTransAlphaTest = uniformsAlphaTest->GetUniform<glm::mat3>("posTrans");
        texTransAlphaTest = uniformsAlphaTest->GetUniform<glm::mat3>("texTrans");
        texAlphaTest = uniformsAlphaTest->GetUniform<Texture2DView>("tex");
        minAlphaAlphaTest = uniformsAlphaTest->GetUniform<GLfloat>("minAlpha");
    }
}

void _ImmediateRenderer::Initialize(void)
{
    _InitVtxBuf();
    _InitShaderBasic();
    _InitShaderAlphaTest();
}

bool _ImmediateRenderer::IsAvailable(void)
{
    return vtxBuf.IsAvailable();
}

void _ImmediateRenderer::Destroy(void)
{
    vtxBuf.Destroy();

    shaderBasic.Destroy();
    attribsBasic.reset();
    uniformsBasic.reset();

    shaderAlphaTest.Destroy();
    attribsAlphaTest.reset();
    uniformsAlphaTest.reset();
}

void _ImmediateRenderer::DrawTexturedBox(
    const glm::vec2 &LB, const glm::vec2 &RT,
    const glm::vec2 &uvLB, const glm::vec2 &uvRT,
    const Texture2DView tex, const ScreenScale &scale,
    RenderMode mode, const RenderDesc &desc)
{
    assert(IsAvailable());
    glm::mat3 posTransMat = scale.ProjMatrix() *
                            Transform::Translate(LB) *
                            Transform::Scale(RT - LB);
    glm::mat3 texTransMat = Transform::Translate(uvLB) *
                            Transform::Scale(uvRT - uvLB);
    switch(mode)
    {
    case RenderMode::Basic:
    {
        shaderBasic.Bind();
        attribsBasic->Bind();

        posTransBasic.SetAndApply(posTransMat);
        texTransBasic.SetAndApply(texTransMat);
        texBasic.SetAndApply(tex);

        RenderContext::GetInstance().DrawTriangles(6);

        attribsBasic->Unbind();
        shaderBasic.Unbind();
        break;
    }
    case RenderMode::AlphaTest:
    {
        shaderAlphaTest.Bind();
        attribsAlphaTest->Bind();

        posTransBasic.SetAndApply(posTransMat);
        texTransBasic.SetAndApply(texTransMat);
        texBasic.SetAndApply(tex);
        minAlphaAlphaTest.SetAndApply(desc.alphaThreshold);

        RenderContext::GetInstance().DrawTriangles(6);

        attribsAlphaTest->Unbind();
        shaderAlphaTest.Unbind();
        break;
    }
    default:
        abort();
    }
}

void _ImmediateRenderer::DrawTexturedBoxWithScreenTrans(
    const glm::vec2 &LB, const glm::vec2 &RT,
    const glm::vec2 &uvLB, const glm::vec2 &uvRT,
    const Texture2DView tex, const ScreenScale &scale,
    RenderMode mode, const RenderDesc &desc)
{
    assert(IsAvailable());
    glm::mat3 posTransMat = scale.ProjMatrix() *
                            scale.TransMatrix() *
                            Transform::Translate(LB) *
                            Transform::Scale(RT - LB);
    glm::mat3 texTransMat = Transform::Translate(uvLB) *
                            Transform::Scale(uvRT - uvLB);
    switch(mode)
    {
    case RenderMode::Basic:
    {
        shaderBasic.Bind();
        attribsBasic->Bind();

        posTransBasic.SetAndApply(posTransMat);
        texTransBasic.SetAndApply(texTransMat);
        texBasic.SetAndApply(tex);

        RenderContext::GetInstance().DrawTriangles(6);

        attribsBasic->Unbind();
        shaderBasic.Unbind();
        break;
    }
    case RenderMode::AlphaTest:
    {
        shaderAlphaTest.Bind();
        attribsAlphaTest->Bind();

        posTransBasic.SetAndApply(posTransMat);
        texTransBasic.SetAndApply(texTransMat);
        texBasic.SetAndApply(tex);
        minAlphaAlphaTest.SetAndApply(desc.alphaThreshold);

        RenderContext::GetInstance().DrawTriangles(6);

        attribsAlphaTest->Unbind();
        shaderAlphaTest.Unbind();
        break;
    }
    default:
        abort();
    }
}

__OWE_END_NAMESPACE__(_ImmediateRendererAux)
__OWE_END_NAMESPACE__(OWE)
