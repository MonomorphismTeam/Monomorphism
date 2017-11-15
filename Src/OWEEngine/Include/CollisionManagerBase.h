/*================================================================
Filename: CollisionManagerBase.h
Date: 2017.10.13
Created by AirGuanZ
================================================================*/
#ifndef __OWE_COLLISION_BASE_H__
#define __OWE_COLLISION_BASE_H__

#include <cstdint>
#include <vector>

#include "BoundingArea.h"
#include "TypeOpr.h"

__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(_CollisionManagerAux)

template<typename T>
struct _HasMemberAux_GetBoundingAreas
{
    template<typename U>
    static char _Helper(decltype(&T::GetBoundingAreas));
    template<typename U>
    static std::uint32_t _Helper(...);

    enum { value = ((sizeof(_Helper<T>(nullptr)) == 1) ? 1 : 0) };
};

template<typename T>
inline constexpr bool _HasMember_GetBoundingAreas(void)
{
    return static_cast<int>(_HasMemberAux_GetBoundingAreas<T>::value) == 1;
}

__OWE_END_NAMESPACE__(_CollisionManagerAux)

template<typename T>
class CollisionManagerBase
{
public:
    using ObjectType = T;

    static_assert(_CollisionManagerAux::_HasMember_GetBoundingAreas<ObjectType>(),
        "CollisionManager: T must have member function 'GetBoundingAreas`");

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
    return Utility::IsBaseOf<CollisionManagerBase<typename T::ObjectType>, T>();
}

__OWE_END_NAMESPACE__(OWE)

#endif //__OWE_COLLISION_BASE_H__
