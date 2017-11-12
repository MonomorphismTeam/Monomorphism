/*================================================================
Filename: TestApp_SceneWithActor.h
Date: 2017.11.8
Created by AirGuanZ
================================================================*/
#ifndef __TEST_APP_SCENE_WITH_ACTOR_H__
#define __TEST_APP_SCENE_WITH_ACTOR_H__

#include <string>

#include "Scene.h"
#include "NormalBlock.h"

#include "NormalCreature.h"

namespace Test
{
    using namespace std;
    using namespace OWE;

    class TestApp_SceneWithActor
    {
    public:
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

            Scene scene;
            scene.Initialize();

            NormalBlock *b = new NormalBlock("TestApp_SceneWithActor\\Brick.png");
            constexpr float BRICK_SIZE = 2.0f;
            //添加一些砖头block
            for(int i = 0; i != 10; ++i)
            {
                b->AddArea(BoundingArea(BoundingArea::AABB(i * BRICK_SIZE, -BRICK_SIZE, (i + 1) * BRICK_SIZE, 0.0f)),
                    vec2(i * BRICK_SIZE, -BRICK_SIZE), vec2((i + 1) * BRICK_SIZE, 0.0f));
            }

            for(int i = 5; i != 10; ++i)
            {
                b->AddArea(BoundingArea(BoundingArea::AABB(i * BRICK_SIZE, BRICK_SIZE - 1.0f, (i + 1) * BRICK_SIZE, 2.0f * BRICK_SIZE - 1.0f)),
                    vec2(i * BRICK_SIZE, BRICK_SIZE - 1.0f),
                    vec2((i + 1) * BRICK_SIZE, 2.0f * BRICK_SIZE - 1.0f));
            }

            scene.AddBlockArea(b);

            scene.Run();
        }
    };
}

#endif //__TEST_APP_SCENE_WITH_ACTOR_H__
