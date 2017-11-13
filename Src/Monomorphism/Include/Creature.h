/*================================================================
Filename: Creature.h
Date: 2017.10.20
Created by AirGuanZ
================================================================*/
#ifndef __CREATURE_AREA_H__
#define __CREATURE_AREA_H__

#include <vector>
#include <OWE.h>

class Creature
{
public:
    virtual ~Creature(void) { }

    virtual void Update(glm::vec2, double time) = 0;
    virtual void Draw(const OWE::ScreenScale &scale) = 0;
    virtual std::vector<OWE::BoundingArea>GetBoundingAreas(void) const = 0;

    virtual void SetPosition(const glm::vec2) = 0;
    virtual void SetVelocity(glm::vec2) = 0;
    virtual glm::vec2 GetPosition(void) const = 0;
    virtual glm::vec2 GetVelocity(void) const = 0;

    virtual void MinusHP(float ch) = 0;

    virtual bool IsDead(void) const = 0;
};

#endif //__CREATURE_AREA_H__
