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
        Shifting,
        AttackingWithSword,
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
            left   = false;
            right  = false;
            jump   = false;
            shift  = false;
            attack = false;
        }

        bool left;
        bool right;
        bool jump;
        bool shift;
        bool attack;
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
    重置加速度
    actor_.Update();
    叠加外界加速度
    actor_.UpdateVelocity();
    重置环境输入和用户输入
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

    using TransSeq = std::vector<glm::mat3>;

    Actor(void);

    void Initialize(void);
    void ClearAccVel(void);

    UserInput &GetUserInput(void);
    EnvirInput &GetEnvirInput(void);

    void Update(double time);
    void UpdateVelocity(double time);

    void Draw(const OWE::ScreenScale &scale);

    glm::vec2 &GetPosition(void);
    glm::vec2 &GetTexSize(void);
    glm::vec2 &GetVelocity(void);
    glm::vec2 &GetAccVelocity(void);

    void SetRunningVel(float Vel);        //移动时自给的水平加速度
    void SetFloatingAccVel(float accVel); //在空中时自给的水平加速度
    void SetJumpingVel(float vel);        //跳跃竖直方向带来的初速度
    void SetShiftingVel(float vel);       //闪避速度

    void SetMaxFloatingVel(float vel);
    void SetFloatingFricAccVel(float accVel);

    void SetWeapon(Weapon *weapon);

private:
    void _UpdateStanding(double time);
    void _UpdateRunning(double time);
    void _UpdateJumping(double time);
    void _UpdateShifting(double time);
    void _UpdateAttackingWithSword(double time);

    void _DrawNormalAction(const OWE::ScreenScale &scale);
    void _DrawStanding(const OWE::ScreenScale &scale);
    void _DrawRunning(const OWE::ScreenScale &scale);
    void _DrawJumping(const OWE::ScreenScale &scale);
    void _DrawShifting(const OWE::ScreenScale &scale);
    void _DrawAttackingWithSword(const OWE::ScreenScale &scale);

private:
    glm::vec2 pos_;
    glm::vec2 texSize_;
    glm::vec2 vel_;
    glm::vec2 accVel_;

    glm::vec2 airFricAccVel_; //阻力加速度

    /*
        地面移动直接给速度
        空中有阻力，移动由加速度给出
    */

    float runningVel_;          //水平移动速度
    float floatingAccVel_;      //空中移动加速度

    float maxFloatingVel_;      //最大空中水平移动速度
    float floatingFricAccVel_;  //空中阻力加速度
    
    float jumpingVel_;          //跳跃竖直方向带来的初速度
    float shiftingVel_;         //闪避速度

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

    Action::TexSeq actTexAttackingWithSword_;
    Action::KpSeq  actKpAttackingWithSword_;

    //携带的武器

    Weapon *weapon_;

    //武器的仿射变换

    TransSeq weaponTransAttackingWithSword_;

    //攻击行为是否发生在跳跃时

    bool attackWhenFloating_;
};

#endif //__ACTOR_H__
