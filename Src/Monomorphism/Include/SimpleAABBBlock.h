/*================================================================
Filename: SimpleAABBBlock.h
Date: 2017.11.12
Created by AirGuanZ
================================================================*/
#ifndef __SIMPLE_AABB_BLOCK_H__
#define __SIMPLE_AABB_BLOCK_H__

#include <glm\glm.hpp>
#include <OWE.h>

#include "BlockArea.h"

class SimpleAABBBlock : public BlockArea
{
public:
    SimpleAABBBlock(
        OWE::Texture2DView tex,
        const glm::vec2 &LB, const glm::vec2 &RT,
        const glm::vec2 &uvLB = glm::vec2(0.0f), const glm::vec2 &uvRT = glm::vec2(1.0f));

    void Update(double time);
    void Drwa(const OWE::ScreenScale &scale);

    std::vector<OWE::BoundingArea> GetBoundingAreas(void) const;

    bool IsDead(void) const;

private:
    OWE::Texture2DView tex_;
    glm::vec2 LB_, RT_;
    glm::vec2 uvLB_, uvRT_;
};

#endif //__SIMPLE_AABB_BLOCK_H__
