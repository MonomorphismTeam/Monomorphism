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

        Clock clock_;
        ScreenScale scale_;

        Actor actor_;

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

            actor_.Initialize();
            actor_.SetPosition(vec2(5.0f, 0.0f));

            scale_.Reinit(50.0f, 50.0f);
            clock_.Restart();
            done_ = false;
            while(!done_)
            {
                rc.ClearColorAndDepth();
                clock_.Tick();

                actor_.UpdateMoving(clock_.ElapsedTime());
                actor_.UpdateJumping(clock_.ElapsedTime());

                rc.DoEvents();
                rc.Present();
            }
        }
    };
}

#endif //__TEST_APP_ACTOR_WALKING_H__
