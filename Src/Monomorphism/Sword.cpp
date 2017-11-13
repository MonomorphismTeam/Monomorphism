/*================================================================
Filename: Sword.h
Date: 2017.11.5
Created by AirGuanZ
================================================================*/
#include "Include\Actor.h"
#include "Include\Scene.h"
#include "Include\Sword.h"
#include "Include\DamageAreaSword.h"

Sword::Sword(void)
    : restarted_(false)
{

}

Sword::~Sword(void)
{

}

void Sword::Restart(void)
{
    restarted_ = true;
    damageAreas_.clear();
}

Scene *g_scene;
void Sword::Update(const Actor &actor, double time)
{
    if(restarted_)
    {
        assert(damageAreas_.empty());
        restarted_ = false;
        //TODO：填充要产生的伤害区域
        glm::vec2 pos = actor.GetPosition();
        Actor::Direction dir = actor.GetDirection();
        DamageArea *area;
        if(dir == Actor::Direction::Right)
        {
            area = new DamageAreaSword(
                OWE::BoundingArea(OWE::BoundingArea::AABB(pos.x, pos.y, pos.x + 3.0f, pos.y + 2.0f)),
                3.0f, 2.0f);
        }
        else
        {
            area = new DamageAreaSword(
                OWE::BoundingArea(OWE::BoundingArea::AABB(pos.x - 3.0f, pos.y, pos.x, pos.y + 2.0f)),
                3.0f, 2.0f);
        }
        g_scene->AddDamageArea(area);
    }
    else
        damageAreas_.clear();
}

void Sword::Draw(const Actor &actor, const OWE::ScreenScale &scale)
{

}

std::vector<DamageArea*> Sword::GetDamageAreas(void) const
{
    return damageAreas_;
}

bool Sword::Busy(void) const
{
    return false;
}
