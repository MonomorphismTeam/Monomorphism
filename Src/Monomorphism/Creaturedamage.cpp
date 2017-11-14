#include "Include\Creaturedamage.h"
#include "Include\Actor.h"
#include "Include\Creature.h"
CreaturedamageA::CreaturedamageA(const OWE::BoundingArea &area, float damage, double restLife)
    : area_(area), damage_(damage), restLife_(restLife)
{

}

CreaturedamageA::~CreaturedamageA(void)
{
    //do nothing
}

void CreaturedamageA::Update(double time)
{
    restLife_ -= time;
}

void CreaturedamageA::Draw(const OWE::ScreenScale &scale)
{
    //do nothing
}

std::vector<OWE::BoundingArea> CreaturedamageA::GetBoundingAreas(void) const
{
    return { OWE::BoundingArea(area_) };
}

void CreaturedamageA::Damage(Creature * monster)
{
    //do nothing
}

void CreaturedamageA::Damage(Actor *actor)
{
    actor->Hurt(damage_);
    restLife_ = 0;
}

bool CreaturedamageA::IsDead(void) const
{
    if(restLife_ < 0) return true;
    else return false;
}

//-------------------------B-------------------
CreaturedamageB::CreaturedamageB(glm::vec2 shooting, glm::vec2 poi, float damage,
    double restLife, bool who, float speed, std::string filepath)
{
    sizet_ = glm::vec2(0.3f, 0.3f);
    float length = glm::sqrt(shooting.x * shooting.x + shooting.y * shooting.y);
    length /= speed;
    velocity_ = glm::vec2(shooting.x / length, shooting.y / length);
    speed_ = speed;
    lp_ = poi;
    damage_ = damage;
    restLife_ = restLife;
    if(who)creat_ = Creator::MONSTER;
    else creat_ = Creator::ACTOR;

    OWE::_Texture2DAux::Desc tmp;
    OWE::_Texture2DAux::_LoadTexture2DFromFile(filepath, tmp, text_);
}

CreaturedamageB::~CreaturedamageB()
{

}

void CreaturedamageB::Update(double time)
{
    restLife_ -= time;
    lp_ += glm::vec2(time * velocity_.x, time * velocity_.y);
    //improve move & destroy;
}

void CreaturedamageB::Draw(const OWE::ScreenScale &scale)
{
    OWE::ImmediateRenderer::DrawTexturedBoxWithScreenTrans(lp_, lp_ + sizet_, glm::vec2(0, 0), glm::vec2(0, 0),
        text_.GetTextureView(), scale, OWE::ImmediateRenderer::RenderMode::AlphaTest);
}

std::vector<OWE::BoundingArea> CreaturedamageB::GetBoundingAreas(void) const
{
    OWE::BoundingArea::AABB tmp(lp_.x, lp_.y, lp_.x + sizet_.x, lp_.y + sizet_.y);
    return { OWE::BoundingArea(tmp) };
}

void CreaturedamageB::Damage(Creature *creature)
{
    if(creat_ == Creator::ACTOR)
    {
        creature->MinusHP(damage_);
        restLife_ = 0;
    }
}

void CreaturedamageB::Damage(Actor *actor)
{
    if(creat_ == Creator::MONSTER)
    {
        actor->Hurt(damage_);
        restLife_ = 0;
    }
}

bool CreaturedamageB::IsDead(void) const
{
    if(restLife_ < 0) return true;
    else return false;
}
