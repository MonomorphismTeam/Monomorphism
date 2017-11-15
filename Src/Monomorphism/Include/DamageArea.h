/*================================================================
Filename: DamageArea.h
Date: 2017.10.20
Created by AirGuanZ
================================================================*/
#ifndef __DAMAGE_AREA_H__
#define __DAMAGE_AREA_H__

#include <vector>

#include <glm\glm.hpp>
#include <OWE.h>

class Actor;
class Creature;

class DamageArea
{
public:
    virtual ~DamageArea(void) { }

    virtual void Update(double time) = 0;
    virtual void Draw(const OWE::ScreenScale &scale) = 0;
    virtual void DrawLight(const OWE::ScreenScale &scale) { }

    virtual std::vector<OWE::BoundingArea> GetBoundingAreas(void) const = 0;
    virtual void Damage(Creature *monster) = 0;
    virtual void Damage(Actor *actor) = 0;

    virtual bool IsDead(void) const = 0;
};

#endif //__DAMAGE_AREA_H__
