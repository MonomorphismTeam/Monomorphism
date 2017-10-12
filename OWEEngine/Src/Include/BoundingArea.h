/*================================================================
Filename: BoundingArea.h
Date: 2017.10.11
Created by AirGuanZ
================================================================*/
#ifndef __OWE_BOUNDING_AREA_H__
#define __OWE_BOUNDING_AREA_H__

#include <glm\glm.hpp>

#include "Common.h"

__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(_BoundingAreaAux)

//包围盒类型
enum class _BoundingAreaType
{
    AABB,
    Circle
};

//AABB包围盒
//LB: Left & Bottom
//RT: Right & Top
struct _BoundingArea_AABB
{
    _BoundingArea_AABB(float _xLB = 0.0f, float _yLB = 0.0f,
                       float _xRT = 0.0f, float _yRT = 0.0f)
        : LB(_xLB, _yRT), RT(_xRT, _yRT)
    {

    }
    glm::vec2 LB;
    glm::vec2 RT;
};

//圆形包围区
struct _BoundingArea_Circle
{
    _BoundingArea_Circle(float _xCen = 0.0f, float _yCen = 0.0f, float _radius = 0.0f)
        : cen(_xCen, _yCen), radius(_radius)
    {

    }
    glm::vec2 cen;
    float radius;
};

class _BoundingArea
{
public:
    using Type = _BoundingAreaType;
    using AABB = _BoundingArea_AABB;
    using Circle = _BoundingArea_Circle;

    _BoundingArea(const AABB &aabb);
    _BoundingArea(const Circle &circle);

    Type GetType(void) const;

    //测试与另一个包围盒是否相交
    bool Interscet(const _BoundingArea &other) const;

private:
    Type type_;
    union
    {
        AABB aabb_;
        Circle circle_;
    };
};

__OWE_END_NAMESPACE__(_BoundingAreaAux)

using BoundArea = _BoundingAreaAux::_BoundingArea;

__OWE_END_NAMESPACE__(OWE)

#endif //__OWE_BOUNDING_AREA_H__
