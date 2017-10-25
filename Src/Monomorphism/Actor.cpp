/*================================================================
Filename: Actor.cpp
Date: 2017.10.24
Created by AirGuanZ
================================================================*/
#include <glm\glm.hpp>
#include "Include\Actor.h"

using namespace glm;

Actor::Actor(void)
{
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

    clock_.Restart();
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

void Actor::SetPosition(const glm::vec2 &pos)
{
    position_ = pos;
}

glm::vec2 Actor::GetPosition(void) const
{
    return position_;
}

void Actor::SetVelocity(const glm::vec2 &vel)
{
    velocity_ = vel;
}

glm::vec2 Actor::GetVelocity(void) const
{
    return velocity_;
}

void Actor::SetAcceleratedVelocity(const glm::vec2 &accVel)
{
    acceleratedVelocity_ = accVel;
}

glm::vec2 Actor::GetAcceleratedVelocity(void) const
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
