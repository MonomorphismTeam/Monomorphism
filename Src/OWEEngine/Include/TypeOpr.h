/*================================================================
Filename: TypeOpr.h
Date: 2017.9.19
Created by AirGuanZ
================================================================*/
#ifndef __OWE_TYPE_OPR_H__
#define __OWE_TYPE_OPR_H__

#include <cstdint>
#include <tuple>

#include "Common.h"

__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(Utility)

namespace _TypeOprAux
{
    template<typename T, bool IsNotNumeric>
    struct _RefIf;

    template<typename T>
    struct _RefIf<T, true>
    {
        using Type = typename std::remove_reference<T>::type&;
    };

    template<typename T>
    struct _RefIf<T, false>
    {
        using Type = typename std::remove_reference<T>::type;
    };

    template<typename T>
    struct RefIfNotNumeric
    {
        using Type = typename _TypeOprAux::_RefIf<T, !std::numeric_limits<T>::is_specialized>::Type;
    };

    template<typename A, typename B>
    struct _IsSameTypeAux
    {
        enum { value = 0 };
    };

    template<typename A>
    struct _IsSameTypeAux<A, A>
    {
        enum { value = 1 };
    };

    template<typename T>
    struct _IsClassAux
    {
        template<typename U> static char helper(int U::*);
        template<typename U> static std::uint32_t helper(...);

        enum { value = ((sizeof(helper<T>(nullptr)) == 1) ? 1 : 0) };
    };

    template<typename Base, typename Derived>
    struct _IsBaseOfAux
    {
        template<typename T>
        static char helper(Derived, T);
        static std::uint32_t helper(Base, int);
        struct Conv
        {
            operator Derived();
            operator Base() const;
        };

        enum { value = ((sizeof(helper(Conv(), 0)) == 1) ? 1 : 0) };
    };

    template<typename Base>
    struct _IsBaseOfAux<Base, Base>
    {
        enum { value = 1 };
    };
}

//对非内建数值类型，给出其引用版本
template<typename T>
using RefIfNotNumeric_t = typename _TypeOprAux::RefIfNotNumeric<T>::Type;

//两个类型是否相同
template<typename A, typename B>
inline constexpr bool IsSameType(void)
{
    return static_cast<int>(_TypeOprAux::_IsSameTypeAux<A, B>::value) == 1;
}

//一个类型是否是类
template<typename T>
inline constexpr bool IsClass(void)
{
    return static_cast<int>(_TypeOprAux::_IsClassAux<T>::value) == 1;
}

//一个类型是是否是另一个类型的基类
template<typename Base, typename Derived>
inline constexpr bool IsBaseOf(void)
{
    static_assert(IsClass<Base>() && IsClass<Derived>(), "IsBaseOf: Base/Derived must class-type");
    return static_cast<int>(_TypeOprAux::_IsBaseOfAux<Base, Derived>::value) == 1;
}

__OWE_END_NAMESPACE__(Utility)
__OWE_END_NAMESPACE__(OWE)

#endif //__OWE_TYPE_OPR_H__
