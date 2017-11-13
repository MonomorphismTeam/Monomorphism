/*================================================================
Filename: SceneGeneratpr.cpp
Date: 2017.11.12
Created by AirGuanZ
================================================================*/
#include <random>

#include <glm\glm.hpp>
#include <OWE.h>

#include "Include\NormalBlock.h"
#include "Include\SceneGenerator.h"

using namespace std;
using namespace glm;
using namespace OWE;

constexpr float MIN_RIGHT_BOUND = 150.0f;
constexpr float MAX_RIGHT_BOUND = 250.0f;

void SceneGenerator::GenerateSavingPoint(Scene *scene, float *left, float *right)
{
    assert(scene && left && right);

    //关于存档点的各种常量
    constexpr float LEFT_BOUND = 0.0f;
    constexpr float RIGHT_BOUND = 20.0f;

    //TODO
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
    constexpr float BRICK_SIZE = 1.5f;
    
    NormalBlock *brick = new NormalBlock(string("Bin\\Land\\Brick.png"));
    for(float brickX = 0.0f; brickX < right; brickX += BRICK_SIZE)
    {
        brick->AddArea(
            BoundingArea(BoundingArea::AABB(brickX, -BRICK_SIZE, brickX + BRICK_SIZE, 0.0f)),
            vec2(brickX, -BRICK_SIZE), vec2(brickX + BRICK_SIZE, 0.0f));
        brickX += BRICK_SIZE;
    }

    scene->AddBlockArea(brick);
}
