/*================================================================
Filename: TiledTexture.h
Date: 2017.10.17
Created by AirGuanZ
================================================================*/
#ifndef __OWE_TILED_TEXTURE_H__
#define __OWE_TILED_TEXTURE_H__

#include <cassert>
#include <vector>

#include "Common.h"
#include "ScreenScale.h"
#include "Texture2DView.h"

__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(_TiledTextureAux)

enum class _TiledTextureRenderMode
{
    Basic = 0,
    AlphaTest = 1
};

struct _Tile
{
    glm::vec2 uvLB;
    glm::vec2 uvRT;
    Texture2DView tex;
};

class _TiledTexture
{
public:
    using RenderMode = _TiledTextureRenderMode;

    _TiledTexture(void);
    ~_TiledTexture(void);

    void Initialize(int width, int height, float tileWidth, float tileHeight);
    bool IsAvailable(void) const;
    void Destroy(void);

    void SetRenderMode(RenderMode mode);

    //设置WithAlphaTest绘制模式中的alpha分量阈值
    void SetAlphaThreshold(float alpha);

    void SetTile(int tileX, int tileY, const glm::vec2 &uvLB, const glm::vec2 &uvRT, Texture2DView tex);
    Texture2DView GetTileTex(int tileX, int tileY) const;
    glm::vec2 GetTileTexCoordLB(int tileX, int tileY) const;
    glm::vec2 GetTileTexCoordRT(int tileX, int tileY) const;

    int Width(void) const;
    int Height(void) const;
    float TileWidth(void) const;
    float TileHeight(void) const;

    void Draw(const glm::vec2 &LB, const ScreenScale &scale) const;

private:
    void DrawWithBasicMode(const glm::vec2 &LB, const ScreenScale &scale) const;
    void DrawWithAlphaTestMode(const glm::vec2 &LB, const ScreenScale &scale) const;

    int width_;
    int height_;
    float tileWidth_;
    float tileHeight_;
    float reTileWidth_;
    float reTileHeight_;

    std::vector<_Tile> tiles_;

    RenderMode renderMode_;
    float alphaThreshold_;
};

__OWE_END_NAMESPACE__(_TiledTextureAux)

using TiledTexture = _TiledTextureAux::_TiledTexture;

__OWE_END_NAMESPACE__(OWE)

#include "TiledTexture.inl"

#endif //__OWE_TILED_TEXTURE_H__
