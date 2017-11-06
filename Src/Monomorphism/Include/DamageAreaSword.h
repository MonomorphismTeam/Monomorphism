/*================================================================
Filename: DamageAreaSword.h
Date: 2017.11.6
Created by AirGuanZ
================================================================*/
#ifndef __DAMAGE_AREA_SWORD_H__
#define __DAMAGE_AREA_SWORD_H__

#include "DamageArea.h"

class DamageAreaSword : public DamageArea
{
public:
    DamageAreaSword(const OWE::BoundingArea &area, float damage, double restLift);
    ~DamageAreaSword(void);

    void Update(double time);
    void Draw(const OWE::ScreenScale &scale);

    std::vector<OWE::BoundingArea> GetBoundingAreas(void) const;

    void Damage(Creature *monster);
    void Damage(Actor *actor);

    bool IsDead(void) const;

private:
    OWE::BoundingArea area_;
    float damage_;
    double restLife_;
};

#endif //__DAMAGE_AREA_SWORD_H__
