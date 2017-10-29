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
    _StateTransfer();
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
        expandingState_->Preupdate(this);
        break;
    default:
        abort();
    }
}

//表驱动的话会太大，反正这样一堆if开销可以接受
void Actor::_StateTransfer(void)
{
    switch(internalState_)
    {
    case InternalState::Unknown:
        if(envir_.strongHit)
            _EnterFloating();
        else if(envir_.normalHit)
            _EnterBeingAttacked();
        else if(input_.jumping)
            _EnterJumping();
        else if(input_.attack1 && weapon1_)
            _EnterWeaponState(weapon1_);
        else if(input_.attack2 && weapon2_)
            _EnterWeaponState(weapon2_);
        else if(input_.shifting)
            _EnterShifting();
        else if(input_.movingLeft || input_.movingRight)
            _EnterRunning();
        else
            _EnterStanding();
        break;
    case InternalState::Standing:
        if(envir_.strongHit)
            _EnterFloating();
        else if(envir_.normalHit)
            _EnterBeingAttacked();
        else if(input_.jumping)
            _EnterJumping();
        else if(input_.attack1 && weapon1_)
            _EnterWeaponState(weapon1_);
        else if(input_.attack2 && weapon2_)
            _EnterWeaponState(weapon2_);
        else if(input_.shifting)
            _EnterShifting();
        else if(input_.movingLeft || input_.movingRight)
            _EnterRunning();
        else
            _KeepStanding();
        break;
    case InternalState::Running:
        if(envir_.strongHit)
            _EnterFloating();
        else if(envir_.normalHit)
            _EnterBeingAttacked();
        else if(input_.jumping)
            _EnterJumping();
        else if(input_.attack1 && weapon1_)
            _EnterWeaponState(weapon1_);
        else if(input_.attack2 && weapon2_)
            _EnterWeaponState(weapon2_);
        else if(input_.shifting)
            _EnterShifting();
        else if(input_.movingLeft || input_.movingRight)
            _KeepRunning();
        else
            _EnterStanding();
        break;
    case InternalState::Jumping:
        if(envir_.strongHit || envir_.normalHit)
            _EnterFloating();
        else if(input_.attack1)
            _EnterWeaponState(weapon1_);
        else if(input_.attack2)
            _EnterWeaponState(weapon2_);
        else
            _KeepJumping();
        break;
    case InternalState::BeingAttacked:
        if(envir_.strongHit)
            _EnterFloating();
        else if(envir_.normalHit)
            _EnterBeingAttacked();
        else
            _KeepBeingAttacked();
        break;
    case InternalState::Floating:
        _KeepFloating();
        break;
    case InternalState::Lying:
        if(envir_.strongHit)
            _EnterFloating();
        else if(envir_.normalHit)
            _EnterLying();
        else
            _KeepLying();
        break;
    default:
        assert(internalState_ == InternalState::Other);
        assert(expandingState_);
        expandingState_->Transfer(this);
    }
}

void Actor::_EnterFloating(void)
{
    if(envir_.outVelocity.x > 0.0f)
    {
        actionTexFlip_ = true;
        dir_ = Direction::Left;
    }
    else
    {
        actionTexFlip_ = false;
        dir_ = Direction::Right;
    }
    action_.SetData(actionTexLying_, actionKpLying_, actionColLying_);
    action_.EnableLoop(true);
    action_.Restart();
}
