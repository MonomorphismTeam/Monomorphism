/*================================================================
Filename: Entity.h
Date: 2017.10.12
Created by AirGuanZ
================================================================*/
#ifndef __OWE_ENTITY_H__
#define __OWE_ENTITY_H__

#include <vector>

#include "Common.h"
#include "BoundingArea.h"

__OWE_BEGIN_NAMESPACE__(OWE)

class Entity
{
public:
    virtual ~Entity(void);
    
    virtual std::vector<BoundingArea> GetBoundingArea(void) const;
};

__OWE_END_NAMESPACE__(OWE)

#endif //__OWE_ENTITY_H__
