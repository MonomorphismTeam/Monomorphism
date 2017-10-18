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

    inline void _InitTiledTextureShader(void)
    {
        if(tiledTexShader_Basic.IsAvailable())
            return;

        std::string err;
        if(tiledTexShader_Basic.Initialize(
                err, VSSrc(tiledTexVtxShaderSrc_Basic), FSSrc(tiledTexFragShaderSrc_Basic))
            != Shader::InitState::Success)
            throw std::runtime_error("Failed to initialize shader for tiled texture");
    }

    inline int _TileIndex(int height, int tileX, int tileY)
    {
        return tileY * height + tileX;
    }
}

_TiledTexture::_TiledTexture(void)
    : width_(0), height_(0),
      tileWidth_(0.0f), tileHeight_(0.0f)
{

}

_TiledTexture::~_TiledTexture(void)
{

}

void _TiledTexture::Initialize(int width, int height, float tileWidth, float tileHeight)
{
    assert(width > 0 && height > 0 && tileWidth > 0.0f && tileHeight > 0.0f);

    _InitTiledTextureShader();
    Destroy();

    width_      = width;
    height_     = height;
    tileWidth_  = tileWidth;
    tileHeight_ = tileHeight;
    tiles_.resize(width * height, Tile{ { 0.0f, 0.0f }, { 0.0f, 0.0f }, Texture2DView(0) });
}

bool _TiledTexture::IsAvailable(void) const
{
    return width_ != 0;
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

void _TiledTexture::SetTile(int tileX, int tileY, const glm::vec2 &uvLB, const glm::vec2 &uvRT, Texture2DView tex)
{
    assert(IsAvailable());
    assert(0 <= tileX && tileX < width_);
    assert(0 <= tileY && tileY < height_);
    tiles_[_TileIndex(height_, tileX, tileY)] = Tile{ uvLB, uvRT, tex };
}

Texture2DView _TiledTexture::GetTileTex(int tileX, int tileY) const
{
    assert(IsAvailable());
    assert(0 <= tileX && tileX < width_);
    assert(0 <= tileY && tileY < height_);
    return tiles_[_TileIndex(height_, tileX, tileY)].tex;
}

glm::vec2 _TiledTexture::GetTileTexCoordLB(int tileX, int tileY) const
{
    assert(IsAvailable());
    assert(0 <= tileX && tileX < width_);
    assert(0 <= tileY && tileY < height_);
    return tiles_[_TileIndex(height_, tileX, tileY)].uvLB;
}

glm::vec2 _TiledTexture::GetTileTexCoordRT(int tileX, int tileY) const
{
    assert(IsAvailable());
    assert(0 <= tileX && tileX < width_);
    assert(0 <= tileY && tileY < height_);
    return tiles_[_TileIndex(height_, tileX, tileY)].uvRT;
}

int _TiledTexture::Width(void) const
{
    assert(IsAvailable());
    return width_;
}

int _TiledTexture::Height(void) const
{
    assert(IsAvailable());
    return height_;
}

float _TiledTexture::TileWidth(void) const
{
    assert(IsAvailable());
    return tileWidth_;
}

float _TiledTexture::TileHeight(void) const
{
    assert(IsAvailable());
    return tileHeight_;
}

void _TiledTexture::Draw(const glm::vec2 &LB, const ScreenScale &scale) const
{
    assert(IsAvailable());

}

__OWE_END_NAMESPACE__(_TiledTextureAux)
__OWE_END_NAMESPACE__(OWE)
