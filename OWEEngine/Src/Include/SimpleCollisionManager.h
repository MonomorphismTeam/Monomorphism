/*================================================================
Filename: SimpleCollisionManager.h
Date: 2017.10.14
Created by AirGuanZ
================================================================*/
#ifndef __OWE_SIMPLE_COLLISION_MANAGER_H__
#define __OWE_SIMPLE_COLLISION_MANAGER_H__

#include <algorithm>
#include <iterator>
#include <set>

#include "CollisionManagerBase.h"

__OWE_BEGIN_NAMESPACE__(OWE)

template<typename T>
class SimpleCollisionManager : public CollisionManagerBase<T>
{
public:
    SimpleCollisionManager(void)
    {

    }

    ~SimpleCollisionManager(void)
    {

    }

    void AddObject(T *obj)
    {
        assert(obj);
        objs_.insert(obj);
    }

    void DelObject(T *obj)
    {
        assert(obj);
        objs_.erase(obj);
    }

    std::vector<T*> CollisionWithBoundingArea(const BoundingArea &bound)
    {
        std::vector<T*> rt;
        std::copy_if(objs_.begin(), objs_.end(), std::back_inserter(rt), [&](T *pObj) -> bool
        {
            for(const BoundingArea &b : pObj->GetBoundingArea())
            {
                if(bound.Intersect(b))
                    return true;
            }
            return false;
        });
        return rt;
    }

    std::vector<T*> CollisionWithRay(const glm::vec2 &p, const glm::vec2 &d)
    {
        std::vector<T*> rt;
        std::copy_if(objs_.begin(), objs_.end(), std::back_inserter(rt), [&](T *pObj) -> bool
        {
            for(const BoundingArea &b : pObj->GetBoundingArea())
            {
                if(b.Intersect(p, d))
                    return true;
            }
            return false;
        });
        return rt;
    }

private:
    SimpleCollisionManager(const SimpleCollisionManager<T>&) = delete;
    SimpleCollisionManager<T> &operator=(const SimpleCollisionManager<T>&) = delete;

    std::set<T*> objs_;
};

__OWE_END_NAMESPACE__(OWE)

#endif //__OWE_SIMPLE_COLLISION_MANAGER_H__
