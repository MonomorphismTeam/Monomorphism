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

private:
    void _UpdateStanding(double time);
    void _UpdateRunning(double time);
    void _UpdateJumping(double time);
    void _UpdateShifting(double time);

private:
    glm::vec2 pos_;
    glm::vec2 vel_;
    glm::vec2 accVel_;

    float runningAccVel_;  //移动时自给的水平加速度
    float floatingAccVel_; //在空中时自给的水平加速度
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
