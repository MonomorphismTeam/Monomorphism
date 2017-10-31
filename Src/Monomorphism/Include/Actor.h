/*================================================================
Filename: Actor.h
Date: 2017.10.21
Created by AirGuanZ
================================================================*/
#ifndef __ACTOR_H__
#define __ACTOR_H__

#include <vector>

#include <OWE.h>

#include "ActorAction.h"
#include "Weapon.h"

namespace _ActorAux
{
    enum class ActorInternalState
    {
        Unknown,
        Standing,
        Running,
        Jumping,
        Shifting
    };

    enum class ActorDirection
    {
        Left,
        Right
    };

    struct ActorUserInput
    {
        ActorUserInput(void)
        {
            Reset();
        }

        void Reset(void)
        {
            left  = false;
            right = false;
            jump  = false;
            shift = false;
        }

        bool left;
        bool right;
        bool jump;
        bool shift;
    };

    struct ActorEnvirInput
    {
        ActorEnvirInput(void)
        {
            Reset();
        }

        void Reset(void)
        {
            colLeft  = false;
            colRight = false;
            colUp    = false;
            colDown  = false;
        }

        bool colLeft;
        bool colRight;
        bool colUp;
        bool colDown;
    };
}

/* 状态更新流程：
    actor_.Update();
    重置环境输入和用户输入
    速度、加速度演化
    备份位置，碰撞检测，改速度，恢复碰撞的位置，改环境输入
    记录用户输入
*/
class Actor
{
public:
    using Action = _ActorAux::ActorAction;
    using State = _ActorAux::ActorInternalState;
    using Direction = _ActorAux::ActorDirection;
    using UserInput = _ActorAux::ActorUserInput;
    using EnvirInput = _ActorAux::ActorEnvirInput;

    Actor(void);

    void Initialize(void);

    UserInput &GetUserInput(void);
    EnvirInput &GetEnvirInput(void);

    void Update(double time);

    void Draw(const OWE::ScreenScale &scale);

    glm::vec2 &GetPosition(void);
    glm::vec2 &GetTexSize(void);
    glm::vec2 &GetVelocity(void);
    glm::vec2 &GetAccVelocity(void);

    void SetRunningVel(float Vel);       //移动时自给的水平加速度
    void SetFloatVel(float accVel);   //在空中时自给的水平加速度
    void SetJumpingVel(float vel);       //跳跃竖直方向带来的初速度
    void SetShiftingVel(float vel);      //闪避速度

private:
    void _UpdateStanding(double time);
    void _UpdateRunning(double time);
    void _UpdateJumping(double time);
    void _UpdateShifting(double time);

    void _DrawNormalAction(const OWE::ScreenScale &scale);
    void _DrawStanding(const OWE::ScreenScale &scale);
    void _DrawRunning(const OWE::ScreenScale &scale);
    void _DrawJumping(const OWE::ScreenScale &scale);
    void _DrawShifting(const OWE::ScreenScale &scale);

private:
    glm::vec2 pos_;
    glm::vec2 texSize_;
    glm::vec2 vel_;
    glm::vec2 accVel_;

    float runningVel_;  //移动时自给的水平加速度
    float floatingVel_; //在空中时自给的水平加速度
    float jumpingVel_;     //跳跃竖直方向带来的初速度
    float shiftingVel_;    //闪避速度

    State state_;
    Direction dir_;
    
    UserInput user_;
    EnvirInput envir_;

    Action act_;
    
    //各种动作资源

    Action::TexSeq actTexStanding_;
    Action::KpSeq  actKpStanding_;

    Action::TexSeq actTexRunning_;
    Action::KpSeq  actKpRunning_;

    Action::TexSeq actTexJumping_;
    Action::KpSeq  actKpJumping_;

    Action::TexSeq actTexShifting_;
    Action::KpSeq  actKpShifting_;
};

#endif //__ACTOR_H__
