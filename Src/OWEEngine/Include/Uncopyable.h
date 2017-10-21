/*================================================================
Filename: Uncopyable.h
Date: 2017.10.21
Created by AirGuanZ
================================================================*/
#ifndef __OWE_UNCOPYABLE_H__
#define __OWE_UNCOPYABLE_H__

#include "Common.h"

__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(Utility)

class Uncopyable
{
    Uncopyable(const Uncopyable&) = delete;
    Uncopyable(Uncopyable&&) = delete;
    Uncopyable &operator=(const Uncopyable&) = delete;
};

__OWE_END_NAMESPACE__(Utility)
__OWE_END_NAMESPACE__(OWE)

#endif //__OWE_UNCOPYABLE_H__