/*================================================================
Filename: Texture2D.h
Date: 2017.10.4
Created by AirGuanZ
================================================================*/
#ifndef __OWE_TEXTURE_2D_H__
#define __OWE_TEXTURE_2D_H__

#include <string>

#include "Common.h"
#include "GLHeaders.h"
#include "Texture2DBase.h"

__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(_Texture2DAux)

struct Desc
{
    Desc(void)
        : Desc(0, 0, 0, 0,
               GL_RGBA, GL_NEAREST, GL_NEAREST,
               GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE)
    {

    }

    Desc(int _width, int _height,
         GLenum _inputFormat, GLenum _inputType,
         GLint _internalFormat = GL_RGBA,
         GLint _minFilter      = GL_NEAREST,
         GLint _magFilter      = GL_NEAREST,
         GLint _wrapS          = GL_CLAMP_TO_EDGE,
         GLint _wrapT          = GL_CLAMP_TO_EDGE)
        : width(_width), height(_height),
        inputFormat(_inputFormat), inputType(_inputType),
        internalFormat(_internalFormat),
        minFilter(_minFilter), magFilter(_magFilter),
        wrapS(_wrapS), wrapT(_wrapT)
    {

    }

    int width;
    int height;

    GLenum inputFormat;
    GLenum inputType;
    GLint internalFormat;

    GLint minFilter;
    GLint magFilter;

    GLint wrapS;
    GLint wrapT;
};

class _Texture2D : public Texture2DBase
{
public:
    using Desc = ::OWE::_Texture2DAux::Desc;

    _Texture2D(void);
    _Texture2D(const _Texture2D&) = delete;
    _Texture2D &operator=(const _Texture2D&) = delete;
    ~_Texture2D(void);

    void Initialize(const Desc &desc, void *data);
    void Destroy(void);

private:
    int width_;
    int height_;
};

bool _LoadTexture2DFromFile(const std::string &filename, const _Texture2D::Desc &desc, _Texture2D &tex);

__OWE_END_NAMESPACE__(_Texture2DAux)

using Texture2D = _Texture2DAux::_Texture2D;

inline bool LoadTexture2DFromFile(const std::string &filename, const Texture2D::Desc &desc, Texture2D &tex)
{
    return _Texture2DAux::_LoadTexture2DFromFile(filename, desc, tex);
}

__OWE_END_NAMESPACE__(OWE)

#endif //__OWE_TEXTURE_2D_H__
