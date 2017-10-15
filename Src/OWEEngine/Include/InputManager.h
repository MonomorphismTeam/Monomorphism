/*================================================================
Filename: InputManager.h
Date: 2017.9.12
Created by AirGuanZ
================================================================*/
#ifndef __OWE_INPUT_MANAGER_H__
#define __OWE_INPUT_MANAGER_H__

#include <string>

#include "Common.h"
#include "Singleton.h"

__OWE_BEGIN_NAMESPACE__(OWE)

/*
    USB HID Usage Tables v1.12 (p. 53-60).
    Rearranged to map 7-bit ASCII for printable keys.
    与GLFW键值定义一致
*/
enum class KEY_CODE
{
    KEY_SPACE           = 32,
    KEY_APOSTROPHE      = 39, //'
    KEY_COMMA           = 44, //,
    KEY_MINUS           = 45, //-
    KEY_PERIOD          = 46, //.
    KEY_SLASH           = 47, ///
    
    KEY_0               = 48,
    KEY_1               = 49,
    KEY_2               = 50,
    KEY_3               = 51,
    KEY_4               = 52,
    KEY_5               = 53,
    KEY_6               = 54,
    KEY_7               = 55,
    KEY_8               = 56,
    KEY_9               = 57,
    
    KEY_SEMICOLON       = 59, //;
    KEY_EQUAL           = 61, //=
    
    KEY_A               = 65,
    KEY_B               = 66,
    KEY_C               = 67,
    KEY_D               = 68,
    KEY_E               = 69,
    KEY_F               = 70,
    KEY_G               = 71,
    KEY_H               = 72,
    KEY_I               = 73,
    KEY_J               = 74,
    KEY_K               = 75,
    KEY_L               = 76,
    KEY_M               = 77,
    KEY_N               = 78,
    KEY_O               = 79,
    KEY_P               = 80,
    KEY_Q               = 81,
    KEY_R               = 82,
    KEY_S               = 83,
    KEY_T               = 84,
    KEY_U               = 85,
    KEY_V               = 86,
    KEY_W               = 87,
    KEY_X               = 88,
    KEY_Y               = 89,
    KEY_Z               = 90,
    
    KEY_LBRACKET        = 91, //[
    KEY_BACKSLASH       = 92, //backslash
    KEY_RBRACKET        = 93, //]
    KEY_GRAVE_ACCENT    = 96, //`
    
    KEY_ESCAPE          = 256,
    KEY_ENTER           = 257,
    KEY_TAB             = 258,
    KEY_BACKSPACE       = 259,
    KEY_INSERT          = 260,
    KEY_DELETE          = 261,
    
    KEY_RIGHT           = 262,
    KEY_LEFT            = 263,
    KEY_DOWN            = 264,
    KEY_UP              = 265,
    
    KEY_PAGE_UP         = 266,
    KEY_PAGE_DOWN       = 267,
    KEY_HOME            = 268,
    KEY_END             = 269,
    
    KEY_CAPS_LOCK       = 280,
    KEY_NUM_LOCK        = 282,
    KEY_PRINT_SCREEN    = 283,
    KEY_PAUSE           = 284,
    
    KEY_F1              = 290,
    KEY_F2              = 291,
    KEY_F3              = 292,
    KEY_F4              = 293,
    KEY_F5              = 294,
    KEY_F6              = 295,
    KEY_F7              = 296,
    KEY_F8              = 297,
    KEY_F9              = 298,
    KEY_F10             = 299,
    KEY_F11             = 300,
    KEY_F12             = 301,
    
    KEY_NUMPAD_0        = 320,
    KEY_NUMPAD_1        = 321,
    KEY_NUMPAD_2        = 322,
    KEY_NUMPAD_3        = 323,
    KEY_NUMPAD_4        = 324,
    KEY_NUMPAD_5        = 325,
    KEY_NUMPAD_6        = 326,
    KEY_NUMPAD_7        = 327,
    KEY_NUMPAD_8        = 328,
    KEY_NUMPAD_9        = 329,
    
    KEY_NUMPAD_DECIMAL  = 330,
    KEY_NUMPAD_DIV      = 331,
    KEY_NUMPAD_MUL      = 332,
    KEY_NUMPAD_SUB      = 333,
    KEY_NUMPAD_ADD      = 334,
    KEY_NUMPAD_ENTER    = 335,
    
    KEY_LSHIFT          = 340,
    KEY_LCTRL           = 341,
    KEY_LALT            = 342,
   
    KEY_RSHIFT          = 344,
    KEY_RCTRL           = 345,
    KEY_RALT            = 346,
    KEY_MENU            = 348,

    KEY_CODE_MAX
};

/*
    鼠标左键，滚轮，右键
*/
enum class MOUSE_BUTTON
{
    BUTTON_LEFT         = 0,
    BUTTON_MIDDLE       = 1,
    BUTTON_RIGHT        = 2,
    MOUSE_BUTTON_MAX
};

/*
    KEY_CODE, MOUSE_BUTTON和其int值间的类型转换
*/

inline constexpr int KC2Int(KEY_CODE kc)
    { return static_cast<int>(kc); }
inline constexpr KEY_CODE Int2KC(int v)
    { return static_cast<KEY_CODE>(v); }

inline constexpr int MB2Int(MOUSE_BUTTON mb)
    { return static_cast<int>(mb); }
inline constexpr MOUSE_BUTTON int2MB(int v)
    { return static_cast<MOUSE_BUTTON>(v); }

/*
    键盘输入监听，在InputManager处注册以使用
*/
class KeyboardListener
{
protected:
    friend class InputManager;
    
    virtual void KeyDown(KEY_CODE kc) { }
    virtual void KeyUp  (KEY_CODE kc) { }
};

/*
    鼠标输入监听，在InputManager处注册以使用
*/
class MouseListener
{
protected:
    friend class InputManager;
    
    virtual void MouseButtonDown(MOUSE_BUTTON button) { }
    virtual void MouseButtonUp  (MOUSE_BUTTON button) { }
    
    virtual void MouseWheel(double roll) { }
    virtual void MouseMove(double absX, double absY, double relX, double relY) { }
};

/*
    全局输入管理
    后于RenderContext生效
*/
class InputManager : public Utility::Singleton<InputManager>
{
public:
    bool IsKeyPressed        (KEY_CODE kc);
    bool IsMouseButtonPressed(MOUSE_BUTTON button);

    double GetMousePosX(void);
    double GetMousePosY(void);
    double GetMouseRelX(void);
    double GetMouseRelY(void);

    void LockCursor(bool lock);
    bool IsCursorLocked(void);
    void SetCursorLockPos(double x, double y);

    void ShowCursor(bool show);

    void AddKeyboardListener(KeyboardListener *listener);
    void AddMouseListener(MouseListener *listener);

    void DelKeyboardListener(KeyboardListener *listener);
    void DelMouseListener(MouseListener *listener);

    void ClearKeyboardListener(void);
    void ClearMouseListener(void);

    void _KeyDown(KEY_CODE kc);
    void _KeyUp(KEY_CODE kc);
    void _MBDown(MOUSE_BUTTON mb);
    void _MBUp(MOUSE_BUTTON mb);
    void _MouseMove(double absX, double absY);
    void _MouseWheel(double roll);

    void _ProcessLockedCursor(void);

private:
    friend class SingletonType;

    InputManager(void);
    InputManager(const InputManager&) = delete;
    InputManager &operator=(const InputManager&) = delete;
    ~InputManager(void);
};

__OWE_END_NAMESPACE__(OWE)

#endif //__OWE_INPUT_MANAGER_H__
