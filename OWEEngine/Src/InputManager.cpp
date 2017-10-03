/*================================================================
Filename: InputManager.cpp
Date: 2017.9.12
By AirGuanZ
================================================================*/
#include <cassert>
#include <cstring>
#include <set>
#include <Windows.h>

#include "Include\GLHeaders.h"
#include <GLFW\glfw3.h>

#include "Include\Common.h"
#include "Include\InputManager.h"
#include "Include\RenderContext.h"

__OWE_BEGIN_NAMESPACE__(OWE)

//InputManagerµ¥ÀýÖ¸Õë
OWE_SINGLETON_INSTANCE_PTR(InputManager);

namespace
{
    bool keyPressed[KC2Int(KEY_CODE::KEY_CODE_MAX)];
    bool mbPressed[MB2Int(MOUSE_BUTTON::MOUSE_BUTTON_MAX)];

    double mAbsX, mAbsY;
    double mRelX, mRelY;

    bool cursorLocked;
    double cursorLockX, cursorLockY;

    std::set<KeyboardListener*> kbListeners;
    std::set<MouseListener*> mListeners;
}

bool InputManager::IsKeyPressed(KEY_CODE kc)
{
    return keyPressed[KC2Int(kc)];
}

bool InputManager::IsMouseButtonPressed(MOUSE_BUTTON button)
{
    return mbPressed[MB2Int(button)];
}

double InputManager::GetMousePosX(void)
{
    return mAbsX;
}

double InputManager::GetMousePosY(void)
{
    return mAbsY;
}

double InputManager::GetMouseRelX(void)
{
    return mRelX;
}

double InputManager::GetMouseRelY(void)
{
    return mRelY;
}

void InputManager::LockCursor(bool locked)
{
    cursorLocked = locked;
}

bool InputManager::IsCursorLocked(void)
{
    return cursorLocked;
}

void InputManager::SetCursorLockPos(double x, double y)
{
    cursorLockX = x;
    cursorLockY = y;
}

void InputManager::ShowCursor(bool show)
{
    assert(RenderContext::IsInstanceAvailable());
    ::ShowCursor(show ? TRUE : FALSE);
}

void InputManager::AddKeyboardListener(KeyboardListener *listener)
{
    kbListeners.insert(listener);
}

void InputManager::AddMouseListener(MouseListener *listener)
{
    mListeners.insert(listener);
}

void InputManager::DelKeyboardListener(KeyboardListener *listener)
{
    kbListeners.erase(listener);
}

void InputManager::DelMouseListener(MouseListener *listener)
{
    mListeners.erase(listener);
}

void InputManager::ClearKeyboardListener(void)
{
    kbListeners.clear();
}

void InputManager::ClearMouseListener(void)
{
    mListeners.clear();
}

InputManager::InputManager(void)
{
    std::memset(keyPressed, 0, sizeof(keyPressed));
    std::memset(mbPressed, 0, sizeof(mbPressed));

    mAbsX = 0.0, mAbsY = 0.0;
    mRelX = 0.0, mRelY = 0.0;

    cursorLocked = false;
    cursorLockX = 0.0, cursorLockY = 0.0;

    kbListeners.clear();
    mListeners.clear();
}

InputManager::~InputManager(void)
{
    kbListeners.clear();
    mListeners.clear();
}

void InputManager::_KeyDown(KEY_CODE kc)
{
    int id = KC2Int(kc);
    assert(0 <= id && id < KC2Int(KEY_CODE::KEY_CODE_MAX));
    keyPressed[id] = true;
    for(KeyboardListener *listener : kbListeners)
        listener->KeyDown(kc);
}

void InputManager::_KeyUp(KEY_CODE kc)
{
    int id = KC2Int(kc);
    assert(0 <= id && id < KC2Int(KEY_CODE::KEY_CODE_MAX));
    keyPressed[id] = false;
    for(KeyboardListener *listener : kbListeners)
        listener->KeyUp(kc);
}

void InputManager::_MBDown(MOUSE_BUTTON mb)
{
    int id = MB2Int(mb);
    assert(0 <= id && id < MB2Int(MOUSE_BUTTON::MOUSE_BUTTON_MAX));
    mbPressed[id] = true;
    for(MouseListener *listener : mListeners)
        listener->MouseButtonDown(mb);
}

void InputManager::_MBUp(MOUSE_BUTTON mb)
{
    int id = MB2Int(mb);
    assert(0 <= id && id < MB2Int(MOUSE_BUTTON::MOUSE_BUTTON_MAX));
    mbPressed[id] = false;
    for(MouseListener *listener : mListeners)
        listener->MouseButtonUp(mb);
}

void InputManager::_MouseMove(double absX, double absY)
{
    if(cursorLocked)
        return;
    mRelX = absX - mAbsX;
    mRelY = absY - mAbsY;
    mAbsX = absX;
    mAbsY = absY;
    for(MouseListener *listener : mListeners)
        listener->MouseMove(mAbsX, mAbsY, mRelX, mRelY);
}

void InputManager::_MouseWheel(double roll)
{
    for(MouseListener *listener : mListeners)
        listener->MouseWheel(roll);
}

void InputManager::_ProcessLockedCursor(void)
{
    assert(RenderContext::IsInstanceAvailable());
    if(!cursorLocked)
        return;

    void *_win = RenderContext::GetInstance()._GetWindowHandle();
    GLFWwindow *win = reinterpret_cast<GLFWwindow*>(_win);
    double curX, curY;
    glfwGetCursorPos(win, &curX, &curY);
    mRelX = curX - mAbsX;
    mRelY = curY - mAbsY;
    glfwSetCursorPos(win, cursorLockX, cursorLockY);
    glfwGetCursorPos(win, &mAbsX, &mAbsY);

    if(mRelX || mRelY)
    {
        for(MouseListener *lis : mListeners)
            lis->MouseMove(mAbsX, mAbsY, mRelX, mRelY);
    }
}

__OWE_END_NAMESPACE__(OWE)
