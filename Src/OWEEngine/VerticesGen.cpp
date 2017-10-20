/*================================================================
Filename: VerticesGen.cpp
Date: 2017.10.20
Created by AirGuanZ
================================================================*/
#include <cassert>

#include "Include\VerticesGen.h"

__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(Utility)

void GenBoxVertices(const glm::vec2 &LB, const glm::vec2 &RT, glm::vec2 *data)
{
    assert(data);
    data[0] = LB; data[1] = glm::vec2(LB.x, RT.y); data[2] = RT;
    data[3] = LB; data[4] = RT; data[5] = glm::vec2(RT.x, LB.y);
}

__OWE_END_NAMESPACE__(Utility)
__OWE_END_NAMESPACE__(OWE)