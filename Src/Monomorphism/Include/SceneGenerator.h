/*================================================================
Filename: SceneGeneratpr.h
Date: 2017.11.12
Created by AirGuanZ
================================================================*/
#ifndef __SCENE_GENERATOR_H__
#define __SCENE_GENERATOR_H__

#include <random>
#include "Scene.h"

class SceneGenerator
{
public:
    using StageNumber = int;
    using RandomEngine = std::minstd_rand;
    using SeedType = RandomEngine::result_type;

    static void GenerateSavingPoint(Scene *scene, float *left, float *right);
    static void GenerateScene(SeedType seed, Scene *scene, StageNumber stage, float *left, float *right);

private:
    static void GenerateLand(RandomEngine *eng, Scene *scene, StageNumber stage, float left, float right);
};

#endif //__SCENE_GENERATOR_H__
