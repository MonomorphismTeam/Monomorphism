/*================================================================
Filename: RenderContext.cpp
Date: 2017.9.11
Created by AirGuanZ
================================================================*/
#include <cstdio>
#include <exception>
#include <set>
#include <stack>

#include "Include\GLHeaders.h"
#include <GLFW\glfw3.h>

#include "Include\InputManager.h"
#include "Include\RenderContext.h"

__OWE_BEGIN_NAMESPACE__(OWE)

OWE_SINGLETON_INSTANCE_PTR(RenderContext);

namespace
{
    struct ViewportRecord { GLint data[4]; };
    std::stack<ViewportRecord> vpStack;
}

namespace
{
    GLint GLMaxColorAttachments = -1;

    void InitGLConsts(void)
    {
        glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &GLMaxColorAttachments);
    }
}

namespace
{
    bool glewInited = false;
    GLFWwindow *glfwWindow = nullptr;

    bool InitGLContext(const RenderContext::Desc &desc)
    {
        assert(!glewInited && !glfwWindow);
        assert(desc.vsync >= 0);
        if(!glfwInit())
            goto FAILED;

        int majorVer, minorVer;
        if(std::sscanf(desc.glVersion.c_str(), "%d.%d", &majorVer, &minorVer) != 2)
            goto FAILED;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVer);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVer);
        glfwWindowHint(GLFW_RESIZABLE, desc.resizable ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_DECORATED, desc.bordered ? GLFW_TRUE : GLFW_FALSE);
        GLFWmonitor *monitor = desc.fullscreen ? glfwGetPrimaryMonitor() : nullptr;
        if(!(glfwWindow = glfwCreateWindow(
                desc.winWidth, desc.winHeight, desc.title.c_str(),
                monitor, nullptr)))
            goto FAILED;

        glfwMakeContextCurrent(glfwWindow);
        if(desc.winLeft >= 0 && desc.winTop >= 0)
            glfwSetWindowPos(glfwWindow, desc.winLeft, desc.winTop);
        glfwSwapInterval(desc.vsync);

        glewExperimental = GL_TRUE;
        if(!glewInited)
        {
            if(glewInit() != GLEW_OK)
                goto FAILED;
            glewInited = true;
        }
        InitGLConsts();

        return true;

    FAILED:
        if(glfwWindow)
        {
            glfwTerminate();
            glfwWindow = nullptr;
        }
        return false;
    }

    void DestroyGLContext(void)
    {
        if(glfwWindow)
        {
            glfwTerminate();
            glfwWindow = nullptr;
        }
    }
}

int RenderContext::ClientWidth(void)
{
    assert(glfwWindow);
    int width;
    glfwGetFramebufferSize(glfwWindow, &width, nullptr);
    return width;
}

int RenderContext::ClientHeight(void)
{
    assert(glfwWindow);
    int height;
    glfwGetFramebufferSize(glfwWindow, nullptr, &height);
    return height;
}

int RenderContext::ClientLeft(void)
{
    assert(glfwWindow);
    int left;
    glfwGetWindowFrameSize(glfwWindow, &left, nullptr, nullptr, nullptr);
    return left;
}

int RenderContext::ClientTop(void)
{
    assert(glfwWindow);
    int top;
    glfwGetWindowFrameSize(glfwWindow, nullptr, &top, nullptr, nullptr);
    return top;
}

std::pair<int, int> RenderContext::ClientSize(void)
{
    assert(glfwWindow);
    int width, height;
    glfwGetFramebufferSize(glfwWindow, &width, &height);
    return std::make_pair(width, height);
}

std::pair<int, int> RenderContext::ClientPos(void)
{
    assert(glfwWindow);;
    int left, top;
    glfwGetWindowFrameSize(glfwWindow, &left, &top, nullptr, nullptr);
    return std::make_pair(left, top);
}

void RenderContext::SetWindowPos(int left, int top)
{
    assert(glfwWindow);
    glfwSetWindowPos(glfwWindow, left, top);
}

void RenderContext::SetWindowSize(int width, int height)
{
    assert(glfwWindow);
    glfwSetWindowSize(glfwWindow, width, height);
}

void RenderContext::SetTitle(const char *title)
{
    assert(glfwWindow);
    glfwSetWindowTitle(glfwWindow, title);
}

void RenderContext::SetVsync(int vsync)
{
    assert(glfwWindow && vsync >= 0);
    glfwSwapInterval(vsync);
}

void RenderContext::DoEvents(void)
{
    assert(glfwWindow);
    assert(InputManager::IsInstanceAvailable());
    glfwPollEvents();
    InputManager::GetInstance()._ProcessLockedCursor();
}

void RenderContext::Present(void)
{
    assert(glfwWindow);
    glfwSwapBuffers(glfwWindow);
}

namespace
{
    std::set<WindowListener*> wListeners;
}

void RenderContext::AddWindowListener(WindowListener *listener)
{
    wListeners.insert(listener);
}

void RenderContext::DelWindowListener(WindowListener *listener)
{
    wListeners.erase(listener);
}

void RenderContext::ClearWindowListener(void)
{
    wListeners.clear();
}

void RenderContext::CancelWindowClosing(void)
{
    assert(glfwWindow);
    glfwSetWindowShouldClose(glfwWindow, GLFW_FALSE);
}

void RenderContext::PushViewport(void)
{
    ViewportRecord vp;
    glGetIntegerv(GL_VIEWPORT, vp.data);
    vpStack.push(vp);
}

void RenderContext::PopViewport(void)
{
    ViewportRecord vp = vpStack.top();
    vpStack.pop();
    glViewport(vp.data[0], vp.data[1], vp.data[2], vp.data[3]);
}

GLint RenderContext::GetGLMaxColorAttachments(void)
{
    return GLMaxColorAttachments;
}

void *RenderContext::_GetWindowHandle(void)
{
    return glfwWindow;
}

//输入相关的glfw回调函数
namespace
{
    void KeyCallback(GLFWwindow*, int key, int, int action, int)
    {
        if(action == GLFW_PRESS)
            InputManager::GetInstance()._KeyDown(Int2KC(key));
        else if(action == GLFW_RELEASE)
            InputManager::GetInstance()._KeyUp(Int2KC(key));
    }

    void MouseMoveCallback(GLFWwindow*, double x, double y)
    {
        InputManager::GetInstance()._MouseMove(x, y);
    }

    void MouseButtonCallback(GLFWwindow*, int button, int action, int)
    {
        MOUSE_BUTTON mb;
        switch(button)
        {
        case GLFW_MOUSE_BUTTON_LEFT:
            mb = MOUSE_BUTTON::BUTTON_LEFT;
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            mb = MOUSE_BUTTON::BUTTON_MIDDLE;
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            mb = MOUSE_BUTTON::BUTTON_RIGHT;
            break;
        default:
            abort();
        }

        if(action == GLFW_PRESS)
            InputManager::GetInstance()._MBDown(mb);
        else if(action == GLFW_RELEASE)
            InputManager::GetInstance()._MBUp(mb);
    }

    void WindowClosedCallback(GLFWwindow*)
    {
        for(WindowListener *lis : wListeners)
            lis->WindowClosed();
    }
}

RenderContext::RenderContext(const RenderContext::Desc &desc)
{
    assert(!InputManager::IsInstanceAvailable());
    if(!InitGLContext(desc))
        throw std::runtime_error("Failed to initialize GL render context");

    glfwSetWindowCloseCallback(glfwWindow, WindowClosedCallback);

    InputManager::InitInstance();
    glfwSetKeyCallback(glfwWindow, KeyCallback);
    glfwSetCursorPosCallback(glfwWindow, MouseMoveCallback);
    glfwSetMouseButtonCallback(glfwWindow, MouseButtonCallback);
}

RenderContext::~RenderContext(void)
{
    DestroyGLContext();
    InputManager::DelInstance();
}

__OWE_END_NAMESPACE__(OWE)
