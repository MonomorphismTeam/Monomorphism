/*================================================================
Filename: TiledTexture.inl
Date: 2017.10.18
Created by AirGuanZ
================================================================*/
__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(_TiledTextureAux)

inline int _TileIndex(int height, int tileX, int tileY)
{
    return tileY * height + tileX;
}

inline _TiledTexture::_TiledTexture(void)
    : width_(0), height_(0),
    tileWidth_(0.0f), tileHeight_(0.0f)
{

}

inline bool _TiledTexture::IsAvailable(void) const
{
    return width_ != 0;
}

inline void _TiledTexture::SetTile(int tileX, int tileY, const glm::vec2 &uvLB, const glm::vec2 &uvRT, Texture2DView tex)
{
    assert(IsAvailable());
    assert(0 <= tileX && tileX < width_);
    assert(0 <= tileY && tileY < height_);
    tiles_[_TileIndex(height_, tileX, tileY)] = _Tile{ uvLB, uvRT, tex };
}

inline Texture2DView _TiledTexture::GetTileTex(int tileX, int tileY) const
{
    assert(IsAvailable());
    assert(0 <= tileX && tileX < width_);
    assert(0 <= tileY && tileY < height_);
    return tiles_[_TileIndex(height_, tileX, tileY)].tex;
}

inline glm::vec2 _TiledTexture::GetTileTexCoordLB(int tileX, int tileY) const
{
    assert(IsAvailable());
    assert(0 <= tileX && tileX < width_);
    assert(0 <= tileY && tileY < height_);
    return tiles_[_TileIndex(height_, tileX, tileY)].uvLB;
}

inline glm::vec2 _TiledTexture::GetTileTexCoordRT(int tileX, int tileY) const
{
    assert(IsAvailable());
    assert(0 <= tileX && tileX < width_);
    assert(0 <= tileY && tileY < height_);
    return tiles_[_TileIndex(height_, tileX, tileY)].uvRT;
}

inline int _TiledTexture::Width(void) const
{
    assert(IsAvailable());
    return width_;
}

inline int _TiledTexture::Height(void) const
{
    assert(IsAvailable());
    return height_;
}

inline float _TiledTexture::TileWidth(void) const
{
    assert(IsAvailable());
    return tileWidth_;
}

inline float _TiledTexture::TileHeight(void) const
{
    assert(IsAvailable());
    return tileHeight_;
}

__OWE_END_NAMESPACE__(_TiledTextureAux)
__OWE_END_NAMESPACE__(OWE)
