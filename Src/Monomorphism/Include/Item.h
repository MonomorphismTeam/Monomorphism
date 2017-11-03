/*================================================================
Filename: Item.h
Date: 2017.10.20
Created by AirGuanZ
================================================================*/
#ifndef __ITEM_H__
#define __ITEM_H__

#include <vector>
#include <OWE.h>

class Actor;
class Creature;

class Item
{
public:
    virtual ~Item(void) { }

    virtual void Update(double time) = 0;
    virtual void Draw(const OWE::ScreenScale &scale) = 0;

    virtual std::vector<OWE::BoundingArea> GetBoundingAreas(void) const = 0;
    virtual void InteractWithCreature(Creature *creature) = 0;
    virtual void InteractWithActor(Actor *actor) = 0;

    virtual bool IsDead(void) const = 0;
};

#endif //__ITEM_H__
