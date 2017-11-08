/*================================================================
Filename: Scene.cpp
Date: 2017.11.7
Created by AirGuanZ
================================================================*/
#include <glm\glm.hpp>
#include <OWE.h>

#include "Include\Scene.h"
#include "Include\Sword.h"

using namespace glm;
using namespace OWE;

//==================================== 角色参数 ====================================

constexpr float ACTOR_RUNNING_VEL           = 0.012f;       //移动速度
constexpr float ACTOR_FLOATING_ACC_VEL      = 0.00006f;     //悬空时自给水平加速度
constexpr float ACTOR_SHIFTING_VEL          = 0.02f;        //闪避速度
constexpr float ACTOR_JUMPING_VEL           = 0.025f;       //跳跃初速度
constexpr float ACTOR_MAX_FLOATING_VEL      = 0.012f;       //悬空时最大水平速度
constexpr float ACTOR_FLOATING_FRIC_ACC_VEL = 0.00003f;     //悬空时空气阻力带来的水平加速度
constexpr float ACTOR_GRAVITY_ACC           = -0.00008f;    //重力加速度

constexpr KEY_CODE ACTOR_INPUT_KEY_MOVE_LEFT     = KEY_CODE::KEY_A;             //向左移动
constexpr KEY_CODE ACTOR_INPUT_KEY_MOVE_RIGHT    = KEY_CODE::KEY_D;             //向右移动
constexpr KEY_CODE ACTOR_INPUT_KEY_JUMP          = KEY_CODE::KEY_SPACE;         //跳跃
constexpr KEY_CODE ACTOR_INPUT_KEY_SHIFT         = KEY_CODE::KEY_LSHIFT;        //闪避
constexpr MOUSE_BUTTON ACTOR_INPUT_BUTTON_ATTACK = MOUSE_BUTTON::BUTTON_LEFT;   //攻击

//==================================================================================

Scene::Scene(void)
    : rc_(RenderContext::GetInstance()),
      im_(InputManager::GetInstance())
{
    
}

void Scene::Initialize(void)
{
    actor_.Initialize();
    scale_.Reinit(35.0f, 35.0f);
    
    //初始化角色参数
    actor_.SetRunningVel        (ACTOR_RUNNING_VEL);
    actor_.SetFloatingAccVel    (ACTOR_FLOATING_ACC_VEL);
    actor_.SetShiftingVel       (ACTOR_SHIFTING_VEL);
    actor_.SetJumpingVel        (ACTOR_JUMPING_VEL);

    actor_.SetMaxFloatingVel    (ACTOR_MAX_FLOATING_VEL);
    actor_.SetFloatingFricAccVel(ACTOR_FLOATING_FRIC_ACC_VEL);

    actor_.GetPosition() = vec2(3.0f, 2.0f);
    actor_.GetTexSize()  = vec2(0.02f, 0.02f);

    actor_.SetWeapon(new Sword());
}

void Scene::Run(void)
{
    mainLoopDone_ = false;
    clock_.Restart();
    while(!mainLoopDone_)
    {
        clock_.Tick();
        
        _UpdateActor();
        
        //场景渲染
        rc_.ClearColorAndDepth();
        
        actor_.Draw(scale_);

        rc_.Present();
        
        //外部输入处理
        rc_.DoEvents();
        
        if(im_.IsKeyPressed(KEY_CODE::KEY_ESCAPE))
            mainLoopDone_ = true;
    }
}

void Scene::_UpdateActor(void)
{
    actor_.ClearAccVel();
    actor_.Update(clock_.ElapsedTime());
    
    //清空角色输入
    actor_.ResetInput();
    
    //给予重力加速度并更新速度
    actor_.GetAccVelocity().y += ACTOR_GRAVITY_ACC;
    actor_.UpdateVelocity(clock_.ElapsedTime());

    //某个新的角色位置是否会发生碰撞
    auto testNewPos = [&](const vec2 &newPos) -> bool
    {
        actor_.GetPosition() = newPos;
        for(auto &area : actor_.GetBoundingAreas())
        {
            if(!blockColMgr_.CollisionWithBoundingArea(area).empty())
                return false;
        }
        return true;
    };

    //向无碰撞的方向移动
    vec2 oldPos    = actor_.GetPosition();
    vec2 deltaPos  = static_cast<float>(clock_.ElapsedTime()) * actor_.GetVelocity();
    if(!testNewPos(oldPos + deltaPos)) //新位置不行，考虑恢复x或y坐标
    {
        if(testNewPos(oldPos + vec2(deltaPos.x, 0.0f))) //先试试恢复y坐标
        {
            actor_.GetVelocity().y = 0.0f;
            //看看是脑袋撞了还是脚撞了
            if(deltaPos.y > 0.0f)
                actor_.GetEnvirInput().colUp = true;
            else
                actor_.GetEnvirInput().colDown = true;
        }
        else //只恢复y坐标不行
        {
            if(testNewPos(oldPos + vec2(0.0f, deltaPos.y))) //再试试x坐标
            {
                actor_.GetVelocity().x = 0.0f;
                //看看是左边撞了还是右边撞了
                if(deltaPos.x > 0.0f)
                    actor_.GetEnvirInput().colRight = true;
                else
                    actor_.GetEnvirInput().colLeft = true;
            }
            else //看来只能直接恢复上一帧坐标了
            {
#ifdef _DEBUG
                assert(testNewPos(oldPos));
#else
                actor_.GetPosition() = oldPos;
#endif
            }
        }
    }

    //更新用户输入
    auto &userInput  = actor_.GetUserInput();
    userInput.left   = im_.IsKeyPressed(ACTOR_INPUT_KEY_MOVE_LEFT);
    userInput.right  = im_.IsKeyPressed(ACTOR_INPUT_KEY_MOVE_RIGHT);
    userInput.shift  = im_.IsKeyPressed(ACTOR_INPUT_KEY_SHIFT);
    userInput.jump   = keyJump_.Update(im_.IsKeyPressed(ACTOR_INPUT_KEY_JUMP));
    userInput.attack = keyAttack_.Update(im_.IsMouseButtonPressed(ACTOR_INPUT_BUTTON_ATTACK));
}
