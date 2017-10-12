/*================================================================
Filename: BoundingArea.cpp
Date: 2017.10.12
Created by AirGuanZ
================================================================*/
#include "Include\Common.h"
#include "Include\BoundingArea.h"

__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(_BoundingAreaAux)

//辅助函数：区域相交测试
namespace
{
    //两个区间是否交叠
    inline bool _IntervalInct(float min1, float max1, float min2, float max2)
    {
        return !(max1 < min2 || max2 < min1);
    }

    inline bool _Intersect(const _BoundingArea::AABB &box1,
                           const _BoundingArea::AABB &box2)
    {
        return _IntervalInct(box1.LB.x, box1.RT.x, box2.LB.x, box2.RT.x) &&
            _IntervalInct(box1.LB.y, box1.RT.y, box2.LB.y, box2.RT.y);
    }

    inline bool _Intersect(const _BoundingArea::Circle &cir1,
                           const _BoundingArea::Circle &cir2)
    {
        glm::vec2 d = cir1.cen - cir2.cen;
        float r = cir1.radius + cir2.radius;
        return d.x * d.x + d.y * d.y < r * r;
    }

    inline bool _Intersect(const _BoundingArea::AABB &box,
                           const _BoundingArea::Circle &cir)
    {
        glm::vec2 u = glm::abs(cir.cen - 0.5f * (box.LB + box.RT));
        glm::vec2 v = u - 0.5f * (box.RT - box.LB);
        return v.x * v.x + v.y * v.y < cir.radius;
    }

    inline bool _Intersect(const _BoundingArea::Circle &cir,
                           const _BoundingArea::AABB &box)
    {
        return _Intersect(box, cir);
    }
}

_BoundingArea::_BoundingArea(const _BoundingArea::AABB &aabb)
    : type_(Type::AABB)
{
    aabb_ = aabb;
}

_BoundingArea::_BoundingArea(const _BoundingArea::Circle &circle)
    : type_(Type::Circle)
{
    circle_ = circle;
}

_BoundingArea::Type _BoundingArea::GetType(void) const
{
    return type_;
}

//IMPROVE：可以优化
bool _BoundingArea::Interscet(const _BoundingArea &other) const
{
    switch(type_)
    {
    case Type::AABB:
        switch(other.GetType())
        {
        case Type::AABB:
            return _Intersect(aabb_, other.aabb_);
        case Type::Circle:
            return _Intersect(aabb_, other.circle_);
        default:
            abort();
        }
    case Type::Circle:
        switch(other.GetType())
        {
        case Type::AABB:
            return _Intersect(circle_, other.aabb_);
        case Type::Circle:
            return _Intersect(circle_, other.circle_);
        default:
            abort();
        }
    }
    abort();
    return false;
}

__OWE_END_NAMESPACE__(_BoundingAreaAux)
__OWE_END_NAMESPACE__(OWE)
