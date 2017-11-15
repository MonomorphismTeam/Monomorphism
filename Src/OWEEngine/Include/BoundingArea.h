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
    Circle,
    OBB
};

//AABB包围盒
//LB: Left & Bottom
//RT: Right & Top
struct _BoundingArea_AABB
{
    _BoundingArea_AABB(float _xLB = 0.0f, float _yLB = 0.0f,
                       float _xRT = 0.0f, float _yRT = 0.0f)
        : LB(_xLB, _yLB), RT(_xRT, _yRT)
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

//非平行轴式包围盒
struct _BoundingArea_OBB
{
    _BoundingArea_OBB(float x = 0.0f, float y = 0.0f, float ex1 = 0.0f, float ey1 = 0.0f, float ex2 = 0.0f, float ey2 = 0.0f)
        : p(x, y), e1(ex1, ey1), e2(ex2, ey2)
    {
        assert(dot(e1, e2) < 1e-4f);
    }
    glm::vec2 p;
    glm::vec2 e1;
    glm::vec2 e2;
};

class _BoundingArea
{
public:
    using Type = _BoundingAreaType;
    using AABB = _BoundingArea_AABB;
    using Circle = _BoundingArea_Circle;
    using OBB = _BoundingArea_OBB;

    _BoundingArea(const AABB &aabb);
    _BoundingArea(const Circle &circle);
    _BoundingArea(const OBB &obb);

    Type GetType(void) const;

    //测试与另一个包围盒是否相交
    bool Intersect(const _BoundingArea &other) const;

    //若存在t >= 0使得p + t * d与自身相交，则返回最小的这样的t
    //否则返回-1
    float Intersect(const glm::vec2 &p, const glm::vec2 &d) const;

private:
    Type type_;
    union
    {
        AABB aabb_;
        Circle circle_;
        OBB obb_;
    };
};

__OWE_END_NAMESPACE__(_BoundingAreaAux)

using BoundingArea = _BoundingAreaAux::_BoundingArea;

__OWE_END_NAMESPACE__(OWE)

#endif //__OWE_BOUNDING_AREA_H__
