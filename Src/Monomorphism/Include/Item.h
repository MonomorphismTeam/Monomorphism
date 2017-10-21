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

    virtual void Update(void) = 0;
    virtual void Draw(void) = 0;

    virtual std::vector<OWE::BoundingArea> GetBoundingAreas(void) const = 0;
    virtual void InteractWithCreature(Creature *creature) = 0;
    virtual void InteractWithActor(Actor *actor) = 0;
};

#endif //__ITEM_H__
