/*================================================================
Filename: Actor.cpp
Date: 2017.10.24
Created by AirGuanZ
================================================================*/
#include <limits>
#include <numeric>

#include <glm\glm.hpp>
#include "Include\Actor.h"
#include "Include\ConfigureFile.h"
#include "Include\ResourceNames.h"

using namespace std;
using namespace glm;
using namespace OWE;

namespace
{
    void _LoadActorStandingAnimation(vector<Texture2D> &texSeq, vector<float> &kpSeq)
    {
        texSeq.clear();
        kpSeq.clear();

        ConfigureFile config;
        if(!config.Load(ACTOR_ANIMATION_STANDING_CONFIGURE))
        {
            throw OWE::FatalError(string("Failed to load configure file: ") +
                                  ACTOR_ANIMATION_STANDING_CONFIGURE);
        }

        int cnt = stoi(config("Count"));
        texSeq.resize(cnt);
        kpSeq.resize(cnt);
        for(int i = 0; i != cnt; ++i)
        {
            string stri = to_string(i);
            string filename = config("Tex" + stri);
            if(!LoadTexture2DFromFile(filename, Texture2D::Desc(), texSeq[i]))
                throw FatalError("Failed to load texture from file: " + filename);
            kpSeq[i] = stof(config("KeyPoint") + stri);
        }
    }

    void _LoadActorWalkingAnimation(vector<Texture2D> &texSeq, vector<float> &kpSeq, vector<float> &speedSeq)
    {
        texSeq.clear();
        kpSeq.clear();
        speedSeq.clear();

        ConfigureFile config;
        if(!config.Load(ACTOR_ANIMATION_WALKING_CONFIGURE))
        {
            throw OWE::FatalError(string("Failed to load configure file: ") +
                                  ACTOR_ANIMATION_WALKING_CONFIGURE);
        }

        int cnt = stoi(config("Count"));
        texSeq.resize(cnt);
        kpSeq.resize(cnt);
        speedSeq.resize(cnt);
        for(int i = 0; i != cnt; ++i)
        {
            string stri = to_string(i);
            string filename = config("Tex" + stri);
            if(!LoadTexture2DFromFile(filename, Texture2D::Desc(), texSeq[i]))
                throw FatalError("Failed to load texture from file: " + filename);
            kpSeq[i] = stof(config("KeyPoint" + stri));
            speedSeq[i] = stof(config("Speed" + stri));
        }

        //速度归一化
        float speedFactor = 1.0f / accumulate(begin(speedSeq), end(speedSeq), numeric_limits<float>::min(),
            [](float acc, float f) { return std::max(acc, f); });
        for(float &f : speedSeq)
            f *= speedFactor;
    }
}

Actor::Actor(void)
{
    _LoadActorStandingAnimation(standingTexSeq_, standingKpSeq_);
    _LoadActorWalkingAnimation(walkingTexSeq_, walkingKpSeq_, walkingSpeedSeq_);
    Reset();
}

Actor::~Actor(void)
{

}

void Actor::Reset(void)
{
    maxVelocity_ = 0.0f;
    minVelocity_ = 0.0f;

    position_ = vec2(0.0f, 0.0f);
    velocity_ = vec2(0.0f, 0.0f);
    acceleratedVelocity_ = vec2(0.0f, 0.0f);

    HP_ = 0.0f;

    state_ = State::Unknown;
    dir_ = Direction::Right;

    weapons_.clear();
    weapons_.resize(ACTOR_WEAPON_CNT, nullptr);

    aniTime_ = 0.0f;
    aniIdx_ = 0;
}

void Actor::SetMaxVelocity(float maxVel)
{
    maxVelocity_ = maxVel;
}

float Actor::GetMaxVelocity(void) const
{
    return maxVelocity_;
}

void Actor::SetMinVelocity(float minVel)
{
    minVelocity_ = minVel;
}

float Actor::GetMinVelocity(void) const
{
    return minVelocity_;
}

void Actor::SetPosition(const vec2 &pos)
{
    position_ = pos;
}

vec2 Actor::GetPosition(void) const
{
    return position_;
}

void Actor::SetVelocity(const vec2 &vel)
{
    velocity_ = vel;
}

vec2 Actor::GetVelocity(void) const
{
    return velocity_;
}

void Actor::SetAcceleratedVelocity(const vec2 &accVel)
{
    acceleratedVelocity_ = accVel;
}

vec2 Actor::GetAcceleratedVelocity(void) const
{
    return acceleratedVelocity_;
}

Actor::State Actor::GetState(void) const
{
    return state_;
}

void Actor::SetWeapon(Weapon *weapon, int idx)
{
    assert(0 <= idx && idx < static_cast<int>(weapons_.size()));
    weapons_[idx] = weapon;
}

void Actor::AttackWithWeapon(int idx)
{
    assert(0 <= idx && idx < static_cast<int>(weapons_.size()));
    if(!weapons_[idx] || weapons_[idx]->IsBusy())
        return;

}
