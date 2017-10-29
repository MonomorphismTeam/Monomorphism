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

Actor::Actor(void)
{
    expandingState_ = nullptr;
    internalState_ = InternalState::Standing;
}

const Actor::Input &Actor::GetInput(void) const
{
    return input_;
}

Actor::Input &Actor::GetInput(void)
{
    return input_;
}

const Actor::Environment &Actor::GetEnvironment(void) const
{
    return envir_;
}

Actor::Environment &Actor::GetEnvironment(void)
{
    return envir_;
}

void Actor::Update(void)
{
    _Preupdate();
}

void Actor::_Preupdate(void)
{
    switch(internalState_)
    {
    case InternalState::Unknown:
        //do nothing
        break;
    case InternalState::Standing:
    case InternalState::Running:
        if(!envir_.collisionDown)
            internalState_ = InternalState::Unknown;
        break;
    case InternalState::Floating:
        if(envir_.collisionDown)
            internalState_ = InternalState::Lying;
        break;
    case InternalState::Jumping:
        if(envir_.collisionDown)
            internalState_ = InternalState::Unknown;
        break;
    case InternalState::Shifting:
    case InternalState::BeingAttacked:
    case InternalState::Lying:
        if(action_.End())
            internalState_ = InternalState::Unknown;
        break;
    case InternalState::Other:
        expandingState_->Preupdate(*this);
        break;
    default:
        abort();
    }
}
