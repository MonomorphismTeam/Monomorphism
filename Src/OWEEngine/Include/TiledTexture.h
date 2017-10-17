/*================================================================
Filename: TiledTexture.h
Date: 2017.10.17
Created by AirGuanZ
================================================================*/
#ifndef __OWE_TILED_TEXTURE_H__
#define __OWE_TILED_TEXTURE_H__

#include "Common.h"
#include "ScreenScale.h"
#include "Texture2DView.h"

__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(_TiledTextureAux)

class _TiledTexture
{
public:
    _TiledTexture(void);
    ~_TiledTexture(void);

    bool Initialize(int width, int height, float tileWidth, float tileHeight);
    bool IsAvailable(void) const;
    void Destroy(void);

    void SetTile(int tileX, int tileY, const glm::vec2 &uvLB, const glm::vec2 &uvRT, Texture2DView tex);
    Texture2DView GetTileTex(int tileX, int tileY) const;
    glm::vec2 GetTileTexCoordLB(int tileX, int tileY) const;
    glm::vec2 GetTileTexCoordRT(int tileX, int tileY) const;

    int Width(void) const;
    int Height(void) const;
    float TileWidth(void) const;
    float TileHeight(void) const;

    void Draw(const glm::vec2 &centre, ScreenScale &scale) const;
};

__OWE_END_NAMESPACE__(_TiledTextureAux)
__OWE_END_NAMESPACE__(OWE)

#endif //__OWE_TILED_TEXTURE_H__
