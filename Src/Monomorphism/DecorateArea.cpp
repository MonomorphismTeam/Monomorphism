/*================================================================
Filename: DecorateArea.cpp
Date: 2017.11.14
Created by AirGuanZ
================================================================*/
#include <OWE.h>
#include <glm\glm.hpp>

#include "Include\DecorateArea.h"

using namespace OWE;
using namespace glm;

DecorateArea::DecorateArea(const glm::vec2 &LB,
                           const glm::vec2 &RT,
                           const glm::vec2 &lightLB,
                           const glm::vec2 &lightRT,
                           OWE::Texture2DView tex,
                           OWE::Texture2DView light)
    : tex_(tex), light_(light),
      LB_(LB), RT_(RT),
      lightLB_(lightLB), lightRT_(lightRT)
{

}

void DecorateArea::Update(double time)
{

}

void DecorateArea::Draw(const OWE::ScreenScale &scale)
{
    ImmediateRenderer::DrawTexturedBoxWithScreenTrans(
        LB_, RT_, vec2(0.0f), vec2(1.0f),
        tex_, scale,
        ImmediateRenderer::RenderMode::AlphaTest);
}

void DecorateArea::DrawLight(const OWE::ScreenScale &scale)
{
    if(light_.IsAvailable())
    {
        ImmediateRenderer::DrawTexturedBoxWithScreenTrans(
            lightLB_, lightRT_, vec2(0.0f), vec2(1.0f),
            light_, scale,
            ImmediateRenderer::RenderMode::Basic);
    }
}

std::vector<OWE::BoundingArea> DecorateArea::GetBoundingAreas(void) const
{
    return { };
}

bool DecorateArea::IsDead(void) const
{
    return false;
}
