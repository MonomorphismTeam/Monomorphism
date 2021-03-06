/*================================================================
Filename: Texture2DBase.h
Date: 2017.10.11
Created by AirGuanZ
================================================================*/
#ifndef __OWE_TEXTURE_2D_BASE_H__
#define __OWE_TEXTURE_2D_BASE_H__

#include <cassert>

#include "Common.h"
#include "GLHeaders.h"

__OWE_BEGIN_NAMESPACE__(OWE)

class Texture2DBase
{
public:
    explicit Texture2DBase(GLuint tex = 0)
        : tex_(tex)
    {

    }

    Texture2DBase(const Texture2DBase &) = default;
    Texture2DBase &operator=(const Texture2DBase&) = default;

    Texture2DBase(Texture2DBase &&other)
        : tex_(other.tex_)
    {
        other.tex_ = 0;
    }

    bool IsAvailable(void) const
    {
        return tex_ != 0;
    }

    void Bind(GLint slot) const
    {
        assert(IsAvailable());
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, tex_);
    }

    GLuint _Unsafe_GetID(void) const
    {
        return tex_;
    }

protected:
    GLuint tex_;
};

__OWE_END_NAMESPACE__(OWE)

#endif //__OWE_TEXTURE_2D_BASE_H__
