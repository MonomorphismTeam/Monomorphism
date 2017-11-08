/*================================================================
Filename: Scene.h
Date: 2017.11.7
Created by AirGuanZ
================================================================*/
#ifndef __SCENE_H__
#define __SCENE_H__

#include <vector>

#include <glm\glm.hpp>
#include <OWE.h>

#include "Actor.h"
#include "BlockArea.h"
#include "Creature.h"
#include "DamageArea.h"
#include "Item.h"
#include "KeyDown.h"

class Scene
{
public:
    template<typename T>
    using CollisionManager = OWE::SimpleCollisionManager<T>;
    
    Scene(void);

    void Initialize(void);
    void Run(void);

private:
    void _UpdateActor(void);

private:
    OWE::RenderContext &rc_;
    OWE::InputManager &im_;

    //时钟、屏幕坐标系和结束标志
    OWE::Clock clock_;
    OWE::ScreenScale scale_;
    bool mainLoopDone_;

    //角色
    Actor actor_;

    //角色输入管理
    KeyDown keyJump_;
    KeyDown keyAttack_;

    //场景元素集合
    std::vector<BlockArea*>  blockAreas_;
    std::vector<Creature*>   creatures_;
    std::vector<DamageArea*> damageAreas_;
    std::vector<Item*>       items_;

    //碰撞体管理
    CollisionManager<BlockArea>  blockColMgr_;
    CollisionManager<Creature>   creatureColMgr_;
    CollisionManager<DamageArea> damageColMgr_;
    CollisionManager<Item>       itemColMgr_;
};

#endif //__SCENE_H__
