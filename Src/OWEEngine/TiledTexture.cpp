/*================================================================
Filename: TiledTexture.cpp
Date: 2017.10.17
Created by AirGuanZ
================================================================*/
#include <cassert>
#include <stdexcept>
#include <string>

#include "Include\Shader.h"
#include "Include\TiledTexture.h"
#include "Include\Transform2D.h"

__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(_TiledTextureAux)

namespace
{
#include "Include\TiledTextureShaderSource.inl"

    //用于tiled texture绘制的顶点缓存
    VertexBuffer<glm::vec2> vtxPos;

    //基本绘制模式
    Shader shaderBasic;

    Shader::UniformMgrPtr uniformsBasic;
    Shader::AttribMgrPtr attribsBasic;

    UniformVariable<glm::mat3> vtxTransMat_Basic;
    UniformVariable<glm::mat3> texTransMat_Basic;
    UniformVariable<Texture2DBase> tex_Basic;

    //带alpha test的绘制模式
    Shader shaderAlphaTest;

    Shader::UniformMgrPtr uniformsAlphaTest;
    Shader::AttribMgrPtr attribsAlphaTest;

    UniformVariable<glm::mat3> vtxTransMatAlphaTest;
    UniformVariable<glm::mat3> texTransMatAlphaTest;
    UniformVariable<Texture2DBase> texAlphaTest;
    UniformVariable<GLfloat> minAlphaAlphaTest;

    void _InitTiledTexture(void)
    {
        if(!vtxPos.IsAvailable())
        {
            const glm::vec2 vtxPosData[] =
            {
                { 0.0f, 0.0f },
                { 0.0f, 1.0f },
                { 1.0f, 1.0f },
                { 0.0f, 0.0f },
                { 1.0f, 1.0f },
                { 1.0f, 0.0f }
            };
            vtxPos.Initialize(6, vtxPosData);
        }

        if(!shaderBasic.IsAvailable())
        {
            std::string err;
            if(shaderBasic.Initialize(err, VSSrc(tiledTexVtxShaderSrc_Basic),
                                           FSSrc(tiledTexFragShaderSrc_Basic))
                != Shader::InitState::Success)
                throw std::runtime_error("Failed to initialize shader for tiled texture");

            uniformsBasic = shaderBasic.GetUniformMgrPtr();
            attribsBasic = shaderBasic.GetAttribMgrPtr();

            attribsBasic->GetAttrib<glm::vec2>("inPos").SetBuffer(vtxPos);
            vtxTransMat_Basic = uniformsBasic->GetUniform<glm::mat3>("vtxTransMat");
            texTransMat_Basic = uniformsBasic->GetUniform<glm::mat3>("texTransMat");
            tex_Basic = uniformsBasic->GetUniform<Texture2DBase>("tex");
        }

        if(!shaderAlphaTest.IsAvailable())
        {
            std::string err;
            if(shaderAlphaTest.Initialize(err, VSSrc(tiledTexVtxShaderSrc_AlphaTest),
                                               FSSrc(tiledTexFragShaderSrc_AlphaTest))
                != Shader::InitState::Success)
                throw std::runtime_error("Failed to initialize shader for tiled texture");

            uniformsAlphaTest = shaderAlphaTest.GetUniformMgrPtr();
            attribsAlphaTest = shaderAlphaTest.GetAttribMgrPtr();

            attribsAlphaTest->GetAttrib<glm::vec2>("inPos").SetBuffer(vtxPos);
            vtxTransMatAlphaTest = uniformsAlphaTest->GetUniform<glm::mat3>("vtxTransMat");
            texTransMatAlphaTest = uniformsAlphaTest->GetUniform<glm::mat3>("texTransMat");
            texAlphaTest = uniformsAlphaTest->GetUniform<Texture2DBase>("tex");
            minAlphaAlphaTest = uniformsAlphaTest->GetUniform<GLfloat>("minAlpha");
        }
    }

    void _DrawTileWithBasicMode(float xLB, float yLB, float width, float height,
                                const _Tile &tile, const ScreenScale &scale)
    {
        glm::mat3 vtxTransMat = scale.ProjMatrix() *
                                Transform::Translate(glm::vec2(xLB, yLB)) *
                                Transform::Scale(glm::vec2(width, height));
        glm::mat3 texTransMat = Transform::Translate(tile.uvLB) *
                                Transform::Scale(tile.uvRT - tile.uvLB);

        shaderBasic.Bind();
        attribsBasic->Bind();

        vtxTransMat_Basic.SetAndApply(vtxTransMat);
        texTransMat_Basic.SetAndApply(texTransMat);
        tex_Basic.SetAndApply(tile.tex);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        attribsBasic->Unbind();
        shaderBasic.Unbind();
    }

    void _DrawTileWithBasicMode(float xLB, float yLB, float width, float height,
                                const _Tile &tile, const ScreenScale &scale, float minAlpha)
    {
        glm::mat3 vtxTransMat = scale.ProjMatrix() *
                                Transform::Translate(glm::vec2(xLB, yLB)) *
                                Transform::Scale(glm::vec2(width, height));
        glm::mat3 texTransMat = Transform::Translate(tile.uvLB) *
                                Transform::Scale(tile.uvRT - tile.uvLB);

        shaderAlphaTest.Bind();
        attribsAlphaTest->Bind();

        vtxTransMatAlphaTest.SetAndApply(vtxTransMat);
        texTransMatAlphaTest.SetAndApply(texTransMat);
        texAlphaTest.SetAndApply(tile.tex);
        minAlphaAlphaTest.SetAndApply(minAlpha);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        attribsAlphaTest->Unbind();
        shaderAlphaTest.Unbind();
    }
}

void _TiledTexture::Initialize(int width, int height, float tileWidth, float tileHeight)
{
    assert(width > 0 && height > 0 && tileWidth > 0.0f && tileHeight > 0.0f);

    _InitTiledTexture();
    Destroy();

    width_        = width;
    height_       = height;
    tileWidth_    = tileWidth;
    tileHeight_   = tileHeight;
    reTileWidth_  = 1.0f / tileWidth;
    reTileHeight_ = 1.0f / tileHeight;
    tiles_.resize(width * height, _Tile{ { 0.0f, 0.0f }, { 0.0f, 0.0f }, Texture2DView(0) });
}

void _TiledTexture::Destroy(void)
{
    if(IsAvailable())
    {
        width_ = height_ = 0;
        tileWidth_ = tileHeight_ = 0.0f;
        tiles_.clear();
    }
}

void _TiledTexture::Draw(const glm::vec2 &LB, const ScreenScale &scale) const
{
    using DrawFuncPtr = void (_TiledTexture::*)(const glm::vec2&, const ScreenScale&) const;
    static const DrawFuncPtr drawers[] =
    {
        &_TiledTexture::DrawWithBasicMode,
        &_TiledTexture::DrawWithAlphaTestMode
    };
    (this->*drawers[static_cast<int>(renderMode_)])(LB, scale);
}

void _TiledTexture::DrawWithBasicMode(const glm::vec2 &_LB, const ScreenScale &scale) const
{
    assert(IsAvailable());

    const glm::vec2 LB = _LB - LBpos_;
    int tileXBegin = static_cast<int>(glm::floor(LB.x * reTileWidth_));
    int tileYBegin = static_cast<int>(glm::floor(LB.y * reTileHeight_));
    int tileXEnd = static_cast<int>(glm::ceil((LB.x + scale.ScreenWidth()) * reTileWidth_));
    int tileYEnd = static_cast<int>(glm::ceil((LB.y + scale.ScreenHeight()) * reTileHeight_));

    tileXBegin = glm::max(tileXBegin, 0);
    tileYBegin = glm::max(tileYBegin, 0);
    tileXEnd = glm::min(tileXEnd, width_);
    tileYEnd = glm::min(tileYEnd, height_);

    float yLB = tileYBegin * tileHeight_ - LB.y;
    int yIdxBase = tileYBegin * width_;;
    for(int y = tileYBegin; y < tileYEnd; ++y, yLB += tileHeight_, yIdxBase += width_)
    {
        float xLB = tileXBegin * tileWidth_ - LB.x;
        for(int x = tileXBegin; x < tileXEnd; ++x, xLB += tileWidth_)
            _DrawTileWithBasicMode(xLB, yLB, tileWidth_, tileHeight_, tiles_[yIdxBase + x], scale);
    }
}

void _TiledTexture::DrawWithAlphaTestMode(const glm::vec2 &_LB, const ScreenScale &scale) const
{
    assert(IsAvailable());

    const glm::vec2 LB = _LB - LBpos_;
    int tileXBegin = static_cast<int>(glm::floor(LB.x * reTileWidth_));
    int tileYBegin = static_cast<int>(glm::floor(LB.y * reTileHeight_));
    int tileXEnd = static_cast<int>(glm::ceil((LB.x + scale.ScreenWidth()) * reTileWidth_));
    int tileYEnd = static_cast<int>(glm::ceil((LB.y + scale.ScreenHeight()) * reTileHeight_));

    tileXBegin = glm::max(tileXBegin, 0);
    tileYBegin = glm::max(tileYBegin, 0);
    tileXEnd = glm::min(tileXEnd, width_);
    tileYEnd = glm::min(tileYEnd, height_);

    float yLB = tileYBegin * tileHeight_ - LB.y;
    int yIdxBase = tileYBegin * width_;;
    for(int y = tileYBegin; y < tileYEnd; ++y, yLB += tileHeight_, yIdxBase += width_)
    {
        float xLB = tileXBegin * tileWidth_ - LB.x;
        for(int x = tileXBegin; x < tileXEnd; ++x, xLB += tileWidth_)
            _DrawTileWithBasicMode(xLB, yLB, tileWidth_, tileHeight_,
                                   tiles_[yIdxBase + x], scale, alphaThreshold_);
    }
}

__OWE_END_NAMESPACE__(_TiledTextureAux)
__OWE_END_NAMESPACE__(OWE)
