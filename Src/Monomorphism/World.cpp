/*================================================================
Filename: World.cpp
Date: 2017.11.12
Created by AirGuanZ
================================================================*/
#include <fstream>

#include "Include\BackgroundArea.h"
#include "Include\SimpleAABBBlock.h"
#include "Include\ResourceNames.h"
#include "Include\World.h"

using namespace std;
using namespace glm;
using namespace OWE;

namespace
{
    inline SceneGenerator::SeedType _ConstructStageSeed(SceneGenerator::SeedType world, World::StageNumber stage)
    {
        return static_cast<SceneGenerator::SeedType>(world + stage);
    }
}

OWE_SINGLETON_INSTANCE_PTR(World);

TextureManager &World::GetTextureManager(void)
{
    return texMgr_;
}

void World::InitializeScene(SceneGenerator::SeedType worldSeed, StageNumber stage)
{
    assert((stage & 1) == 0 && !scene_);

    worldSeed_ = worldSeed;
    stage_ = stage;

    scene_ = new Scene;
    scene_->Initialize();
    SceneGenerator::GenerateSavingPoint(scene_, &leftBound_, &rightBound_);
}

void World::Run(void)
{
    assert(scene_);

    while(true)
    {
        Scene::RunningResult rt = scene_->Run();

        //以关闭的方式离开场景
        if(rt == Scene::RunningResult::Closed)
            break;

        //关卡转移
        stage_ += (rt == Scene::RunningResult::OutOfRightBound ? 1 : -1);
        stage_ = std::max(stage_, 0);

        delete scene_;
        scene_ = new Scene;
        scene_->Initialize();
        if(stage_ & 1)
            SceneGenerator::GenerateScene(worldSeed_, scene_, stage_, &leftBound_, &rightBound_);
        else
        {
            _Save();
            SceneGenerator::GenerateSavingPoint(scene_, &leftBound_, &rightBound_);
        }
        scene_->SetBound(leftBound_, rightBound_);

        if(rt == Scene::RunningResult::OutOfLeftBound)
            scene_->GetActor().GetPosition() = vec2(rightBound_ - 1.5f, 1e-2f);
        else
            scene_->GetActor().GetPosition() = vec2(leftBound_ + 1.5f, 1e-2f);
    }

    delete scene_;
    scene_ = nullptr;
}

Scene &World::GetCurrentScene(void)
{
    assert(scene_);
    return *scene_;
}

World::World(void)
    : scene_(nullptr)
{
    _InitializeResources();
}

void World::_InitializeResources(void)
{
    texMgr_.Clear();
    texMgr_.Initialize(GLOBAL_TEXTURE_RESOURCE);
}

void World::_Save(void)
{
    std::ofstream fout(SAVE_FILENAME, std::ofstream::trunc);
    if(!fout)
        throw OWE::FatalError("Failed to save progress into file: " + std::string(SAVE_FILENAME));
    fout << worldSeed_ << " " << stage_;
    fout.close();
}
