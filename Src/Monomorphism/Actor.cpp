/*================================================================
Filename: Actor.cpp
Date: 2017.10.24
Created by AirGuanZ
================================================================*/
#include <limits>
#include <numeric>
#include <string>

#include <glm\glm.hpp>
#include <OWE.h>

#include "Include\Actor.h"
#include "Include\ConfigureFile.h"
#include "Include\ResourceNames.h"

using namespace std;
using namespace glm;
using namespace OWE;

namespace
{
    void _LoadActionRsc(const ConfigureFile &conf, const string &action,
                        Actor::Action::TexSeq &texSeq, Actor::Action::KpSeq &kpSeq)
    {
        texSeq.clear();
        kpSeq.clear();

        int cnt = stoi(conf(action, "Count"));
        texSeq.resize(cnt);
        kpSeq.resize(cnt);

        for(int i = 0; i != cnt; ++i)
        {
            string stri = to_string(i);
            string texFilename = conf(action, "Tex" + stri);
            if(!LoadTexture2DFromFile(texFilename, Texture2D::Desc(), texSeq[i]))
                throw FatalError("Failed to load texture from file: " + texFilename);
            kpSeq[i] = stod(conf(action, "Kp" + stri)) + (i ? kpSeq[i - 1] : 0.0);
        }
    }
}

Actor::Actor(void)
{
    pos_        = vec2(0.0f, 0.0f);
    texSize_    = vec2(1.0f, 1.0f);
    vel_        = vec2(0.0f, 0.0f);
    accVel_     = vec2(0.0f, 0.0f);
    airFricAccVel_ = vec2(0.0f, 0.0f);

    runningVel_  = 0.0f;
    floatingAccVel_ = 0.0f;

    maxFloatingVel_ = 0.0f;
    floatingFricAccVel_ = 0.0f;

    jumpingVel_     = 0.0f;
    shiftingVel_    = 0.0f;

    state_ = State::Unknown;
    dir_   = Direction::Right;

    user_.Reset();
    envir_.Reset();
}

void Actor::Initialize(void)
{
    //========= 初始化用于绘制的纹理资源 =========

    ConfigureFile conf;
    if(!conf.Load(ACTOR_ACTION_RESOURCE))
        throw FatalError(string("Failed to load configure file: ") + ACTOR_ACTION_RESOURCE);

    _LoadActionRsc(conf, "Standing", actTexStanding_, actKpStanding_);
    _LoadActionRsc(conf, "Running",  actTexRunning_,  actKpRunning_);
    _LoadActionRsc(conf, "Jumping",  actTexJumping_,  actKpJumping_);
    _LoadActionRsc(conf, "Shifting", actTexShifting_, actKpShifting_);

    conf.Clear();
}

void Actor::ClearAccVel(void)
{
    accVel_     = vec2(0.0f, 0.0f);
    airFricAccVel_ = vec2(0.0f, 0.0f);
}

Actor::UserInput &Actor::GetUserInput(void)
{
    return user_;
}

Actor::EnvirInput &Actor::GetEnvirInput(void)
{
    return envir_;
}

/*
    根据当前动作决定是否要转移到其他动作
*/
void Actor::Update(double time)
{
    switch(state_)
    {
    case State::Unknown:
        _UpdateStanding(time);
        break;
    case State::Standing:
        _UpdateStanding(time);
        break;
    case State::Running:
        _UpdateRunning(time);
        break;
    case State::Jumping:
        _UpdateJumping(time);
        break;
    case State::Shifting:
        _UpdateShifting(time);
        break;
    }
}

void Actor::UpdateVelocity(double time)
{
    float t = static_cast<float>(time);
    vel_ += t * accVel_;

    if(sign(vel_.x) != sign(airFricAccVel_.x))
    {
        if(vel_.x < 0.0f)
            vel_.x = glm::min(0.0f, vel_.x + airFricAccVel_.x * t);
        else if(vel_.x > 0.0f)
            vel_.x = glm::max(0.0f, vel_.x + airFricAccVel_.x * t);
    }

    vel_.x = clamp(vel_.x, -maxFloatingVel_, maxFloatingVel_);
}

void Actor::_UpdateStanding(double time)
{
    State oldState = state_;
    state_ = State::Standing;

    //若脚下悬空或按了跳跃键，进入跳跃姿态
    if(!envir_.colDown || user_.jump)
    {
        _UpdateJumping(time);
        return;
    }

    //若按下shift，进入闪避姿态
    if(user_.shift)
    {
        _UpdateShifting(time);
        return;
    }

    //进入移动状态
    if(user_.left || user_.right)
    {
        _UpdateRunning(time);
        return;
    }

    //是否是新进入Standing状态
    if(state_ != oldState)
    {
        act_.SetData(&actTexStanding_, &actKpStanding_);
        act_.EnableLoop(true);
        act_.Restart();
    }

    vel_.x = 0.0f;
    act_.Tick(time);
}

void Actor::_UpdateRunning(double time)
{
    State oldState = state_;
    state_ = State::Running;

    //若脚下悬空或按了跳跃键，进入跳跃姿态
    if(!envir_.colDown || user_.jump)
    {
        _UpdateJumping(time);
        return;
    }

    //若按下shift，进入闪避姿态
    if(user_.shift)
    {
        _UpdateShifting(time);
        return;
    }

    //若没按移动键，回到Standing
    if(!user_.left && !user_.right)
    {
        _UpdateStanding(time);
        return;
    }

    //本帧输入的移动方向
    Direction newDir = user_.left ? Direction::Left : Direction::Right;

    //是否算作新进入Running状态
    if(state_ != oldState || newDir != dir_)
    {
        dir_ = newDir;
        act_.SetData(&actTexRunning_, &actKpRunning_);
        act_.EnableLoop(true);
        act_.Restart();
    }

    //accVel_ += vec2((dir_ == Direction::Right ? runningAccVel_ : -runningAccVel_), 0.0f);
    vel_.x = (dir_ == Direction::Right ? runningVel_ : -runningVel_);
    act_.Tick(time);
}

void Actor::_UpdateJumping(double time)
{
    State oldState = state_;
    state_ = State::Jumping;

    //若脚下着地，进入站立姿态
    //加上!jump条件是因为主动起跳时也符合colDown条件
    if(envir_.colDown && !user_.jump)
    {
        _UpdateStanding(time);
        return;
    }

    //是否是新进入Jumping状态
    if(state_ != oldState)
    {
        act_.SetData(&actTexJumping_, &actKpJumping_);
        act_.EnableLoop(true);
        act_.Restart();

        //如果是主动跳起来的，给予竖直初速度
        if(envir_.colDown && user_.jump)
            vel_.y = jumpingVel_;
    }

    //是否在空中给予了加速度
    if(user_.left)
    {
        accVel_ += vec2(-floatingAccVel_, 0.0f);
        dir_ = Direction::Left;
    }
    else if(user_.right)
    {
        accVel_ += vec2(floatingAccVel_, 0.0f);
        dir_ = Direction::Right;
    }
    else //没按键，接受阻力的制裁
    {
        if(vel_.x > 0.0f)
            airFricAccVel_ += vec2(-floatingFricAccVel_, 0.0f);
        else if(vel_.x < 0.0f)
            airFricAccVel_ += vec2(floatingFricAccVel_, 0.0f);
    }

    act_.Tick(time);
}

void Actor::_UpdateShifting(double time)
{
    State oldState = state_;
    state_ = State::Shifting;

    //时间结束，自动转移为Standing状态
    if(act_.End())
    {
        act_.Restart();
        _UpdateStanding(time);
        return;
    }

    //是否是新进入Shifting状态的
    if(state_ != oldState)
    {
        //刚进Shifting时还有改方向的机会
        if(user_.left)
            dir_ = Direction::Left;
        else if(user_.right)
            dir_ = Direction::Right;

        act_.SetData(&actTexShifting_, &actKpShifting_);
        act_.EnableLoop(false);
        act_.Restart();
    }

    //给翻滚速度，不管还在不在地上
    vel_.x = (dir_ == Direction::Left ? -shiftingVel_ : shiftingVel_);

    act_.Tick(time);
}

void Actor::Draw(const ScreenScale &scale)
{
    switch(state_)
    {
    case State::Standing:
        _DrawStanding(scale);
        break;
    case State::Running:
        _DrawRunning(scale);
        break;
    case State::Jumping:
        _DrawJumping(scale);
        break;
    case State::Shifting:
        _DrawShifting(scale);
        break;
    default:
        abort();
    }
}

void Actor::_DrawNormalAction(const ScreenScale &scale)
{
    vec2 LB = pos_ - vec2(texSize_.x / 2.0f, 0.0f);
    vec2 RT = pos_ + vec2(texSize_.x / 2.0f, texSize_.y);
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
    ImmediateRenderer::DrawTexturedBox(
        LB, RT, uvLB, uvRT,
        act_.CurrentTex(), scale,
        ImmediateRenderer::RenderMode::AlphaTest);
}

void Actor::_DrawStanding(const ScreenScale &scale)
{
    _DrawNormalAction(scale);
}

void Actor::_DrawRunning(const ScreenScale &scale)
{
    _DrawNormalAction(scale);
}

void Actor::_DrawJumping(const ScreenScale &scale)
{
    vec2 LB = pos_ - vec2(texSize_.x / 2.0f, 0.0f);
    vec2 RT = pos_ + vec2(texSize_.x / 2.0f, texSize_.y);
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
    Texture2DView tex;
    if(vel_.y >= 0.0f)
        tex = actTexJumping_[0];
    else
        tex = actTexJumping_[1];
    ImmediateRenderer::DrawTexturedBox(
        LB, RT, uvLB, uvRT,
        tex, scale,
        ImmediateRenderer::RenderMode::AlphaTest);
}

void Actor::_DrawShifting(const ScreenScale &scale)
{
    _DrawNormalAction(scale);
}

vec2 &Actor::GetPosition(void)
{
    return pos_;
}

vec2 &Actor::GetTexSize(void)
{
    return texSize_;
}

vec2 &Actor::GetVelocity(void)
{
    return vel_;
}

vec2 &Actor::GetAccVelocity(void)
{
    return accVel_;
}

void Actor::SetRunningVel(float accVel)
{
    runningVel_ = accVel;
}

void Actor::SetFloatingAccVel(float accVel)
{
    floatingAccVel_ = accVel;
}

void Actor::SetJumpingVel(float vel)
{
    jumpingVel_ = vel;
}

void Actor::SetShiftingVel(float vel)
{
    shiftingVel_ = vel;
}

void Actor::SetMaxFloatingVel(float vel)
{
    maxFloatingVel_ = vel;
}

void Actor::SetFloatingFricAccVel(float accVel)
{
    floatingFricAccVel_ = accVel;
}
