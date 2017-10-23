/*================================================================
Filename: TiledTexture.cpp
Date: 2017.10.17
Created by AirGuanZ
================================================================*/
#include <cassert>
#include <stdexcept>
#include <string>

#include "Include\ImmediateRenderer.h"
#include "Include\Shader.h"
#include "Include\TiledTexture.h"
#include "Include\Transform2D.h"
#include "Include\VerticesGen.h"

__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(_TiledTextureAux)

namespace
{
    inline void _DrawTileWithBasicMode(float xLB, float yLB, float width, float height,
                                       const _Tile &tile, const ScreenScale &scale)
    {
        ImmediateRenderer::DrawTexturedBox(
            glm::vec2(xLB, yLB), glm::vec2(xLB, yLB) + glm::vec2(width, height),
            tile.uvLB, tile.uvRT, tile.tex, scale);
    }

    inline void _DrawTileWithAlphaTestMode(float xLB, float yLB, float width, float height,
                                           const _Tile &tile, const ScreenScale &scale, float minAlpha)
    {
        ImmediateRenderer::DrawTexturedBox(
            glm::vec2(xLB, yLB), glm::vec2(xLB, yLB) + glm::vec2(width, height),
            tile.uvLB, tile.uvRT, tile.tex, scale,
            ImmediateRenderer::RenderMode::AlphaTest,
            ImmediateRenderer::RenderDesc{ minAlpha });
    }
}

void _TiledTexture::Initialize(int width, int height, float tileWidth, float tileHeight)
{
    assert(width > 0 && height > 0 && tileWidth > 0.0f && tileHeight > 0.0f);

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
            _DrawTileWithAlphaTestMode(xLB, yLB, tileWidth_, tileHeight_,
                                       tiles_[yIdxBase + x], scale, alphaThreshold_);
    }
}

__OWE_END_NAMESPACE__(_TiledTextureAux)
__OWE_END_NAMESPACE__(OWE)
