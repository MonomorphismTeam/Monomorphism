/*================================================================
Filename: ScreenScale.cpp
Date: 2017.10.15
Created by AirGuanZ
================================================================*/
#include <cassert>

#include "Include\RenderContext.h"
#include "Include\ScreenScale.h"

__OWE_BEGIN_NAMESPACE__(OWE)

ScreenScale::ScreenScale(void)
    : ScreenScale(1.0f, 1.0f)
{

}

ScreenScale::ScreenScale(float xpp, float ypp)
{
    Reinit(xpp, ypp);
}

ScreenScale::~ScreenScale(void)
{

}

void ScreenScale::Reinit(float xpp, float ypp)
{
    assert(xpp > 0.0f && ypp > 0.0f);
    xpp_ = xpp;
    ypp_ = ypp;
    clientWidth_ = RenderContext::GetInstance().ClientWidth();
    clientHeight_ = RenderContext::GetInstance().ClientHeight();
    screenWidth_ = clientWidth_ / xpp;
    screenHeight_ = clientHeight_ / ypp;
}

float ScreenScale::GetXPerPixel(void) const
{
    return xpp_;
}

float ScreenScale::GetYPerPixel(void) const
{
    return ypp_;
}

float ScreenScale::ScreenWidth(void) const
{
    return screenWidth_;
}

float ScreenScale::ScreenHeight(void) const
{
    return screenHeight_;
}

float ScreenScale::ScreenToGLX(float x) const
{
    return 2.0f * x / screenWidth_ - 1.0f;
}

float ScreenScale::ScreenToClientX(float x) const
{
    return x * xpp_;
}

float ScreenScale::GLToScreenX(float x) const
{
    return 0.5f * (x + 1.0f) * screenWidth_;
}

float ScreenScale::ClientToScreenX(float x) const
{
    return x / xpp_;
}

float ScreenScale::ScreenToGLY(float y) const
{
    return 2.0f * y / screenHeight_ - 1.0f;
}

float ScreenScale::ScreenToClientY(float y) const
{
    return (screenHeight_ - y) * ypp_;
}

float ScreenScale::GLToScreenY(float y) const
{
    return 0.5f * (y + 1.0f) * screenHeight_;
}

float ScreenScale::ClientToScreenY(float y) const
{
    return (clientHeight_ - y) / ypp_;
}

glm::vec2 ScreenScale::ScreenToGL(const glm::vec2 &coord) const
{
    return glm::vec2{ ScreenToGLX(coord.x), ScreenToGLY(coord.y) };
}

glm::vec2 ScreenScale::ScreenToClient(const glm::vec2 &coord) const
{
    return glm::vec2{ ScreenToClientX(coord.x), ScreenToClientY(coord.y) };
}

glm::vec2 ScreenScale::GLToScreen(const glm::vec2 &coord) const
{
    return glm::vec2{ GLToScreenX(coord.x), GLToScreenY(coord.y) };
}

glm::vec2 ScreenScale::ClientToScreen(const glm::vec2 &coord) const
{
    return glm::vec2{ ClientToScreenX(coord.x), ClientToScreenY(coord.y) };
}

__OWE_END_NAMESPACE__(OWE)
