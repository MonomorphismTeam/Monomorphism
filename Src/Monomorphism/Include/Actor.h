/*================================================================
Filename: Actor.h
Date: 2017.10.21
Created by AirGuanZ
================================================================*/

/* actor、weapon大概这么设计：
    actor有本身的动画，也有根据weapon而变的动画
    把actor的动画交给weapon会让actor的扩展非常不方便
    所以应该是actor提供有限种可供选择的用weapon的动画，weapon告知actor应该用哪个
    对weapon的形态和位置变换可以提供仿射变换，变换矩阵是actor交给weapon来画的
*/

#ifndef __ACTOR_H__
#define __ACTOR_H__

#include <vector>

#include <OWE.h>
#include "Weapon.h"

constexpr int ACTOR_WEAPON_CNT = 2;

class Actor : public OWE::Utility::Uncopyable
{
public:
    enum class State
    {
        Unknown,
        Standing,
        Walking,
        Floating,
        Attacking,
        Shifting,
        BeingAttacked
    };

    enum class Direction
    {
        Left, Right
    };

    Actor(void);
    ~Actor(void);

    void Initialize(void);
    void Reset(void);

    void SetMaxVelocity(float maxVel);
    float GetMaxVelocity(void) const;

    void SetMinVelocity(float minVel);
    float GetMinVelocity(void) const;

    void SetPosition(const glm::vec2 &pos);
    glm::vec2 GetPosition(void) const;

    void SetSize(const glm::vec2 &size);
    glm::vec2 GetSize(void) const;

    void SetVelocity(const glm::vec2 &vel);
    glm::vec2 GetVelocity(void) const;

    void SetAcceleratedVelocity(const glm::vec2 &pos);
    glm::vec2 GetAcceleratedVelocity(void) const;

    void SetState(State state);
    State GetState(void) const;

    void UpdateMoving(double deltaTime);
    void UpdateJumping(double deltaTime);

    void Draw(const OWE::ScreenScale &scale);

private:
    static bool _CanWalk(State state);
    static bool _CanJump(State state);

    float maxVelocity_;
    float minVelocity_;

    glm::vec2 position_;
    glm::vec2 size_;

    glm::vec2 velocity_;
    glm::vec2 acceleratedVelocity_;

    State state_;
    Direction dir_;

    //动画计时器
    double aniTime_;
    int aniIdx_;

    //每个动画分为texSeq和kpSeq两部分
    //texSeq给出一个动画帧序列，人物位于画面中央下部
    //kpSeq[i]给出第i帧的结束时间

    //站立动画
    std::vector<OWE::Texture2D> standingTexSeq_;
    std::vector<double> standingKpSeq_;

    //行走动画
    std::vector<OWE::Texture2D> walkingTexSeq_;
    std::vector<double> walkingKpSeq_;

    //跳跃动画
    std::vector<OWE::Texture2D> jumpingTexSeq_;
    std::vector<double> jumpingKpSeq_;

    static OWE::KEY_CODE keyLeft_;
    static OWE::KEY_CODE keyRight_;
    static OWE::KEY_CODE keyJump_;

    //行走速度
    static float walkingSpeed_;
    static float jumpingSpeed_;
};

#endif //__ACTOR_H__
