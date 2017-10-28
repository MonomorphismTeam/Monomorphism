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
    void _LoadActorStandingAnimation(vector<Texture2D> &texSeq, vector<double> &kpSeq)
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
            kpSeq[i] = stod(config("KeyPoint" + stri));
            if(i) kpSeq[i] += kpSeq[i - 1];
        }
    }

    void _LoadActorWalkingAnimation(vector<Texture2D> &texSeq, vector<double> &kpSeq)
    {
        texSeq.clear();
        kpSeq.clear();

        ConfigureFile config;
        if(!config.Load(ACTOR_ANIMATION_WALKING_CONFIGURE))
        {
            throw OWE::FatalError(string("Failed to load configure file: ") +
                                  ACTOR_ANIMATION_WALKING_CONFIGURE);
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
            kpSeq[i] = stod(config("KeyPoint" + stri));
            if(i) kpSeq[i] += kpSeq[i - 1];
        }
    }

    void _LoadActorJumpingAnimation(vector<OWE::Texture2D> &texSeq, vector<double> &kpSeq)
    {
        texSeq.clear();
        kpSeq.clear();

        ConfigureFile config;
        if(!config.Load(ACTOR_ANIMATION_JUMPING_CONFIGURE))
        {
            throw FatalError(string("Failed to load configure file: ") +
                             ACTOR_ANIMATION_JUMPING_CONFIGURE);
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
            kpSeq[i] = stod(config("KeyPoint" + stri));
            if(i) kpSeq[i] += kpSeq[i - 1];
        }
    }
}

OWE::KEY_CODE Actor::keyLeft_  = OWE::KEY_CODE::KEY_A;
OWE::KEY_CODE Actor::keyRight_ = OWE::KEY_CODE::KEY_D;
OWE::KEY_CODE Actor::keyJump_  = OWE::KEY_CODE::KEY_SPACE;

float Actor::walkingSpeed_ = 0.005f;
float Actor::jumpingSpeed_ = 0.008f;

Actor::Actor(void)
{
    Reset();
}

Actor::~Actor(void)
{

}

void Actor::Initialize(void)
{
    //各种动作的纹理资源
    _LoadActorStandingAnimation(standingTexSeq_, standingKpSeq_);
    _LoadActorWalkingAnimation(walkingTexSeq_, walkingKpSeq_);
    _LoadActorJumpingAnimation(jumpingTexSeq_, jumpingKpSeq_);

    Reset();
}

void Actor::Reset(void)
{
    maxVelocity_ = 0.0f;
    minVelocity_ = 0.0f;

    position_ = vec2(0.0f, 0.0f);
    velocity_ = vec2(0.0f, 0.0f);
    acceleratedVelocity_ = vec2(0.0f, 0.0f);

    state_ = State::Standing;
    dir_ = Direction::Right;

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

void Actor::SetSize(const glm::vec2 &size)
{
    size_ = size;
}

glm::vec2 Actor::GetSize(void) const
{
    return size_;
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

void Actor::SetState(Actor::State state)
{
    state_ = state;
}

Actor::State Actor::GetState(void) const
{
    return state_;
}

void Actor::UpdateMoving(double time)
{
    InputManager &im = InputManager::GetInstance();
    
    State newState = state_;
    Direction newDir = dir_;
    if(_CanWalk(state_))
    {
        newState = State::Standing;
        if(im.IsKeyPressed(keyLeft_))
        {
            newState = State::Walking;
            newDir = Direction::Left;
        }
        else if(im.IsKeyPressed(keyRight_))
        {
            newState = State::Walking;
            newDir = Direction::Right;
        }
    }

    if(newState == State::Walking)
    {
        if(state_ == State::Walking && newDir == dir_) //继续播放移动动画
        {
            aniTime_ += time;
            if(aniTime_ >= walkingKpSeq_.back())
            {
                aniTime_ = 0.0f;
                aniIdx_ = 0;
            }
            else while(aniTime_ >= walkingKpSeq_[aniIdx_])
                ++aniIdx_;
        }
        else //重新开始播放移动动画
        {
            aniTime_ = 0.0f;
            aniIdx_ = 0;
        }
        velocity_ = vec2(newDir == Direction::Left ? -walkingSpeed_ : walkingSpeed_, 0.0f);
    }
    else if(newState == State::Standing)
    {
        aniTime_ = 0.0f;
        aniIdx_ = 0;
        velocity_ = vec2(0.0f);
    }

    state_ = newState;
    dir_ = newDir;
}

void Actor::UpdateJumping(double time)
{
    InputManager &im = InputManager::GetInstance();

    State newState = state_;
    Direction newDir = dir_;
    if(_CanJump(state_) && im.IsKeyPressed(keyJump_))
    {
        newState = State::Floating;
        velocity_.y += jumpingSpeed_;
        aniTime_ = 0.0f;
        aniIdx_ = 0;
    }

    if(newState == State::Floating)
    {
        if(im.IsKeyPressed(keyLeft_))
            newDir = Direction::Left;
        else if(im.IsKeyPressed(keyRight_))
            newDir = Direction::Right;
        else velocity_.x = 0.0f;
        if(newDir != dir_)
            velocity_.x = -velocity_.x;

        if(state_ == State::Floating && newDir == dir_) //继续播放动画
        {
            aniTime_ += time;
            if(aniTime_ >= jumpingKpSeq_.back())
            {
                aniTime_ = 0.0f;
                aniIdx_ = 0;
            }
            else while(aniTime_ >= jumpingKpSeq_[aniIdx_])
                ++aniIdx_;
        }
        else //重新播放动画
        {
            aniTime_ = 0.0f;
            aniIdx_ = 0;
        }
    }

    state_ = newState;
    dir_ = newDir;
}

void Actor::EndFloating(void)
{
    if(state_ == State::Floating)
    {
        state_ = State::Standing;
        aniIdx_ = 0;
        aniTime_ = 0;
        UpdateMoving(0.0f);
    }
}

void Actor::Draw(const ScreenScale &scale)
{
    vec2 LB = position_ - vec2(0.5f * size_.x, 0.0f);
    vec2 RT = position_ + vec2(0.5f * size_.x, size_.y);
    vec2 uvLB, uvRT;
    if(dir_ == Direction::Left)
    {
        uvLB = vec2(1.0f, 0.0f);
        uvRT = vec2(0.0f, 1.0f);
    }
    else
    {
        uvLB = vec2(0.0f, 0.0f);
        uvRT = vec2(1.0f, 1.0f);
    }

    switch(state_)
    {
    case State::Standing:
        ImmediateRenderer::DrawTexturedBox(
            LB, RT, uvLB, uvRT,
            standingTexSeq_[aniIdx_], scale,
            ImmediateRenderer::RenderMode::AlphaTest);
        break;
    case State::Walking:
        ImmediateRenderer::DrawTexturedBox(
            LB, RT, uvLB, uvRT,
            walkingTexSeq_[aniIdx_], scale,
            ImmediateRenderer::RenderMode::AlphaTest);
        break;
    case State::Floating:
        ImmediateRenderer::DrawTexturedBox(
            LB, RT, uvLB, uvRT,
            jumpingTexSeq_[aniIdx_], scale,
            ImmediateRenderer::RenderMode::AlphaTest);
        break;
    }
}

bool Actor::_CanWalk(State state)
{
    if(state == State::Standing ||
       state == State::Walking)
        return true;
    return false;
}

bool Actor::_CanJump(State state)
{
    if(state == State::Standing ||
       state == State::Walking)
        return true;
    return false;
}
