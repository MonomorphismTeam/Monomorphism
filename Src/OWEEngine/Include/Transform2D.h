/*================================================================
Filename: Transform2D.h
Date: 2017.10.16
Created by AirGuanZ
================================================================*/
#ifndef __OWE_TRANSFORM_2D_H__
#define __OWE_TRANSFORM_2D_H__

#include <glm\glm.hpp>

#include "Common.h"

__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(Transform)

inline glm::mat3 Translate(float x, float y)
{
    return glm::mat3(1.0f, 0.0f, 0.0f,
                     0.0f, 1.0f, 0.0f,
                     x,    y,    1.0f);
}

inline glm::mat3 Translate(const glm::vec2 &p)
{
    return Translate(p.x, p.y);
}

inline glm::mat3 Scale(const glm::vec2 &s)
{
    return glm::mat3(s.x, 0.0f, 0.0f,
                     0.0f, s.y, 0.0f,
                     0.0f, 0.0f, 1.0f);
}

inline glm::mat3 Rotate(float radian)
{
    return glm::mat3();
}

__OWE_END_NAMESPACE__(Transform)
__OWE_END_NAMESPACE__(OWE)

#endif //__OWE_TRANSFORM_2D_H__