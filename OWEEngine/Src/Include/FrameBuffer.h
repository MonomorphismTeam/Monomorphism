/*================================================================
Filename: FrameBuffer.h
Date: 2017.10.9
Created by AirGuanZ
================================================================*/
#ifndef __OWE_FRAME_BUFFER_H__
#define __OWE_FRAME_BUFFER_H__

#include <vector>

#include "Common.h"
#include "GLHeaders.h"
#include "Texture2D.h"

__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(_FrameBufferAux)

class _FrameBuffer
{
public:
    _FrameBuffer(void);
    _FrameBuffer(const _FrameBuffer&) = delete;
    _FrameBuffer &operator=(const _FrameBuffer&) = delete;
    ~_FrameBuffer(void);

    bool Initialize(int width, int height, int attachPntCnt);
    void Destroy(void);
    bool IsAvailable(void) const;

    int Width(void) const;
    int Height(void) const;

    bool HasDepth(void) const;
    bool HasStencil(void) const;

    //TODO：添加depth + stencil共用一块buffer的功能
    void AddTex(GLint attachPnt, const Texture2D::Desc &desc);
    void AddTex(GLint attachPnt, const Texture2D &tex);
    void AddDepth(void);
    void AddStencil(void);

    GLint GetTexID(GLint attachPnt) const;
    void BindTex(GLint attachPnt, GLint slot) const;

    //Framebuffer的begin、end对不允许嵌套
    //IMPROVE：去掉这个限制
    void Begin(void) const;
    void End(void) const;

private:
    struct AttachTexRecord
    {
        GLuint ID;      //纹理ID
        bool owned;     //是否拥有纹理资源所有权
    };

    int width_;
    int height_;

    GLuint fbo_;
    GLuint depth_;
    GLuint stencil_;
    std::vector<AttachTexRecord> attachRecs_;
};

__OWE_END_NAMESPACE__(_FrameBufferAux)

using FrameBuffer = _FrameBufferAux::_FrameBuffer;

__OWE_END_NAMESPACE__(OWE)

#endif //__OWE_FRAME_BUFFER_H__
