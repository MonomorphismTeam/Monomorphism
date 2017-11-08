/*================================================================
Filename: TestApp_SceneWithActor.h
Date: 2017.11.8
Created by AirGuanZ
================================================================*/
#ifndef __TEST_APP_SCENE_WITH_ACTOR_H__
#define __TEST_APP_SCENE_WITH_ACTOR_H__

#include "Scene.h"

namespace Test
{
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

            //≥ı ºªØRenderContext
            RenderContext::InitInstance(desc);
            RenderContext &rc = RenderContext::GetInstance();
            InputManager &im = InputManager::GetInstance();

            Scene scene;
            scene.Initialize();
            scene.Run();
        }
    };
}

#endif //__TEST_APP_SCENE_WITH_ACTOR_H__
