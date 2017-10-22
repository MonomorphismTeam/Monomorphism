/*================================================================
Filename: Actor.cpp
Date: 2017.10.21
Created by AirGuanZ
================================================================*/
#include "Include\Actor.h"

Actor::Actor(void)
    : position_(0.0f),
      velocity_(0.0f),
      acceleratedVelocity_(0.0f),
      HP_(0.0f), onGround_(false)
{

}

Actor::~Actor(void)
{

}

glm::vec2 Actor::GetPosition(void) const
{
    return position_;
}

void Actor::SetPosition(const glm::vec2 &pos)
{
    position_ = pos;
}

glm::vec2 Actor::GetVelocity(void) const
{
    return velocity_;
}

void Actor::SetVelocity(const glm::vec2 &velocity)
{
    velocity_ = velocity;
}

glm::vec2 Actor::GetAcceleratedVelocity(void) const
{
    return acceleratedVelocity_;
}

void Actor::SetAcceleratedVelocity(const glm::vec2 &accVel)
{
    acceleratedVelocity_ = accVel;
}
