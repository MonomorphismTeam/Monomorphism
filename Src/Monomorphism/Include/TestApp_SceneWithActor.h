/*================================================================
Filename: TestApp_SceneWithActor.h
Date: 2017.11.8
Created by AirGuanZ
================================================================*/
#ifndef __TEST_APP_SCENE_WITH_ACTOR_H__
#define __TEST_APP_SCENE_WITH_ACTOR_H__

#include <string>

#include "NormalBlock.h"
#include "NormalCreature.h"
#include "Scene.h"

extern Scene *g_scene;

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
            for(int i = 0; i != 100; ++i)
            {
                b->AddArea(BoundingArea(BoundingArea::AABB(i * BRICK_SIZE, -BRICK_SIZE, (i + 1) * BRICK_SIZE, 0.0f)),
                    vec2(i * BRICK_SIZE, -BRICK_SIZE), vec2((i + 1) * BRICK_SIZE, 0.0f));
            }
            for(int i = 0; i != 2; ++i)
            {
                b->AddArea(BoundingArea(BoundingArea::AABB(i * BRICK_SIZE, BRICK_SIZE - 1.0f, (i + 1) * BRICK_SIZE, 2.0f * BRICK_SIZE - 1.0f)),
                    vec2(i * BRICK_SIZE, BRICK_SIZE - 1.0f),
                    vec2((i + 1) * BRICK_SIZE, 2.0f * BRICK_SIZE - 1.0f));
            }
            for(int i = 98; i != 100; ++i)
            {
                b->AddArea(BoundingArea(BoundingArea::AABB(i * BRICK_SIZE, BRICK_SIZE - 1.0f, (i + 1) * BRICK_SIZE, 2.0f * BRICK_SIZE - 1.0f)),
                    vec2(i * BRICK_SIZE, BRICK_SIZE - 1.0f),
                    vec2((i + 1) * BRICK_SIZE, 2.0f * BRICK_SIZE - 1.0f));
            }

            //放置一个creature
            std::string creatureFilepath[3] =
            {
                "TestApp_SceneWithActor/Creature1.png",
                "TestApp_SceneWithActor/Creature2.png",
                "TestApp_SceneWithActor/Creature3.png"
            };
            Creature *ct = new NormalCreature(10.0f, vec2(15.0f, 8.0f), vec2(16.0f, 10.0f), creatureFilepath, 0.005f, 10.0f);
            scene.AddCreature(ct);

            Creature *ct2 = new NormalCreature(10.0f, vec2(17.0f, 8.0f), vec2(19.0f, 10.0f), creatureFilepath, 0.005f, 10.0f);
            scene.AddCreature(ct2);

            Creature *ct3 = new NormalCreature(10.0f, vec2(20.0f, 8.0f), vec2(22.0f, 12.0f), creatureFilepath, 0.005f, 10.0f);
            scene.AddCreature(ct3);

            g_scene = &scene;

            scene.AddBlockArea(b);

            scene.Run();
        }
    };
}

#endif //__TEST_APP_SCENE_WITH_ACTOR_H__
