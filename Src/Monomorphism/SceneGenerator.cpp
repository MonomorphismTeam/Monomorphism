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

constexpr float MIN_RIGHT_BOUND = 60.0f;
constexpr float MAX_RIGHT_BOUND = 70.0f;

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

void SceneGenerator::GenerateScene(SeedType seed, Scene *scene, SceneGenerator::StageNumber stage, float *left, float *right)
{
    assert(scene && left && right);

    RandomEngine eng(seed);
    std::uniform_real_distribution<float> rightDis(MIN_RIGHT_BOUND, MAX_RIGHT_BOUND);
    *left = 0.0f;
    *right = rightDis(eng);

    GenerateLand(&eng, scene, stage, *left, *right);
}

void SceneGenerator::GenerateLand(RandomEngine *eng, Scene *scene, SceneGenerator::StageNumber stage, float left, float right)
{
    assert(eng && scene && left < right);

    NormalBlock *brick = new NormalBlock(string("Bin\\Land\\BlackBrick.png"));
    brick->AddArea(
        BoundingArea(BoundingArea::AABB(-100.0f, -100.0f, 100.0f, 0.0f)),
        vec2(-100.0f, -100.0f), vec2(100.0f, 0.0f));

    scene->AddBlockArea(brick);

    //随机放一些水母
    {
        std::string medusaTexs[] =
        {
            "Bin\\Creature\\Medusa.png",
            "Bin\\Creature\\MedusaAttacked.png",
            "Bin\\Creature\\MedusaDead.png"
        };
        std::uniform_int_distribution<int> medusaDis(1 + (stage / 2), 1 + 2 * (stage - 1));
        std::uniform_real_distribution<float> medusaPosDis(0.0f, 1.0f);
        int medusaCnt = medusaDis(*eng);
        for(int i = 0; i != medusaCnt; ++i)
        {
            glm::vec2 LB(left + 10.0f + medusaPosDis(*eng) * (right - 20.0f - left), 5.0f);
            float medusaSize = 1.0f + medusaPosDis(*eng);
            glm::vec2 RT = LB + medusaSize * vec2(2.0f, 2.0f);
            NormalCreature *ct = new NormalCreature(medusaSize * 60.0f, LB, RT, medusaTexs, 0.003f / medusaSize, 40.0f * medusaSize);
            scene->AddCreature(ct);
        }
    }

    //放置slime
    {
        std::string slimeTexs[] =
        {
            "Bin\\Creature\\Slime.png",
            "Bin\\Creature\\Slimeattacked.png",
            "Bin\\Creature\\MedusaDead.png"
        };
        std::uniform_int_distribution<int> slimeDis(1, stage);
        std::uniform_real_distribution<float> slimeSizeDis(0.0f, 1.0f);
        int slimeCnt = slimeDis(*eng);
        for(int i = 0; i != slimeCnt; ++i)
        {
            glm::vec2 LB(left + 10.0f + slimeSizeDis(*eng) * (right - 20.0f - left), 5.0f);
            float slimeSize = 1.0f + 2.0f * slimeSizeDis(*eng);
            glm::vec2 RT = LB + slimeSize * vec2(1.0f, 0.4f);
            NormalCreature *ct = new NormalCreature(slimeSize * 50.0f, LB, RT, slimeTexs, 0.001f / slimeSize, 10.0f * slimeSize);
            scene->AddCreature(ct);
        }
    }

    //在两边放灯
    {
        BlockArea *lampLeft = new DecorateArea(vec2(left + 4.0f, 0.0f), vec2(left + 6.0f, 7.0f),
            vec2(left - 3.0f, -2.0f), vec2(left + 13.0f, 14.0f),
            World::GetInstance().GetTextureManager().GetTexture("Lamp"),
            World::GetInstance().GetTextureManager().GetTexture("LampLight"));

        BlockArea *lampRight = new DecorateArea(vec2(right - 6.0f, 0.0f), vec2(right - 4.0f, 7.0f),
            vec2(right - 13.0f, -2.0f), vec2(right + 3.0f, 14.0f),
            World::GetInstance().GetTextureManager().GetTexture("Lamp"),
            World::GetInstance().GetTextureManager().GetTexture("LampLight"));

        scene->AddBlockArea(lampLeft);
        scene->AddBlockArea(lampRight);
    }

    //在两边放门
    {
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
    }

    //放一些草
    {
        uniform_real_distribution<float> grassDis(left - 5.0f, right + 5.0f);
        uniform_int_distribution<int> grassCntDis(50, 80);
        uniform_int_distribution<int> grassTypeDis(0, 3);
        int grassCnt = grassCntDis(*eng);
        for(int i = 0; i != grassCnt; ++i)
        {
            glm::vec2 LB = vec2(grassDis(*eng), -0.1f);
            glm::vec2 RT = LB + vec2(1.5f, 1.0f);
            int type = grassTypeDis(*eng);
            BlockArea *g = new DecorateArea(LB, RT, vec2(0.0f), vec2(1.0f),
                World::GetInstance().GetTextureManager().GetTexture("Grass" + to_string(type)),
                OWE::Texture2DView());
            scene->AddBlockArea(g);
        }
    }

    //放一些栅栏
    {
        uniform_real_distribution<float> fenceDis(left, right - 3.0f);
        uniform_int_distribution<int> fenceCntDis(1, 7);
        int fenceCnt = fenceCntDis(*eng);
        for(int i = 0; i != fenceCnt; ++i)
        {
            glm::vec2 LB = vec2(fenceDis(*eng), 0.0f);
            glm::vec2 RT = LB + vec2(3.0f, 1.5f);
            BlockArea *g = new DecorateArea(LB, RT, vec2(0.0f), vec2(1.0f),
                World::GetInstance().GetTextureManager().GetTexture("Fence"),
                OWE::Texture2DView());
            scene->AddBlockArea(g);
        }
    }

    //放一些墓碑
    {
        uniform_real_distribution<float> fenceDis(left, right - 10.0f);
        uniform_int_distribution<int> fenceCntDis(1, 4);
        uniform_real_distribution<float> fenceSizeDis(0.0f, 1.0f);
        int fenceCnt = fenceCntDis(*eng);
        for(int i = 0; i != fenceCnt; ++i)
        {
            glm::vec2 LB = vec2(fenceDis(*eng), 0.0f);
            glm::vec2 RT = LB + vec2(1.0f, 2.0f) * (0.7f + 0.8f * fenceSizeDis(*eng));
            BlockArea *g = new DecorateArea(LB, RT, vec2(0.0f), vec2(1.0f),
                World::GetInstance().GetTextureManager().GetTexture("TombStone1"),
                OWE::Texture2DView());
            scene->AddBlockArea(g);
        }

        //中间放一个大墓碑
        glm::vec2 LB = vec2(fenceDis(*eng), 0.0f);
        glm::vec2 RT = LB + vec2(6.0f + 6.0f * fenceSizeDis(*eng));
        BlockArea *bt = new DecorateArea(LB, RT, vec2(0.0f), vec2(1.0f),
            World::GetInstance().GetTextureManager().GetTexture("TombStone0"),
            OWE::Texture2DView());
        scene->AddBlockArea(bt);
    }

    //放点树
    {
        uniform_real_distribution<float> treeDis(left, right - 10.0f);
        uniform_real_distribution<float> treeSizeDis(0.0f, 1.0f);
        glm::vec2 LB = vec2(treeDis(*eng), 0.0f);
        glm::vec2 RT = LB + (12.0f + 5.0f * treeSizeDis(*eng)) * vec2(1.0f);
        BlockArea *bt = new DecorateArea(LB, RT, vec2(0.0f), vec2(1.0f),
            World::GetInstance().GetTextureManager().GetTexture("ScaryTree"),
            OWE::Texture2DView());
        scene->AddBlockArea(bt);
    }

    scene->SetBackgroundColor(0.7f, 0.7f, 0.7f, 1.0f);
}
