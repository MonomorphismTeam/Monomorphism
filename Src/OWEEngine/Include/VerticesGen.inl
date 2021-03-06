/*================================================================
Filename: VerticesGen.cpp
Date: 2017.10.20
Created by AirGuanZ
================================================================*/
__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(Utility)

inline void GenBoxVertices(const glm::vec2 &LB, const glm::vec2 &RT, glm::vec2 *data)
{
    assert(data);
    data[0] = data[3] = LB;
    data[2] = data[4] = RT;
    data[1] = glm::vec2(LB.x, RT.y);
    data[5] = glm::vec2(RT.x, LB.y);
}

__OWE_END_NAMESPACE__(Utility)
__OWE_END_NAMESPACE__(OWE)
