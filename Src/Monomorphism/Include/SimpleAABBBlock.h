/*================================================================
Filename: SimpleAABBBlock.h
Date: 2017.11.13
Created by AirGuanZ
================================================================*/
#ifndef __SIMPLE_AABB_BLOCK_H__
#define __SIMPLE_AABB_BLOCK_H__

#include <OWE.h>
#include <glm\glm.hpp>

#include "BlockArea.h"

class SimpleAABBBlock : public BlockArea
{
public:
    SimpleAABBBlock(const glm::vec2 &LB, const glm::vec2 &RT, OWE::Texture2DView tex);
    ~SimpleAABBBlock(void);

    void Update(double time);
    void Draw(const OWE::ScreenScale &scale);

    std::vector<OWE::BoundingArea> GetBoundingAreas(void) const;

    bool IsDead(void) const;

private:
    glm::vec2 LB_, RT_;
    OWE::Texture2DView tex_;
};

#endif //__SIMPLE_AABB_BLOCK_H__
