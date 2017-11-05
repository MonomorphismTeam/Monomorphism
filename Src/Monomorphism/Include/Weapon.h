/*================================================================
Filename: Weapon.h
Date: 2017.10.23
Created by AirGuanZ
================================================================*/
#ifndef __WEAPON_H__
#define __WEAPON_H__

#include <vector>

#include <glm\glm.hpp>
#include <OWE.h>

class DamageArea;
class Actor;

class Weapon
{
public:
    virtual ~Weapon(void) { }

    virtual void Restart(void) = 0;
    virtual void Update(const Actor &actor, double time) = 0;
    virtual void Draw(const Actor &actor, const OWE::ScreenScale &scale) = 0;

    virtual std::vector<DamageArea*> GetDamageAreas(void) const = 0;

    virtual bool Busy(void) const = 0;
};

#endif //__WEAPON_H__
