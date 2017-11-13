/*================================================================
Filename: App.h
Date: 2017.11.13
Created by AirGuanZ
================================================================*/
#ifndef __APP_H__
#define __APP_H__

#include <OWE.h>

#include "World.h"

class App
{
public:
    void Run(void)
    {
        //初始化RenderContext
        OWE::RenderContext::Desc desc;
        desc.bordered = false;
        desc.resizable = false;
        desc.fullscreen = false;
        desc.winLeft   = 50;
        desc.winTop    = 50;
        desc.winWidth  = 1600;
        desc.winHeight = 900;
        desc.title = "Monomorphism";
        desc.glVersion = "4.3";

        OWE::RenderContext::InitInstance(desc);
        OWE::RenderContext &rc = OWE::RenderContext::GetInstance();
        OWE::InputManager &im  = OWE::InputManager::GetInstance();

        //创建世界
        World::InitInstance();
        World::GetInstance().InitializeScene(128, 0);
        World::GetInstance().Run();
    }
};

#endif //__APP_H__
