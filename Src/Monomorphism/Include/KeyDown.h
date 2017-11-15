/*================================================================
Filename: KeyDown.cpp
Date: 2017.11.8
Created by AirGuanZ
================================================================*/
#ifndef __KEY_DOWN_H__
#define __KEY_DOWN_H__

class KeyDown
{
public:
    bool Update(bool pressed)
    {
        bool rt = false;
        if(pressed && !pressed_)
            rt = true;
        pressed_ = pressed;
        return rt;
    }

private:
    bool pressed_ = false;
};

#endif //__KEY_DOWN_H__
