/*================================================================
Filename: Scene.cpp
Date: 2017.11.7
Created by AirGuanZ
================================================================*/
#include <set>

#include <glm\glm.hpp>
#include <OWE.h>

#include "Include\HPMedicine.h"
#include "Include\PauseMenu.h"
#include "Include\ResourceNames.h"
#include "Include\Scene.h"
#include "Include\Sword.h"
#include "Include\World.h"

using namespace std;
using namespace glm;
using namespace OWE;

//==================================== 角色参数 ====================================

constexpr float ACTOR_RUNNING_VEL           = 0.012f;       //移动速度
constexpr float ACTOR_FLOATING_ACC_VEL      = 0.00006f;     //悬空时自给水平加速度
constexpr float ACTOR_SHIFTING_VEL          = 0.035f * 1.1f;        //闪避速度
constexpr float ACTOR_JUMPING_VEL           = 0.025f;       //跳跃初速度
constexpr float ACTOR_MAX_FLOATING_VEL      = 0.012f;       //悬空时最大水平速度
constexpr float ACTOR_FLOATING_FRIC_ACC_VEL = 0.00003f;     //悬空时空气阻力带来的水平加速度
constexpr float ACTOR_GRAVITY_ACC           = -0.00008f;    //重力加速度


constexpr KEY_CODE ACTOR_INPUT_KEY_MOVE_LEFT     = KEY_CODE::KEY_A;             //向左移动
constexpr KEY_CODE ACTOR_INPUT_KEY_MOVE_RIGHT    = KEY_CODE::KEY_D;             //向右移动
constexpr KEY_CODE ACTOR_INPUT_KEY_JUMP          = KEY_CODE::KEY_SPACE;         //跳跃
constexpr KEY_CODE ACTOR_INPUT_KEY_SHIFT         = KEY_CODE::KEY_LSHIFT;        //闪避
constexpr MOUSE_BUTTON ACTOR_INPUT_BUTTON_ATTACK = MOUSE_BUTTON::BUTTON_LEFT;   //攻击

constexpr float ACTOR_INIT_HP = 100.0f; //人物初始血量

//==================================================================================
constexpr int ITEMDROP = 30;

Scene::Scene(void)
    : rc_(RenderContext::GetInstance()),
      im_(InputManager::GetInstance())
{
    
}

Scene::~Scene(void)
{
    for(BlockArea *area : blockAreas_)
        delete area;
    for(Creature *creature : creatures_)
        delete creature;
    for(DamageArea *area : damageAreas_)
        delete area;
    for(Item *item : items_)
        delete item;
    if(tiledBackground_)
        delete tiledBackground_;
}

void Scene::AddBlockArea(BlockArea *area)
{
    assert(area);
    blockAreas_.insert(area);
    blockColMgr_.AddObject(area);
}

void Scene::AddCreature(Creature *creature)
{
    assert(creature);
    creatures_.insert(creature);
    creatureColMgr_.AddObject(creature);
}

void Scene::AddDamageArea(DamageArea *area)
{
    assert(area);
    damageAreas_.insert(area);
    damageColMgr_.AddObject(area);
}

void Scene::AddItem(Item *item)
{
    assert(item);
    items_.insert(item);
    itemColMgr_.AddObject(item);
}

void Scene::Initialize(void)
{
    _InitializeShader();

    leftBound_ = std::numeric_limits<float>::lowest();
    rightBound_ = std::numeric_limits<float>::max();

    actor_.Initialize();
    scale_.Reinit(50.0f * 2 / 3, 50.0f * 2 / 3);
    
    //初始化角色参数
    actor_.SetRunningVel        (ACTOR_RUNNING_VEL);
    actor_.SetFloatingAccVel    (ACTOR_FLOATING_ACC_VEL);
    actor_.SetShiftingVel       (ACTOR_SHIFTING_VEL);
    actor_.SetJumpingVel        (ACTOR_JUMPING_VEL);

    actor_.SetMaxFloatingVel    (ACTOR_MAX_FLOATING_VEL);
    actor_.SetFloatingFricAccVel(ACTOR_FLOATING_FRIC_ACC_VEL);

    actor_.GetPosition() = vec2(1.0f, 5.0f);
    actor_.GetTexSize()  = vec2(0.02f, 0.02f);

    actor_.SetWeapon(new Sword());
    actor_.SetHP(ACTOR_INIT_HP);

    //准备画布
    int winWidth = RenderContext::GetInstance().ClientWidth();
    int winHeight = RenderContext::GetInstance().ClientHeight();

    fbBasic_.Initialize(winWidth, winHeight, 1);
    fbBasic_.AddTex(0, OWE::Texture2D::Desc());
    fbBasic_.AddDepth();

    fbLight_.Initialize(winWidth, winHeight, 1);
    fbLight_.AddTex(0, OWE::Texture2D::Desc());
    fbLight_.AddDepth();
}

void Scene::SetBound(float left, float right)
{
    leftBound_ = left;
    rightBound_ = right;
}

void Scene::SetBackgroundColor(float r, float g, float b, float a)
{
    bkgdColor_ = vec4(r, g, b, a);
}

void Scene::SetTiledBackground(TiledTexture *tex)
{
    tiledBackground_ = tex;
}

Scene::RunningResult Scene::Run(void)
{
    clock_.Restart();
    while(true)
    {
        clock_.Tick();
        
        _UpdateActor();
        _UpdateBlockAreas();
        _UpdateCreatures();
        _UpdateItems();

        _InteractWithItems();

        _InteractWithDamageAreas();
        _UpdateDamageArea();

        scale_.SetCentrePosition(actor_.GetPosition() + vec2(0.0f, scale_.ScreenHeight() * 0.25f));

        //光源遮罩渲染
        fbLight_.Begin();
        {
            rc_.SetClearColor(vec4(0.0f));
            rc_.ClearColorAndDepth();
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE); //亮度直接相加，虽然物理正确的实现是平方相加再开方

            actor_.DrawLight(scale_);
            _DrawBlockAreasLight();
            _DrawDamageAreasLight();

            glDisable(GL_BLEND);
        }
        fbLight_.End();

        //场景渲染
        fbBasic_.Begin();
        {
            rc_.SetClearColor(bkgdColor_);
            rc_.ClearColorAndDepth();

            if(tiledBackground_)
                tiledBackground_->Draw(vec2(0.0f), scale_);

            _DrawBlockAreas();
            _DrawCreatures();
            _DrawItems();
            actor_.Draw(scale_);
            _DrawDamageArea();
        }
        fbBasic_.End();

        //最后合成场景和光照
        rc_.ClearColorAndDepth();

        chainShader_.Bind();
        chainAttribs_->Bind();

        chainBasicTex_.SetAndApply(fbBasic_.GetTex(0));
        chainLightTex_.SetAndApply(fbLight_.GetTex(0));
        rc_.DrawTriangles(6);

        chainAttribs_->Unbind();
        chainShader_.Unbind();

        _DrawActorState();

        rc_.Present();
        
        //外部输入处理
        rc_.DoEvents();
        
        if(actor_.GetPosition().x < leftBound_)
            return RunningResult::OutOfLeftBound;
        if(actor_.GetPosition().x > rightBound_)
            return RunningResult::OutOfRightBound;

        if(actor_.GetHP() < 0.0f)
        {
            im_._KeyUp(KEY_CODE::KEY_SPACE);
            while(true)
            {
                rc_.ClearColorAndDepth();

                OWE::ImmediateRenderer::DrawTexturedBox(
                    vec2(0.0f), vec2(scale_.ScreenWidth(), scale_.ScreenHeight()),
                    vec2(0.0f), vec2(1.0f),
                    World::GetInstance().GetTextureManager().GetTexture("YouDie"),
                    scale_);

                if(im_.IsKeyPressed(KEY_CODE::KEY_SPACE))
                    break;

                rc_.DoEvents();
                rc_.Present();
            }
            return RunningResult::Closed;
        }

        if(im_.IsKeyPressed(KEY_CODE::KEY_P))
        {
            PauseMenu pause;
            clock_.Pause();
            if(!pause.Pause())
                return RunningResult::Closed;
            clock_.Continue();
        }

        if(keyLockHP_.Update(im_.IsKeyPressed(KEY_CODE::KEY_NUMPAD_0)))
            actor_.LockHP(!actor_.IsHPLocked());
    }
}

Actor &Scene::GetActor(void)
{
    return actor_;
}

void Scene::_InitializeShader(void)
{
    assert(!chainShader_.IsAvailable() && !chainVtxPosBuf_.IsAvailable() && !chainVtxUVBuf_.IsAvailable());

    //准备顶点缓存

    int winW = RenderContext::GetInstance().ClientWidth();
    int winH = RenderContext::GetInstance().ClientHeight();

    glm::vec2 vtxPosBufData[6];
    glm::vec2 vtxUVBufData[6];
    Utility::GenBoxVertices(vec2(-1.0f), vec2(1.0f), vtxPosBufData);
    Utility::GenBoxVertices(vec2(0.0f), vec2(1.0f), vtxUVBufData);
    chainVtxPosBuf_.Initialize(6, vtxPosBufData);
    chainVtxUVBuf_.Initialize(6, vtxUVBufData);

    //加载源代码
    string vtxSrc, fragSrc, errMsg;
    if(!Utility::ASCIIFile::Read(SCENE_LIGHT_VERTEX_SHADER, vtxSrc) ||
       !Utility::ASCIIFile::Read(SCENE_LIGHT_FRAGMENT_SHADER, fragSrc))
        throw FatalError("Failed to load shader source for scene lighting");

    if(chainShader_.Initialize(errMsg, VSSrc(vtxSrc), FSSrc(fragSrc)) != Shader::InitState::Success)
        throw FatalError("Failed to initialize shader for scene lighting: " + errMsg);

    chainUniforms_ = chainShader_.GetUniformMgrPtr();
    chainAttribs_ = chainShader_.GetAttribMgrPtr();
    
    chainAttribs_->GetAttrib<glm::vec2>("pos").SetBuffer(chainVtxPosBuf_);
    chainAttribs_->GetAttrib<glm::vec2>("uv").SetBuffer(chainVtxUVBuf_);

    chainBasicTex_ = chainUniforms_->GetUniform<Texture2DView>("basic");
    chainLightTex_ = chainUniforms_->GetUniform<Texture2DView>("light");
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
            {
                actor_.GetEnvirInput().colDown = true;
                //搜索一个合适的恢复位置
                constexpr float deltaY = 1e-3f;
                float dy = 0.0f;
                while(!testNewPos(oldPos + deltaPos + vec2(0.0f, dy)))
                    dy += deltaY;
            }
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

void Scene::_UpdateBlockAreas(void)
{
    //update
    for(BlockArea *pArea : blockAreas_)
        pArea->Update(clock_.ElapsedTime());

    //删去dead block
    std::set<BlockArea*> newBlockAreas;
    for(BlockArea *pArea : blockAreas_)
    {
        if(pArea->IsDead())
        {
            blockColMgr_.DelObject(pArea);
            delete pArea;
        }
        else
            newBlockAreas.insert(pArea);
    }
    blockAreas_ = std::move(newBlockAreas);
}

void Scene::_DrawBlockAreas(void)
{
    for(BlockArea *pArea : blockAreas_)
        pArea->Draw(scale_);
}

void Scene::_DrawBlockAreasLight(void)
{
    for(BlockArea *area : blockAreas_)
        area->DrawLight(scale_);
}

bool Scene::_CreatureTestCoi(const Creature *p)
{
    for(auto &area : p->GetBoundingAreas())
    {
        if(!blockColMgr_.CollisionWithBoundingArea(area).empty())
            return false;
    }
    return true;
}

void Scene::_UpdateCreatures(void)
{
    //update
    double timex = clock_.ElapsedTime();
    for(Creature *pCreature : creatures_)
        pCreature->Update(actor_.GetPosition(), timex);
    //向速度移动
    for(Creature *pCreature : creatures_)
    {
        glm::vec2 oldPosition = pCreature->GetPosition();
        glm::vec2 deltaPos = static_cast<float>(timex) * pCreature->GetVelocity();
        pCreature->SetPosition(oldPosition + deltaPos);

        if(!_CreatureTestCoi(pCreature))//新位置产生了碰撞
        {
            //尝试恢复y轴坐标
            pCreature->SetPosition(oldPosition + glm::vec2(deltaPos.x, 0.0f));
            if(_CreatureTestCoi(pCreature))//恢复成功了
            {
                pCreature->SetVelocity(glm::vec2(pCreature->GetVelocity().x, 0.0));//把y的速度变为0
                                                                                   //搜索一个合适的恢复位置(下落防止抖动)
                if(deltaPos.y < 0)
                {
                    constexpr float deltaY = 1e-2f;
                    float dy = 0.0f;
                    //testNewPos(oldPos + deltaPos + vec2(0.0f, dy)
                    do
                    {
                        pCreature->SetPosition(oldPosition + deltaPos + vec2(0.0f, dy));
                        dy += deltaY;
                    } while(!_CreatureTestCoi(pCreature));
                }
            }
            else
            {
                //需要恢复x坐标
                pCreature->SetPosition(oldPosition + glm::vec2(0.0f, deltaPos.y));
                if(_CreatureTestCoi(pCreature))
                {
                    pCreature->SetVelocity(glm::vec2(0.0f, pCreature->GetVelocity().y));
                }
                else
                {
                    pCreature->SetPosition(oldPosition);
                    pCreature->SetVelocity(glm::vec2(0.0f, 0.0f));
                    //直接恢复上一帧的状态
                }
            }
        }
    }

    //删去dead creature
    std::set<Creature*> newCreatures;
    for(Creature *pCreature : creatures_)
    {
        if(pCreature->IsDead())
        {
            //物品几率掉落

            int s = rand();
            s = glm::abs(s) % 100;
            if(s < ITEMDROP)//add item;
            {
                Item *p = new HPMedicine(20000.0f, false, false, pCreature->GetPosition(), 
                    pCreature->GetPosition() + vec2(1.0f, 1.0f), 
                    World::GetInstance().GetTextureManager().GetTexture("Medicine"));
                AddItem(p);
                //Item = new  create new item;
            }
            creatureColMgr_.DelObject(pCreature);
            delete pCreature;
        }
        else
            newCreatures.insert(pCreature);
    }
    creatures_ = std::move(newCreatures);
}

void Scene::_DrawCreatures(void)
{
    for(Creature *pC : creatures_)
        pC->Draw(scale_);
}

void Scene::_InteractWithDamageAreas(void)
{
    //人的伤害判定
    {
        set<DamageArea*> as;
        for(const BoundingArea &area : actor_.GetBoundingAreas())
        {
            auto damageA = damageColMgr_.CollisionWithBoundingArea(area);
            copy(begin(damageA), end(damageA), inserter(as, begin(as)));
        }
        for(DamageArea *a : as)
            a->Damage(&actor_);
    }

    //其他生物的伤害判定
    for(Creature *c : creatures_)
    {
        set<DamageArea*> as;
        for(const BoundingArea &area : c->GetBoundingAreas())
        {
            auto damageA = damageColMgr_.CollisionWithBoundingArea(area);
            copy(begin(damageA), end(damageA), inserter(as, begin(as)));
        }
        for(DamageArea *a : as)
            a->Damage(c);
    }
}

void Scene::_UpdateDamageArea(void)
{
    //update
    for(DamageArea *pArea : damageAreas_)
        pArea->Update(clock_.ElapsedTime());

    //删去dead block
    std::set<DamageArea*> newDamageAreas;
    for(DamageArea *pArea : damageAreas_)
    {
        if(pArea->IsDead())
        {
            damageColMgr_.DelObject(pArea);
            delete pArea;
        }
        else
            newDamageAreas.insert(pArea);
    }
    damageAreas_ = std::move(newDamageAreas);
}

void Scene::_DrawDamageArea(void)
{
    for(DamageArea *area : damageAreas_)
        area->Draw(scale_);
}

void Scene::_DrawDamageAreasLight(void)
{
    for(DamageArea *a : damageAreas_)
        a->DrawLight(scale_);
}

void Scene::_UpdateItems(void)
{
    //Update
    for(Item *item : items_)
        item->Update(clock_.ElapsedTime());

    //删除死掉的item
    std::set<Item*> newItems;
    for(Item *item : items_)
    {
        if(item->IsDead())
        {
            itemColMgr_.DelObject(item);
            delete item;
        }
        else
            newItems.insert(item);
    }
    items_ = std::move(newItems);
}

void Scene::_DrawItems(void)
{
    for(Item *item : items_)
        item->Draw(scale_);
}

void Scene::_InteractWithItems(void)
{
    std::set<Item*> inItems;
    for(auto &area : actor_.GetBoundingAreas())
    {
        auto it = itemColMgr_.CollisionWithBoundingArea(area);
        for(auto *p : it)
            inItems.insert(p);
    }

    for(auto *p : inItems)
        p->InteractWithActor(&actor_);
}

void Scene::_DrawActorState(void)
{
    OWE::ImmediateRenderer::DrawTexturedBox(
        vec2(1.0f, scale_.ScreenHeight() - 1.3f),
        vec2(1.0f + actor_.GetHP() / 100.0f * 4.0f, scale_.ScreenHeight() - 1.2f),
        vec2(0.0f), vec2(1.0f),
        World::GetInstance().GetTextureManager().GetTexture("HPBar"), scale_);
}
