/*================================================================
Filename: BoundingArea.cpp
Date: 2017.10.12
Created by AirGuanZ
================================================================*/
#include <algorithm>
#include <cassert>
#include <tuple>

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
        glm::vec2 v = glm::max(u - 0.5f * (box.RT - box.LB), glm::vec2{ 0.0f, 0.0f });
        return v.x * v.x + v.y * v.y < cir.radius;
    }

    inline bool _Intersect(const _BoundingArea::Circle &cir,
                           const _BoundingArea::AABB &box)
    {
        return _Intersect(box, cir);
    }
}

//辅助函数：线段与包围区域的相交测试
namespace
{
    inline float _Intersect(const _BoundingArea::AABB &aabb,
        const glm::vec2 &p, const glm::vec2 &d)
    {
        assert(glm::length(d) > 1e-8f);
        constexpr float epsilon = 1e-5f;

        //解6个方程得到3个slab区间
        float tx1, tx2, ty1, ty2;
        if(glm::abs(d.x) < epsilon)
        {
            tx1 = std::numeric_limits<float>::min();
            tx2 = std::numeric_limits<float>::max();
        }
        else
        {
            std::tie(tx1, tx2) = std::minmax(
                (aabb.LB.x - p.x) / d.x,
                (aabb.RT.x - p.x) / d.x);
        }

        if(glm::abs(d.y) < epsilon)
        {
            ty1 = std::numeric_limits<float>::min();
            ty2 = std::numeric_limits<float>::max();
        }
        else
        {
            std::tie(ty1, ty2) = std::minmax(
                (aabb.LB.y - p.y) / d.y,
                (aabb.RT.y - p.y) / d.y);
        }

        return _IntervalInct(tx1, tx2, ty1, ty2);
    }

    inline float _Intersect(const _BoundingArea::Circle &cir,
        const glm::vec2 &p, const glm::vec2 &d)
    {
        assert(glm::length(d) > 1e-8f);
        glm::vec2 cmp = cir.cen - p;
        float _2A = 2.0f * glm::dot(d, d);
        float B = 2.0f * glm::dot(d, cmp);
        float C = glm::dot(cmp, cmp) - cir.radius * cir.radius;

        float Delta = B * B - 2.0f * _2A * C;
        if(Delta < 0)
            return -1.0f;

        Delta = glm::sqrt(Delta);
        float t1 = (-B + Delta) / _2A;
        float t2 = (-B - Delta) / _2A;

        float ft1 = t1 < 0.0f ? -1.0f : t1;
        return (0.0f <= t2 && (ft1 < 0.0f || t2 < ft1)) ? t2 : ft1;
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

//IMPROVE：优化do real work的函数的选择流程
bool _BoundingArea::Intersect(const _BoundingArea &other) const
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
        }
        break;
    case Type::Circle:
        switch(other.GetType())
        {
        case Type::AABB:
            return _Intersect(circle_, other.aabb_);
        case Type::Circle:
            return _Intersect(circle_, other.circle_);
        }
        break;
    }
    abort();
    return false;
}

float _BoundingArea::Intersect(const glm::vec2 &p, const glm::vec2 &d) const
{
    switch(type_)
    {
    case Type::AABB:
        return _Intersect(aabb_, p, d);
    case Type::Circle:
        return _Intersect(circle_, p, d);
    }
    abort();
    return -1.0f;
}

__OWE_END_NAMESPACE__(_BoundingAreaAux)
__OWE_END_NAMESPACE__(OWE)
