/*================================================================
Filename: App.h
Date: 2017.11.13
Created by AirGuanZ
================================================================*/
#ifndef __APP_H__
#define __APP_H__

#include <OWE.h>

#include "Mainmenu.h"
#include "World.h"

class App
{
public:
    void Run(void)
    {
        //≥ı ºªØRenderContext
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

        OWE::ScreenScale scale;
        scale.Reinit(1.0f, 1.0f);
        menu::Mainmenu mainMenu("Bin\\MainMenu\\MainMenu.conf");
        mainMenu.callmainloop(scale);
    }
};

#endif //__APP_H__
