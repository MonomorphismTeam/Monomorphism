/*================================================================
Filename: TestApp_ActorMoving.h
Date: 2017.10.30
Created by AirGuanZ
================================================================*/
#ifndef __TEST_ACTOR_MOVING_H__
#define __TEST_ACTOR_MOVING_H__

#include <glm\glm.hpp>
#include <OWE.h>

#include "Actor.h"

namespace Test
{
    using namespace std;
    using namespace glm;
    using namespace OWE;

    class TestApp_ActorMoving : public KeyboardListener
    {
        Actor actor_;

        ScreenScale scale_;
        Clock clock_;

        bool done_;
        bool space_;

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

            //初始化RenderContext
            RenderContext::InitInstance(desc);
            RenderContext &rc = RenderContext::GetInstance();
            InputManager &im = InputManager::GetInstance();
            im.AddKeyboardListener(this);

            //初始化角色资源和参数
            actor_.Initialize();

            actor_.SetRunningVel(0.012f);
            actor_.SetFloatingAccVel(0.00006f);
            actor_.SetShiftingVel(0.04f);
            actor_.SetJumpingVel(0.025f);

            actor_.SetMaxFloatingVel(0.012f);
            actor_.SetFloatingFricAccVel(0.00003f);

            actor_.GetPosition() = vec2(3.0f, 2.0f);
            actor_.GetTexSize() = vec2(2.0f, 2.8f);

            rc.SetClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            scale_.Reinit(40.0f, 40.0f);
            done_ = false;
            space_ = false;
            clock_.Restart();
            
            while(!done_)
            {
                clock_.Tick();
                
                actor_.ClearAccVel();
                actor_.Update(clock_.ElapsedTime());

                //清空actor输入
                actor_.GetUserInput().Reset();
                actor_.GetEnvirInput().Reset();

                //给予重力加速度，做碰撞检测
                actor_.GetAccVelocity().y += -0.00008f;
                actor_.UpdateVelocity(clock_.ElapsedTime());
                actor_.GetPosition() += actor_.GetVelocity() *
                                        static_cast<float>(clock_.ElapsedTime());
                if(actor_.GetPosition().y < 1.0f)
                {
                    actor_.GetPosition().y = 1.0f;
                    actor_.GetVelocity().y = 0.0f;
                    actor_.GetEnvirInput().colDown = true;
                }
                
                //读取用户操作
                if(im.IsKeyPressed(KEY_CODE::KEY_A))
                    actor_.GetUserInput().left = true;
                if(im.IsKeyPressed(KEY_CODE::KEY_D))
                    actor_.GetUserInput().right = true;
                if(im.IsKeyPressed(KEY_CODE::KEY_LSHIFT))
                    actor_.GetUserInput().shift = true;
                if(im.IsKeyPressed(KEY_CODE::KEY_SPACE))
                {
                    if(!space_)
                    {
                        actor_.GetUserInput().jump = true;
                        space_ = true;
                    }
                }
                else space_ = false;

                rc.ClearColorAndDepth();
                actor_.Draw(scale_);
                rc.DoEvents();
                rc.Present();
            }
        }
    };
}

#endif //__TEST_ACTOR_MOVING_H__
