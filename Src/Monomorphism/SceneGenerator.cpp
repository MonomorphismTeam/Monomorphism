/*================================================================
Filename: SceneGeneratpr.cpp
Date: 2017.11.12
Created by AirGuanZ
================================================================*/
#include <random>

#include <glm\glm.hpp>
#include <OWE.h>

#include "Include\BackgroundArea.h"
#include "Include\DecorateArea.h"
#include "Include\NormalBlock.h"
#include "Include\NormalCreature.h"
#include "Include\SceneGenerator.h"
#include "Include\SimpleAABBBlock.h"
#include "Include\World.h"

using namespace std;
using namespace glm;
using namespace OWE;

constexpr float MIN_RIGHT_BOUND = 30.0f;
constexpr float MAX_RIGHT_BOUND = 40.0f;

//存档点场景左右边界
constexpr float SAVING_POINT_LEFT_BOUND = 0.0f;
constexpr float SAVING_POINT_RIGHT_BOUND = 15.0f;
constexpr float SAVING_POINT_MID_POINT = (SAVING_POINT_RIGHT_BOUND - SAVING_POINT_LEFT_BOUND) / 2.0f;
//存档点背景墙大小
constexpr float SAVING_POINT_BRICK_SIZE = 1.5f;
constexpr int SAVING_POINT_BRICK_CNT =
static_cast<int>((SAVING_POINT_RIGHT_BOUND - SAVING_POINT_LEFT_BOUND) /
                 SAVING_POINT_BRICK_SIZE) + 1;

void SceneGenerator::GenerateSavingPoint(Scene *scene, float *left, float *right)
{
    assert(scene && left && right);

    *left = SAVING_POINT_LEFT_BOUND;
    *right = SAVING_POINT_RIGHT_BOUND;
    scene->SetBound(*left, *right);

    //准备tiled background
    TiledTexture *texs = new TiledTexture;
    texs->Initialize(SAVING_POINT_BRICK_CNT, SAVING_POINT_BRICK_CNT / 2,
        SAVING_POINT_BRICK_SIZE, SAVING_POINT_BRICK_SIZE);
    for(int i = 0; i != texs->Width(); ++i)
    {
        for(int j = 0; j != texs->Height(); ++j)
            texs->SetTile(i, j, vec2(0.0f), vec2(1.0f), World::GetInstance().GetTextureManager().GetTexture("BackgroundBrick"));
    }
    texs->SetPosition(vec2(0.0f));
    scene->SetTiledBackground(texs);

    //准备地面
    float brickX = 0.0f;
    while(brickX <= SAVING_POINT_RIGHT_BOUND)
    {
        SimpleAABBBlock *blk = new SimpleAABBBlock(vec2(brickX, -SAVING_POINT_BRICK_SIZE),
            vec2(brickX + SAVING_POINT_BRICK_SIZE, 0.0f),
            World::GetInstance().GetTextureManager().GetTexture("Brick"));
        scene->AddBlockArea(blk);
        brickX += SAVING_POINT_BRICK_SIZE;
    }

    //God!
    DecorateArea *god = new DecorateArea(vec2(SAVING_POINT_MID_POINT - 4.5f, 0.0f), vec2(SAVING_POINT_MID_POINT + 4.5f, 8.0f),
        vec2(SAVING_POINT_MID_POINT - 8.0f, 4.0f - 8.0f), vec2(SAVING_POINT_MID_POINT + 8.0f, 4.0f + 8.0f),
        World::GetInstance().GetTextureManager().GetTexture("God"),
        World::GetInstance().GetTextureManager().GetTexture("LampLight"));
    scene->AddBlockArea(god);
}

void SceneGenerator::GenerateScene(SeedType seed, Scene *scene, float *left, float *right)
{
    assert(scene && left && right);

    RandomEngine eng(seed);
    std::uniform_real_distribution<float> rightDis(MIN_RIGHT_BOUND, MAX_RIGHT_BOUND);
    *left = 0.0f;
    *right = rightDis(eng);

    GenerateLand(&eng, scene, *left, *right);
}

void SceneGenerator::GenerateLand(RandomEngine *eng, Scene *scene, float left, float right)
{
    assert(eng && scene && left < right);

    //TODO：随便扔一堆砖头作为演示，这个得重新做
    /*constexpr float BRICK_SIZE = 1.5f;
    
    NormalBlock *brick = new NormalBlock(string("Bin\\Land\\BlackBrick.png"));
    for(float brickX = 0.0f; brickX <= right; brickX += BRICK_SIZE)
    {
        brick->AddArea(
            BoundingArea(BoundingArea::AABB(brickX, -BRICK_SIZE, brickX + BRICK_SIZE, 0.0f)),
            vec2(brickX, -BRICK_SIZE), vec2(brickX + BRICK_SIZE, 0.0f));
    }*/

    NormalBlock *brick = new NormalBlock(string("Bin\\Land\\BlackBrick.png"));
    brick->AddArea(
        BoundingArea(BoundingArea::AABB(-100.0f, -100.0f, 100.0f, 0.0f)),
        vec2(-100.0f, -100.0f), vec2(100.0f, 0.0f));

    scene->AddBlockArea(brick);



    //在中央放一只水母
    std::string ctTexs[] =
    {
        "Bin\\Creature\\Medusa.png",
        "Bin\\Creature\\MedusaAttacked.png",
        "Bin\\Creature\\MedusaDead.png"
    };
    NormalCreature *ct = new NormalCreature(9.0f, vec2(8.0f, 1.0f), vec2(10.0f, 3.0f), ctTexs, 0.0025f, 70.0f);
    scene->AddCreature(ct);

    //在中央放一个灯
    BlockArea *lamp = new DecorateArea(vec2(4.0f, 0.0f), vec2(6.0f, 7.0f), vec2(-3.0f, -2.0f), vec2(13.0f, 14.0f),
        World::GetInstance().GetTextureManager().GetTexture("Lamp"),
        World::GetInstance().GetTextureManager().GetTexture("LampLight"));
    scene->AddBlockArea(lamp);

    //在两边放门
    BlockArea *doorLeft = new DecorateArea(vec2(left, 0.0f), vec2(left + 1.0f, 2.0f),
        vec2(0.0f), vec2(1.0f),
        World::GetInstance().GetTextureManager().GetTexture("Door"),
        OWE::Texture2DView());
    BlockArea *doorRight = new DecorateArea(vec2(right - 1.0f, 0.0f), vec2(right, 2.0f),
        vec2(0.0f), vec2(1.0f),
        World::GetInstance().GetTextureManager().GetTexture("Door"),
        OWE::Texture2DView());
    scene->AddBlockArea(doorLeft);
    scene->AddBlockArea(doorRight);

    scene->SetBackgroundColor(0.5f, 0.5f, 0.5f, 1.0f);
}
