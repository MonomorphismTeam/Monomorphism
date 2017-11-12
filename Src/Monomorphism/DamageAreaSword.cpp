/*================================================================
Filename: DamageAreaSword.cpp
Date: 2017.11.6
Created by AirGuanZ
================================================================*/
#include "Include\Creature.h"
#include "Include\DamageAreaSword.h"

DamageAreaSword::DamageAreaSword(const OWE::BoundingArea &area, float damage, double restLife)
    : area_(area), damage_(damage), restLife_(restLife)
{

}

DamageAreaSword::~DamageAreaSword(void)
{
    //do nothing
}

void DamageAreaSword::Update(double time)
{
    if(restLife_ >= 0.0)
        restLife_ -= time;
}

void DamageAreaSword::Draw(const OWE::ScreenScale &scale)
{
    //draw nothing
}

std::vector<OWE::BoundingArea> DamageAreaSword::GetBoundingAreas(void) const
{
    return { area_ };
}

void DamageAreaSword::Damage(Creature *creature)
{
    assert(creature);
    creature->minusHP(-damage_);
}

void DamageAreaSword::Damage(Actor *actor)
{
    assert(actor);
    //do nothing
}

bool DamageAreaSword::IsDead(void) const
{
    return restLife_ < 0.0f;
}
