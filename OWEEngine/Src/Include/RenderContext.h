/*================================================================
Filename: RenderContext.h
Date: 2017.9.11
By AirGuanZ
================================================================*/
#ifndef __OWE_RENDER_CONTEXT_H__
#define __OWE_RENDER_CONTEXT_H__

#include <string>
#include <tuple>
#include "Common.h"
#include "Singleton.h"

__OWE_BEGIN_NAMESPACE__(OWE)

namespace _RCAux
{
    struct _RenderContextDesc
    {
        //Window Appearance
        bool bordered         = true;
        bool fullscreen       = false;
        bool resizable        = true;
        std::string title     = "OpenGL Rendering Window";
        int winLeft           = -1;
        int winTop            = -1;
        int winWidth          = 640;
        int winHeight         = 480;
        //Context Attributes
        std::string glVersion = "4.3";
        bool vsync            = true;
    };
}

class WindowListener
{
public:
    friend class WindowListener;

    virtual void WindowResized(int left, int top, int width, int height) { };
    virtual void WindowMoved(int left, int top, int width, int height) { };
    virtual void WindowClosed(void) { };
};

/*
    负责OpenGL Render Context的创建和销毁
    以及GL硬件资源的管理
*/
class RenderContext : public Utility::Singleton<RenderContext>
{
public:
    //用于创建RenderContext的属性结构体
    using Desc = _RCAux::_RenderContextDesc;

    int ClientLeft(void);
    int ClientTop(void);
    int ClientWidth(void);
    int ClientHeight(void);
    std::pair<int, int> ClientSize(void);
    std::pair<int, int> ClientPos(void);

    void SetWindowPos(int left, int top);
    void SetWindowSize(int width, int height);

    void SetTitle(const char *title);

    void SetVsync(bool vsync);

    void DoEvents(void);
    void Present(void);

    void AddWindowListener(WindowListener *listener);
    void DelWindowListener(WindowListener *listener);
    void ClearWindowListener(void);

    void CancelWindowClosing(void);

    void *_GetWindowHandle(void);
private:
    //单例模式支持
    friend class SingletonType;
    
    RenderContext(const Desc &desc);
    RenderContext(const RenderContext&) = delete;
    RenderContext &operator=(const RenderContext&) = delete;
    ~RenderContext(void);
};

__OWE_END_NAMESPACE__(OWE)

#endif //__OWE_RENDER_CONTEXT_H__