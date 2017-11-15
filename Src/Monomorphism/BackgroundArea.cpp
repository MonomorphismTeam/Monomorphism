/*================================================================
Filename: BackgroundArea.cpp
Date: 2017.11.13
Created by AirGuanZ
================================================================*/
#include "Include\BackgroundArea.h"

BackgroundArea::~BackgroundArea(void)
{

}

OWE::TiledTexture &BackgroundArea::GetBackgroundTexture(void)
{
    return bkgd_;
}

void BackgroundArea::Update(double time)
{

}

void BackgroundArea::Draw(const OWE::ScreenScale &scale)
{
    assert(bkgd_.IsAvailable());
    bkgd_.Draw(glm::vec2(0.0f), scale);
}

std::vector<OWE::BoundingArea> BackgroundArea::GetBoundingAreas(void) const
{
    return { };
}

bool BackgroundArea::IsDead(void) const
{
    return false;
}
