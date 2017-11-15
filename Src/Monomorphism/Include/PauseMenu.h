/*================================================================
Filename: PauseMenu.h
Date: 2017.11.14
Created by AirGuanZ
================================================================*/
#ifndef __PAUSE_MENU_H__
#define __PAUSE_MENU_H__

#include <OWE.h>

#include "KeyDown.h"
#include "World.h"

class PauseMenu
{
public:
    enum Selected
    {
        Back, Quit
    };

    bool Pause(void)
    {
        using namespace glm;

        OWE::ScreenScale scale;
        scale.Reinit(1.0f, 1.0f);

        OWE::RenderContext &rc = OWE::RenderContext::GetInstance();
        OWE::InputManager &im = OWE::InputManager::GetInstance();

        while(true)
        {
            if(up_.Update(im.IsKeyPressed(OWE::KEY_CODE::KEY_UP)))
            {
                selected_ = static_cast<Selected>(selected_ - 1);
                if(selected_ < 0)
                    selected_ = Selected::Quit;
            }
            else if(down_.Update(im.IsKeyPressed(OWE::KEY_CODE::KEY_DOWN)))
            {
                selected_ = static_cast<Selected>(selected_ + 1);
                if(selected_ > 1)
                    selected_ = Selected::Back;
            }
            else if(im.IsKeyPressed(OWE::KEY_CODE::KEY_ENTER))
                break;

            rc.ClearColorAndDepth();
            switch(selected_)
            {
            case Back:
                OWE::ImmediateRenderer::DrawTexturedBox(
                    vec2(0.0f), vec2(scale.ScreenWidth(), scale.ScreenHeight()),
                    vec2(0.0f), vec2(1.0f),
                    World::GetInstance().GetTextureManager().GetTexture("PauseBack"),
                    scale);
                break;
            case Quit:
                OWE::ImmediateRenderer::DrawTexturedBox(
                    vec2(0.0f), vec2(scale.ScreenWidth(), scale.ScreenHeight()),
                    vec2(0.0f), vec2(1.0f),
                    World::GetInstance().GetTextureManager().GetTexture("PauseQuit"),
                    scale);
                break;
            default:
                abort();
            }

            rc.DoEvents();
            rc.Present();
        }

        if(selected_ == Quit)
        {
            OWE::InputManager::GetInstance()._KeyUp(OWE::KEY_CODE::KEY_ENTER);
            return false;
        }
        return true;
    }

private:
    Selected selected_ = Back;

    KeyDown up_;
    KeyDown down_;
};

#endif //__PAUSE_MENU_H__
