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
            desc.winWidth = 800;
            desc.winHeight = 600;
            desc.title = "Monomorphism";
            desc.glVersion = "4.3";

            //≥ı ºªØRenderContext
            RenderContext::InitInstance(desc);
            RenderContext &rc = RenderContext::GetInstance();
            InputManager &im = InputManager::GetInstance();
            im.AddKeyboardListener(this);

            rc.SetClearColor(0.0f, 1.0f, 1.0f, 0.0f);

            actor_.Initialize();
            actor_.SetPosition(vec2(1.0f, 0.0f));
            actor_.SetSize(vec2(0.72f, 0.88f));

            scale_.Reinit(100, 100);
            clock_.Restart();
            done_ = false;
            while(!done_)
            {
                rc.ClearColorAndDepth();
                clock_.Tick();

                actor_.UpdateMoving(clock_.ElapsedTime());
                actor_.UpdateJumping(clock_.ElapsedTime());
                actor_.SetVelocity(actor_.GetVelocity() - vec2(0.0f, 0.0005f));
                actor_.SetPosition(actor_.GetPosition() + static_cast<float>(clock_.ElapsedTime()) * actor_.GetVelocity());
                if(actor_.GetPosition().y <= 0.0f)
                {
                    actor_.EndFloating();
                    actor_.SetPosition(vec2(actor_.GetPosition().x, 0.0f));
                }
                actor_.Draw(scale_);

                rc.DoEvents();
                rc.Present();
            }
        }
    };
}

#endif //__TEST_APP_ACTOR_WALKING_H__
