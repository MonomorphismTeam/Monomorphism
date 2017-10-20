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

class Monster;
class Actor;

class DamageArea
{
public:
    virtual ~DamageArea(void) { }
    virtual std::vector<OWE::BoundingArea> GetBoundingAreas(void) = 0;
    virtual void Damage(Monster *monster);
    virtual void Damage(Actor *actor);
};

#endif //__DAMAGE_AREA_H__
