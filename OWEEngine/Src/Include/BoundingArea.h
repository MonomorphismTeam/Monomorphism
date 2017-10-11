/*================================================================
Filename: BoundingArea.h
Date: 2017.10.11
Created by AirGuanZ
================================================================*/
#ifndef __OWE_BOUNDING_AREA_H__
#define __OWE_BOUNDING_AREA_H__

#include "Common.h"

__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(Scene)

enum class _BoundingAreaType
{
    AABB,
    OBB,
    Circle
};

class BoundingArea
{
public:
    using Type = _BoundingAreaType;
};

__OWE_END_NAMESPACE__(Scene)
__OWE_END_NAMESPACE__(OWE)

#endif //__OWE_BOUNDING_AREA_H__