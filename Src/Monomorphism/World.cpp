/*================================================================
Filename: World.cpp
Date: 2017.11.12
Created by AirGuanZ
================================================================*/
#include "Include\BackgroundArea.h"
#include "Include\ResourceNames.h"
#include "Include\World.h"

using namespace std;
using namespace glm;
using namespace OWE;

namespace
{
    //¥Êµµµ„≥°æ∞◊Û”“±ﬂΩÁ
    constexpr float SAVING_POINT_LEFT_BOUND = 0.0f;
    constexpr float SAVING_POINT_RIGHT_BOUND = 20.0f;
    //¥Êµµµ„±≥æ∞«Ω¥Û–°
    constexpr float SAVING_POINT_BRICK_SIZE = 1.5f;
    constexpr int SAVING_POINT_BRICK_CNT =
        4 * static_cast<int>((SAVING_POINT_RIGHT_BOUND - SAVING_POINT_LEFT_BOUND) /
                             SAVING_POINT_BRICK_SIZE);

    inline SceneGenerator::SeedType _ConstructStageSeed(SceneGenerator::SeedType world, World::StageNumber stage)
    {
        return static_cast<SceneGenerator::SeedType>(world + stage);
    }
}

OWE_SINGLETON_INSTANCE_PTR(World);

void World::InitializeScene(SceneGenerator::SeedType worldSeed, StageNumber stage)
{
    assert((stage & 1) == 0 && !scene_);

    worldSeed_ = worldSeed;
    stage_ = stage;

    scene_ = new Scene;
    scene_->Initialize();
    _InitializeSavingPoint(scene_);
}

TextureManager &World::GetTextureManager(void)
{
    return texMgr_;
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

void World::_InitializeSavingPoint(Scene *scene)
{
    assert(scene);
    
    scene->SetBound(SAVING_POINT_LEFT_BOUND, SAVING_POINT_RIGHT_BOUND);

    BackgroundArea *bk = new BackgroundArea;
    TiledTexture &texs = bk->GetBackgroundTexture();
    texs.Initialize(SAVING_POINT_BRICK_CNT, SAVING_POINT_BRICK_CNT,
                    SAVING_POINT_BRICK_SIZE, SAVING_POINT_BRICK_SIZE);
    for(int i = 0; i != SAVING_POINT_BRICK_CNT; ++i)
    {
        for(int j = 0; j != SAVING_POINT_BRICK_CNT; ++j)
            texs.SetTile(i, j, vec2(0.0f), vec2(1.0f), texMgr_.GetTexture("BackgroundBrick"));
    }
}
