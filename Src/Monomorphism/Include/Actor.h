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

/* Actor概要
    本身按 加速度-速度-位移 的套路来，有些操作是给加速度，有些操作是给速度
    首先是状态机：
        Unknown                 根据输入待定（在地上）
        Standing                普通的站立
        Running                 水平移动
        Shifting                翻滚
        Jumping                 跳跃
        BeingAttacked           被捶
        Floating                被捶得飞起 Orz
        Lying                   倒地
        外来状态                外界注册的状态，要求给转移函数
    可能的外界输入：
        左右键，跳跃键，调用武器
        外来运动量
            外来速度
            外来加速度
        自给运动量
            自给速度
            自给加速度

    接收键盘输入

    先做preupdate
        Floating落地进入Lying
        Jumping落地进入Unknown
        Standing、Running脚下悬空进入Jumping
        Shifting、Lying、BeingAttacked时间结束进入Unknown
        外界注册状态自己处理
    
    再做state transfer
        Unknown[被外界强力打击以致悬空]            -> Floating
        Unknown[被外界打击]                        -> BeingAttacked
        Unknown[跳跃键]                            -> Jumping，给跳跃竖直速度，继承水平速度
        Unknown[攻击键]                            -> 交给武器注册的转移函数
        Unknown[移动键]                            -> Running，给移动加速度
        Unknown[其他]                              -> Standing

        Standing[被外界强力打击以致悬空]           -> Floating
        Standing[被外界打击]                       -> BeingAttacked
        Standing[跳跃键]                           -> Jumping，给跳跃竖直速度，继承水平速度
        Standing[攻击键]                           -> 交给武器注册的转移函数
        Standing[移动键]                           -> Running，给移动加速度
        Standing[翻滚键]                           -> Shifting

        Running[被外界强力打击以致悬空]            -> Floating
        Running[被外界打击]                        -> BeingAttacked
        Running[跳跃键]                            -> Jumping，给跳跃竖直速度，继承水平速度
        Running[攻击键]                            -> 交给武器注册的状态
        Running[移动键]                            -> Running，给移动加速度
        Running[其他]                              -> Standing
        Running[翻滚键]                            -> Shifting

        Jumping[被外界强力打击/被外界打击]         -> Floating
        Jumping[攻击键]                            -> 交给武器注册的转移函数
        Jumping[移动键]                            -> Jumping, 给水平加速度

        BeingAttacked[被外界强力打击以致悬空]      -> Floating
        BeingAttacked[被外界打击]                  -> BeingAttacked，重置计时器

        Floating[被外界强力打击/被外界打击]        -> Floating

        Lying[被外界强力打击以致悬空]              -> Floating
        Lying[被外界打击]                          -> Lying，重置计时器

    最后做移动和场景碰撞检测、伤害检测等一大堆东西
*/

class Actor;

namespace _ActorAux
{
    class ActorExpandingState
    {
    public:
        virtual ~ActorExpandingState(void) { }
        virtual void Preupdate(Actor &actor) = 0;
        virtual void Transfer(Actor &actor) = 0;
    };

    enum class ActorState
    {
        Unknown,
        Standing,
        Running,
        Shifting,
        Jumping,
        BeingAttacked,
        Floating,
        Lying,
        Other
    };

    //每帧Actor会接受到的外界输入
    struct ActorEnvironment
    {
        ActorEnvironment(void)
        {
            Reset();
        }

        void Reset(void)
        {
            collisionLeft  = false;
            collisionRight = false;
            collisionUp    = false;
            collisionDown  = false;

            outVelocity     = glm::vec2(0.0f);
            outAcceVelocity = glm::vec2(0.0f);
        }

        //上下左右四个方向有没有撞墙
        bool collisionLeft;
        bool collisionRight;
        bool collisionUp;
        bool collisionDown;

        //外来速度/加速度
        glm::vec2 outVelocity;
        glm::vec2 outAcceVelocity;

    };

    //每帧Actor接收到的用户操作
    struct ActorUserInput
    {
        ActorUserInput(void)
        {
            Reset();
        }

        void Reset(void)
        {
            movingLeft  = false;
            movingRight = false;
            jumping     = false;
            shifting    = false;
        }

        bool movingLeft;
        bool movingRight;
        bool jumping;
        bool shifting;
    };
}

class Actor : public OWE::Utility::Uncopyable
{
public:
    using Action         = _ActorAux::ActorAction;          //动作动画
    using ExpandingState = _ActorAux::ActorExpandingState;  //扩展状态
    using InternalState  = _ActorAux::ActorState;           //内建状态
    using Environment    = _ActorAux::ActorEnvironment;     //环境输入
    using Input          = _ActorAux::ActorUserInput;       //用户输入

    using ActionTexRsc = std::vector<OWE::Texture2D>;       //动作纹理序列

    Actor(void);

    //输入设置
    const Input &GetInput(void) const;
    Input &GetInput(void);

    //环境设置
    const Environment &GetEnvironment(void) const;
    Environment &GetEnvironment(void);

    //状态自演
    void Update(void);

private:
    void _Preupdate(void);

private:
    //输入
    Input input_;
    Environment envir_;
    //当前状态
    ExpandingState *expandingState_;
    InternalState internalState_;
    //动作相关
    Action action_;

    Action::TexSeq  actionTexStanding_;
    Action::TexSeq  actionTexRunning_;
    Action::TexSeq  actionTexShifting_;
    Action::TexSeq  actionTexJumping_;
    Action::TexSeq  actionTexBeingAttacked_;
    Action::TexSeq  actionTexFloating_;
    Action::TexSeq  actionTexLying_;

    ActionTexRsc actionTexRscStanding_;
    ActionTexRsc actionTexRscRunning_;
    ActionTexRsc actionTexRscShifting_;
    ActionTexRsc actionTexRscJumping_;
    ActionTexRsc actionTexRscBeingAttacked_;
    ActionTexRsc actionTexRscFloating_;
    ActionTexRsc actionTexRscLying_;
};

#endif //__ACTOR_H__
