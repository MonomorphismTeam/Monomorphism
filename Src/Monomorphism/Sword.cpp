/*================================================================
Filename: Sword.h
Date: 2017.11.5
Created by AirGuanZ
================================================================*/
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

void Sword::Update(const Actor &actor, double time)
{
    if(restarted_)
    {
        assert(damageAreas_.empty());
        restarted_ = false;
        //TODO：填充要产生的伤害区域
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
