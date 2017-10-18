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

__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(_TiledTextureAux)

namespace
{
    Shader tiledTexShader_Basic;

#include "Include\TiledTextureShaderSource.inl"

    inline void _InitTiledTexture(void)
    {
        if(tiledTexShader_Basic.IsAvailable())
            return;
        std::string err;
        if(tiledTexShader_Basic.Initialize(
                err, VSSrc(tiledTexVtxShaderSrc_Basic), FSSrc(tiledTexFragShaderSrc_Basic))
            != Shader::InitState::Success)
            throw std::runtime_error("Failed to initialize shader for tiled texture");
    }

    inline void _DrawTile(float xLB, float yLB, float width, float height, const _Tile &tile)
    {

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
    assert(IsAvailable());
    int tileXBegin = static_cast<int>(glm::floor(LB.x * reTileWidth_));
    int tileYBegin = static_cast<int>(glm::floor(LB.y * reTileHeight_));
    int tileXEnd   = static_cast<int>(glm::ceil((LB.x + scale.ScreenWidth()) * reTileWidth_));
    int tileYEnd   = static_cast<int>(glm::ceil((LB.y + scale.ScreenHeight()) * reTileHeight_));

    tileXBegin = glm::max(tileXBegin, 0);
    tileYBegin = glm::max(tileYBegin, 0);
    tileXEnd   = glm::min(tileXEnd, width_);
    tileYEnd   = glm::min(tileYEnd, height_);

    for(int y = tileYBegin; y < tileYEnd; ++y)
    {
        for(int x = tileXBegin; x < tileXEnd; ++x)
            _DrawTile(x * tileWidth_, y * tileHeight_,
                      tileWidth_, tileHeight_,
                      tiles_[_TileIndex(height_, x, y)]);
    }
}

__OWE_END_NAMESPACE__(_TiledTextureAux)
__OWE_END_NAMESPACE__(OWE)
