/*================================================================
Filename: TestApp_ActorWalking.h
Date: 2017.10.27
Created by AirGuanZ
================================================================*/
#ifndef __TEST_APP_ACTOR_WALKING_H__
#define __TEST_APP_ACTOR_WALKING_H__

#include <glm\glm.hpp>
#include <OWE.h>
#include "Actor.h"

namespace Test
{
    using namespace std;
    using namespace glm;
    using namespace OWE;

    class TestApp_ActorWalking : public KeyboardListener
    {
        bool done_;

        Clock clock;
        ScreenScale scale_;

        Actor actor;

    public:
        void KeyDown(KEY_CODE kc)
        {
            if(kc == KEY_CODE::KEY_ESCAPE)
                done_ = true;
        }

        void Run(void)
        {
            RenderContext::Desc desc;
            desc.bordered = true;
            desc.resizable = false;
            desc.fullscreen = false;
            desc.winWidth = 1440;
            desc.winHeight = 768;
            desc.title = "Monomorphism";
            desc.glVersion = "4.3";

            //≥ı ºªØRenderContext
            RenderContext::InitInstance(desc);
            RenderContext &rc = RenderContext::GetInstance();
            InputManager &im = InputManager::GetInstance();
            im.AddKeyboardListener(this);
        }
    };
}

#endif //__TEST_APP_ACTOR_WALKING_H__
