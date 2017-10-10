/*================================================================
Filename: TypeOpr.h
Date: 2017.9.19
Created by AirGuanZ
================================================================*/
#ifndef __OWE_TYPE_OPR_H__
#define __OWE_TYPE_OPR_H__

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
}

//对非内建数值类型，给出其引用版本
template<typename T>
struct RefIfNotNumeric
{
    using Type = typename _TypeOprAux::_RefIf<T, !std::numeric_limits<T>::is_specialized>::Type;
};

template<typename T>
using RefIfNotNumeric_t = typename RefIfNotNumeric<T>::Type;

__OWE_END_NAMESPACE__(Utility)
__OWE_END_NAMESPACE__(OWE)

#endif //__OWE_TYPE_OPR_H__
