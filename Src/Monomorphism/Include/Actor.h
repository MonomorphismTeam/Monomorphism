/*================================================================
Filename: Actor.h
Date: 2017.10.21
Created by AirGuanZ
================================================================*/
#ifndef __ACTOR_H__
#define __ACTOR_H__

#include <vector>

#include <OWE.h>
#include "Weapon.h"

/* Actor概要
    本身按 加速度-速度-位移 的套路来，有些操作是给加速度，有些操作是给速度
    首先是状态机：
        Unknown                 根据输入待定（在地上）
        Standing                普通的站立
        Running                 水平移动
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
            外界打击速度
            外界打击加速度
        自给运动量
            自给速度
            自给加速度

    先做自动转移
        Floating落地进入Lying
        Jumping落地进入Unknown
        BeingAttacked结束进入Unknown
        Standing、Running脚下悬空进入Jumping
        外界注册状态自己处理
    
    再做主动转移
        Unknown[被外界强力打击以致悬空]            -> Floating
        Unknown[被外界打击]                        -> BeingAttacked
        Unknown[跳跃键]                            -> Jumping，给跳跃竖直速度，继承水平速度
        Unknown[攻击键]                            -> 交给武器注册的转移函数
        Unknown[移动键]                            -> Running，给移动加速度

        Standing[被外界强力打击以致悬空]           -> Floating
        Standing[被外界打击]                       -> BeingAttacked
        Standing[跳跃键]                           -> Jumping，给跳跃竖直速度，继承水平速度
        Standing[攻击键]                           -> 交给武器注册的转移函数
        Standing[移动键]                           -> Running，给移动加速度

        Running[被外界强力打击以致悬空]            -> Floating
        Running[被外界打击]                        -> BeingAttacked
        Running[跳跃键]                            -> Jumping，给跳跃竖直速度，继承水平速度
        Running[攻击键]                            -> 交给武器注册的状态
        Running[移动键]                            -> Running，给移动加速度
        Running[其他]                              -> Standing

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

class Actor : public OWE::Utility::Uncopyable
{
public:
    
private:

};

#endif //__ACTOR_H__
