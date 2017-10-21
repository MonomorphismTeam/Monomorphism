/*================================================================
Filename: VerticesGen.h
Date: 2017.10.20
Created by AirGuanZ
================================================================*/
#ifndef __OWE_VERTICES_GEN_H__
#define __OWE_VERTICES_GEN_H__

#include <cassert>
#include <glm\glm.hpp>

#include "Common.h"

__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(Utility)

inline void GenBoxVertices(const glm::vec2 &LB, const glm::vec2 &RT, glm::vec2 *data);

__OWE_END_NAMESPACE__(Utility)
__OWE_END_NAMESPACE__(OWE)

#include "VerticesGen.inl"

#endif //__OWE_VERTICES_GEN_H__
