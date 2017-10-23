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

class Weapon
{
public:
    virtual ~Weapon(void) { }

    virtual void Update(void) = 0;
    virtual void Draw(const glm::mat3 &trans, const OWE::ScreenScale &scale) = 0;

    virtual std::vector<OWE::BoundingArea> GetBoundingAreas(void) = 0;
    virtual void Attack(void) = 0;
};

#endif //__WEAPON_H__
