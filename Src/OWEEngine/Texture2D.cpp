/*================================================================
Filename: Texture.cpp
Date: 2017.10.5
Created by AirGuanZ
================================================================*/
#include <cassert>
#include <cstdint>
#include <vector>

#include <FreeImage.h>

#include "Include\Texture2D.h"

__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(_Texture2DAux)

_Texture2D::_Texture2D(void)
    : Texture2DBase(0)
{

}

_Texture2D::_Texture2D(_Texture2D &&other)
    : Texture2DBase(std::move(other)), width_(other.width_), height_(other.height_)
{
    other.width_  = 0;
    other.height_ = 0;
}

_Texture2D::~_Texture2D(void)
{
    Destroy();
}

void _Texture2D::Initialize(const Desc &desc, void *data)
{
    assert(desc.width > 0 && desc.height > 0);
    Destroy();

    GLint curTex;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &curTex);

    glGenTextures(1, &tex_);
    glBindTexture(GL_TEXTURE_2D, tex_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, desc.magFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, desc.minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, desc.wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, desc.wrapT);
    glTexImage2D(
        GL_TEXTURE_2D, 0, desc.internalFormat,
        static_cast<GLsizei>(desc.width),
        static_cast<GLsizei>(desc.height),
        0, desc.inputFormat, desc.inputType, data);
    width_ = desc.width;
    height_ = desc.height;

    glBindTexture(GL_TEXTURE_2D, curTex);
}

void _Texture2D::Destroy(void)
{
    if (IsAvailable())
    {
        glDeleteTextures(1, &tex_);
        tex_ = 0;
    }
}

Texture2DView _Texture2D::GetTextureView(void) const
{
    return Texture2DView(tex_);
}

int _Texture2D::Width(void) const
{
    return width_;
}

int _Texture2D::Height(void) const
{
    return height_;
}

bool _LoadTexture2DFromFile(const std::string &filename, const _Texture2D::Desc &desc, _Texture2D &tex)
{
    //取得文件格式，加载位图
    FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(filename.c_str());
    if(fif == FIF_UNKNOWN || !FreeImage_FIFSupportsReading(fif))
        return false;
    FIBITMAP *bmp = FreeImage_Load(fif, filename.c_str());
    if(!bmp)
        return false;

    //图像基本信息
    //IMPROVE：FreeImage应该提供了更好的确定像素格式的方法
    unsigned int bpp    = FreeImage_GetBPP(bmp) >> 3; //bytes per pixel
    unsigned int width  = FreeImage_GetWidth(bmp);
    unsigned int height = FreeImage_GetHeight(bmp);

    //输入分量数量
    GLenum inputFormat;
    switch(bpp)
    {
    case 3: inputFormat = GL_RGB;  break;
    case 4: inputFormat = GL_RGBA; break;
    default:
        FreeImage_Unload(bmp);
        return false;
    }

    //IMPROVE：优化把bmp的数据读取到inputData的效率
    std::vector<std::uint8_t> inputData(width * height * bpp);
    uint8_t *pIn = inputData.data();
    size_t inputIdx = 0;
    for(size_t y = 0; y != height; ++y)
    {
        BYTE *scanline = FreeImage_GetScanLine(bmp, y);
        for(size_t x = 0; x != width; ++x)
        {
            pIn[inputIdx++] = scanline[FI_RGBA_RED];
            pIn[inputIdx++] = scanline[FI_RGBA_GREEN];
            pIn[inputIdx++] = scanline[FI_RGBA_BLUE];
            if(bpp >= 4)
                pIn[inputIdx++] = scanline[FI_RGBA_ALPHA];
            scanline += bpp;
        }
    }

    _Texture2D::Desc fDesc = desc;
    fDesc.inputFormat = inputFormat;
    fDesc.inputType   = GL_UNSIGNED_BYTE;
    fDesc.width       = width;
    fDesc.height      = height;
    tex.Initialize(fDesc, inputData.data());

    FreeImage_Unload(bmp);
    return true;
}

__OWE_END_NAMESPACE__(_Texture2DAux)
__OWE_END_NAMESPACE__(OWE)
