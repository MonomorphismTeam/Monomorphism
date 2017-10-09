/*================================================================
Filename: FrameBuffer.cpp
Date: 2017.10.9
By AirGuanZ
================================================================*/
#include "Include\GLHeaders.h"

#include "Include\FrameBuffer.h"
#include "Include\RenderContext.h"

__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(_FrameBufferAux)

_FrameBuffer::_FrameBuffer(void)
    : width_(0), height_(0), fbo_(0), depth_(0), stencil_(0)
{

}

_FrameBuffer::~_FrameBuffer(void)
{
    Destroy();
}

bool _FrameBuffer::Initialize(int width, int height, int attachPntCnt)
{
    assert(width > 0 && height > 0 && attachPntCnt > 0);
    if(attachPntCnt >= RenderContext::GetInstance().GetGLMaxColorAttachments())
        return false;
    Destroy();

    glGenFramebuffers(1, &fbo_);
    assert(fbo_ != 0);

    width_ = width;
    height_ = height;
    attachRecs_.resize(attachPntCnt, AttachTexRecord{ 0, false });

    return true;
}

void _FrameBuffer::Destroy(void)
{
    width_ = 0;
    height_ = 0;
    
    for(AttachTexRecord &rec : attachRecs_)
    {
        if(rec.ID && rec.owned)
            glDeleteTextures(1, &rec.ID);
    }
    attachRecs_.clear();

    if(depth_)
    {
        glDeleteRenderbuffers(1, &depth_);
        depth_ = 0;
    }

    if(stencil_)
    {
        glDeleteRenderbuffers(1, &stencil_);
        stencil_ = 0;
    }

    if(fbo_)
    {
        glDeleteFramebuffers(1, &fbo_);
        fbo_ = 0;
    }
}

bool _FrameBuffer::IsAvailable(void) const
{
    if(fbo_ == 0)
        return false;
    GLint curFB;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &curFB);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    bool rt = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;

    glBindFramebuffer(GL_FRAMEBUFFER, curFB);
    return rt;
}

int _FrameBuffer::Width(void) const
{
    return width_;
}

int _FrameBuffer::Height(void) const
{
    return height_;
}

bool _FrameBuffer::HasDepth(void) const
{
    return depth_ != 0;
}

bool _FrameBuffer::HasStencil(void) const
{
    return stencil_ != 0;
}

bool _FrameBuffer::AddTex(GLint attachPnt, const Texture2D::Desc &desc)
{
    assert(fbo_ != 0 && 0 <= attachPnt && attachPnt < attachRecs_.size());

    GLuint texID; GLint curID, curFB;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &curID);
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &curFB);

    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, desc.magFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, desc.minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, desc.wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, desc.wrapT);
    glTexStorage2D(GL_TEXTURE_2D, 0, desc.internalFormat, width_, height_);

    AttachTexRecord &rec = attachRecs_[attachPnt];
    if(rec.ID && rec.owned)
        glDeleteTextures(1, &rec.ID);
    rec.ID = texID;
    rec.owned = true;

    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachPnt, GL_TEXTURE_2D, texID, 0);

    glBindTexture(GL_TEXTURE_2D, curID);
    glBindFramebuffer(GL_FRAMEBUFFER, curFB);
    return true;

FAILED:
    glBindTexture(GL_TEXTURE_2D, curID);
    glBindFramebuffer(GL_FRAMEBUFFER, curFB);
    if(texID)
        glDeleteTextures(1, &texID);
    return false;
}

bool _FrameBuffer::AddTex(GLint attachPnt, const Texture2D &tex)
{
    assert(fbo_ != 0 && 0 <= attachPnt && attachPnt < attachRecs_.size());
    assert(tex.IsAvailable());

    AttachTexRecord &rec = attachRecs_[attachPnt];
    if(rec.ID && rec.owned)
        glDeleteTextures(1, &rec.ID);

    GLint curID, curFB;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &curID);
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &curFB);

    glBindTexture(GL_TEXTURE_2D, tex._Unsafe_GetID());
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachPnt, GL_TEXTURE_2D, tex._Unsafe_GetID(), 0);
    rec.ID = tex._Unsafe_GetID();
    rec.owned = false;

    glBindTexture(GL_TEXTURE_2D, curID);
    glBindFramebuffer(GL_FRAMEBUFFER, curFB);
    return true;
}

bool _FrameBuffer::AddDepth(void)
{
    assert(!fbo_ && !depth_);

    GLint curRB, curFB;
    glGetIntegerv(GL_RENDERBUFFER_BINDING, &curRB);
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &curFB);

    glGenRenderbuffers(1, &depth_);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width_, height_);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_);

    glBindRenderbuffer(GL_RENDERBUFFER, curRB);
    glBindFramebuffer(GL_FRAMEBUFFER, curFB);
    return true;
}

bool _FrameBuffer::AddStencil(void)
{
    assert(!fbo_ && !depth_);

    GLint curRB, curFB;
    glGetIntegerv(GL_RENDERBUFFER_BINDING, &curRB);
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &curFB);

    glGenRenderbuffers(1, &depth_);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX, width_, height_);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_);

    glBindRenderbuffer(GL_RENDERBUFFER, curRB);
    glBindFramebuffer(GL_FRAMEBUFFER, curFB);
    return true;
}

GLint _FrameBuffer::GetTexID(GLint attachPnt) const
{
    assert(0 <= attachPnt && attachPnt < attachRecs_.size());
    return attachRecs_[attachPnt].ID;
}

void _FrameBuffer::BindTex(GLint attachPnt, GLint slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, GetTexID(attachPnt));
}

void _FrameBuffer::Begin(void) const
{
    if(IsAvailable())
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
        glPushAttrib(GL_VIEWPORT_BIT);
        glViewport(0, 0, width_, height_);
    }
}

void _FrameBuffer::End(void) const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glPopAttrib();
}

__OWE_END_NAMESPACE__(_FrameBufferAux)
__OWE_END_NAMESPACE__(OWE)
