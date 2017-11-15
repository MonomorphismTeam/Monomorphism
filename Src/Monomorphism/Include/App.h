/*================================================================
Filename: App.h
Date: 2017.11.13
Created by AirGuanZ
================================================================*/
#ifndef __APP_H__
#define __APP_H__

#include <ctime>
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
        desc.fullscreen = true;
        desc.winLeft   = 50;
        desc.winTop    = 50;
        desc.winWidth  = 800;
        desc.winHeight = 600;
        desc.title = "Monomorphism";
        desc.glVersion = "4.3";

        OWE::RenderContext::InitInstance(desc);
        OWE::RenderContext &rc = OWE::RenderContext::GetInstance();
        OWE::InputManager &im  = OWE::InputManager::GetInstance();

        srand(static_cast<unsigned int>(time(0)));

        OWE::ScreenScale scale;
        scale.Reinit(1.0f, 1.0f);
        menu::Mainmenu mainMenu("Bin\\MainMenu\\MainMenu.conf");
        mainMenu.callmainloop(scale);
    }
};

#endif //__APP_H__
