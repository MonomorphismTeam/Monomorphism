/*================================================================
Filename: DecorateArea.h
Date: 2017.11.14
Created by AirGuanZ
================================================================*/
#ifndef __DECORATE_AREA_H__
#define __DECORATE_AREA_H__

#include "BlockArea.h"

class DecorateArea : public BlockArea
{
public:
    DecorateArea(const glm::vec2 &LB,
                 const glm::vec2 &RT,
                 const glm::vec2 &lightLB,
                 const glm::vec2 &lightRT,
                 OWE::Texture2DView tex,
                 OWE::Texture2DView light);

    void Update(double time);
    void Draw(const OWE::ScreenScale &scale);
    void DrawLight(const OWE::ScreenScale &scale);

    std::vector<OWE::BoundingArea> GetBoundingAreas(void) const;

    bool IsDead(void) const;

private:
    OWE::Texture2DView tex_;
    OWE::Texture2DView light_;

    glm::vec2 LB_;
    glm::vec2 RT_;

    glm::vec2 lightLB_;
    glm::vec2 lightRT_;
};

#endif //__DECORATE_AREA_H__
