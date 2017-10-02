/*================================================================
Filename: VertexBuffer.h
Date: 2017.10.1
By AirGuanZ
================================================================*/
#ifndef __OWE_VERTEX_BUFFER_H__
#define __OWE_VERTEX_BUFFER_H__

#include <cassert>
#include <iostream>

#include "Common.h"
#include "GLHeaders.h"

__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(_VertexBufferAux)

template<bool _Dynamic, bool _Copy>
constexpr GLenum _VertexBufferUsage(void);

template<> constexpr GLenum _VertexBufferUsage<true, true>(void)
{
    return GL_DYNAMIC_COPY;
}

template<> constexpr GLenum _VertexBufferUsage<true, false>(void)
{
    return GL_DYNAMIC_DRAW;
}

template<> constexpr GLenum _VertexBufferUsage<false, false>(void)
{
    return GL_STATIC_DRAW;
}

class _VertexBufferInitializerBase
{
public:
    _VertexBufferInitializerBase(void) = default;

    bool IsAvailable(void) const
    {
        return vbo_ != 0;
    }

    void Destroy(void)
    {
        if(IsAvailable())
        {
            glDeleteBuffers(1, &vbo_);
            vbo_ = 0;
            elemCnt_ = 0;
        }
    }

protected:
    size_t elemCnt_ = 0;
    GLuint vbo_ = 0;
};

template<typename _ElemType, bool _Dynamic, bool _Copy>
class _VertexBufferSpecifier;

template<typename _ElemType, bool _Copy>
class _VertexBufferSpecifier<_ElemType, false, _Copy> : public _VertexBufferInitializerBase
{
public:
    bool Initialize(size_t elemCnt, const _ElemType *initData)
    {
        assert(IsAvailable() == false && initData != nullptr && elemCnt > 0);
        glGenBuffers(1, &vbo_);
        glBindBuffer(GL_VERTEX_ARRAY, vbo_);
        glBufferData(GL_VERTEX_ARRAY, elemCnt * sizeof(_ElemType), initData, _VertexBufferUsage<false, _Copy>());
        elemCnt_ = elemCnt;
        return true;
    }
};

template<typename _ElemType, bool _Copy>
class _VertexBufferSpecifier<_ElemType, true, _Copy> : public _VertexBufferInitializerBase
{
public:
    bool Initialize(size_t elemCnt, const _ElemType *initData)
    {
        assert(IsAvailable() == false && initData != nullptr && elemCnt > 0);
        glGenBuffers(1, &vbo_);
        glBindBuffer(GL_VERTEX_ARRAY, vbo_);
        glBufferData(GL_VERTEX_ARRAY, elemCnt * sizeof(_ElemType), initData, _VertexBufferUsage<true, _Copy>());
        elemCnt_ = elemCnt;
        return true;
    }

    bool Initialize(size_t elemCnt)
    {
        assert(IsAvailable() == false && elemCnt > 0);
        glGenBuffers(1, &vbo_);
        glBindBuffer(GL_VERTEX_ARRAY, vbo_);
        glBufferData(GL_VERTEX_ARRAY, elemCnt * sizeof(_ElemType), nullptr, _VertexBufferUsage<true, _Copy>());
        elemCnt_ = elemCnt;
        return true;
    }
};

template<typename _ElemType, bool _Dynamic, bool _Copy>
class _VertexBuffer : public _VertexBufferSpecifier<_ElemType, _Dynamic, _Copy>
{
public:
    using ElemType = _ElemType;

    _VertexBuffer(void) = default;

    GLuint _Unsafe_GetID(void) const
    {
        return vbo_;
    }
};

__OWE_END_NAMESPACE__(_VertexBufferAux)

template<typename _ElemType, bool _Dynamic = false, bool _Copy = false>
using VertexBuffer = typename _VertexBufferAux::_VertexBuffer<_ElemType, _Dynamic, _Copy>;

__OWE_END_NAMESPACE__(OWE)

#endif //__OWE_VERTEX_BUFFER_H__