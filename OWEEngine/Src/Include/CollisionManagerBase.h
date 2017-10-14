/*================================================================
Filename: CollisionManagerBase.h
Date: 2017.10.13
Created by AirGuanZ
================================================================*/
#ifndef __OWE_COLLISION_BASE_H__
#define __OWE_COLLISION_BASE_H__

#include <vector>

#include "BoundingArea.h"
#include "TypeOpr.h"

__OWE_BEGIN_NAMESPACE__(OWE)

class CollidableObject
{
public:
    virtual std::vector<BoundingArea> GetBoundingArea(void) = 0;
};

template<typename T>
class CollisionManagerBase
{
public:
    static_assert(Utility::IsBaseOf<CollidableObject, T>(),
        "CollisionManager: T must be derived from CollidableObject");

    void AddObject(T *obj)
    {
        static_assert(false, "Default CollisionManager: not implemented");
    }

    void DelObject(T *obj)
    {
        static_assert(false, "Default CollisionManager: not implemented");
    }

    std::vector<T*> CollisionWithBoundingArea(const BoundingArea &bound)
    {
        static_assert(false, "Default CollisionManager: not implemented");
    }

    std::vector<T*> CollisionWithRay(const glm::vec2 &p, const glm::vec2 &d)
    {
        static_assert(false, "Default CollisionManager: not implemented");
    }
};

template<typename T>
inline bool IsCollisionManager(void)
{
    return Utility::IsBaseOf<CollisionManagerBase, T>();
}

__OWE_END_NAMESPACE__(OWE)

#endif //__OWE_COLLISION_BASE_H__
