/*================================================================
Filename: ScreenScale.inl
Date: 2017.10.15
Created by AirGuanZ
================================================================*/
#include "RenderContext.h"

__OWE_BEGIN_NAMESPACE__(OWE)

inline ScreenScale::ScreenScale(void)
    : xpp_(0.0f), ypp_(0.0f),
      clientWidth_(0), clientHeight_(0),
      screenWidth_(0.0f), screenHeight_(0.0f)
{

}

inline ScreenScale::ScreenScale(float xpp, float ypp)
{
    Reinit(xpp, ypp);
}

inline ScreenScale::~ScreenScale(void)
{

}

inline void ScreenScale::Reinit(float xpp, float ypp)
{
    assert(xpp > 0.0f && ypp > 0.0f);
    xpp_ = xpp;
    ypp_ = ypp;
    clientWidth_ = RenderContext::GetInstance().ClientWidth();
    clientHeight_ = RenderContext::GetInstance().ClientHeight();
    screenWidth_ = clientWidth_ / xpp;
    screenHeight_ = clientHeight_ / ypp;
}

inline float ScreenScale::GetXPerPixel(void) const
{
    return xpp_;
}

inline float ScreenScale::GetYPerPixel(void) const
{
    return ypp_;
}

inline float ScreenScale::ScreenWidth(void) const
{
    return screenWidth_;
}

inline float ScreenScale::ScreenHeight(void) const
{
    return screenHeight_;
}

inline float ScreenScale::ScreenToGLX(float x) const
{
    return 2.0f * x / screenWidth_ - 1.0f;
}

inline float ScreenScale::ScreenToClientX(float x) const
{
    return x * xpp_;
}

inline float ScreenScale::GLToScreenX(float x) const
{
    return 0.5f * (x + 1.0f) * screenWidth_;
}

inline float ScreenScale::ClientToScreenX(float x) const
{
    return x / xpp_;
}

inline float ScreenScale::ScreenToGLY(float y) const
{
    return 2.0f * y / screenHeight_ - 1.0f;
}

inline float ScreenScale::ScreenToClientY(float y) const
{
    return (screenHeight_ - y) * ypp_;
}

inline float ScreenScale::GLToScreenY(float y) const
{
    return 0.5f * (y + 1.0f) * screenHeight_;
}

inline float ScreenScale::ClientToScreenY(float y) const
{
    return (clientHeight_ - y) / ypp_;
}

inline glm::vec2 ScreenScale::ScreenToGL(const glm::vec2 &coord) const
{
    return glm::vec2{ ScreenToGLX(coord.x), ScreenToGLY(coord.y) };
}

inline glm::vec2 ScreenScale::ScreenToClient(const glm::vec2 &coord) const
{
    return glm::vec2{ ScreenToClientX(coord.x), ScreenToClientY(coord.y) };
}

inline glm::vec2 ScreenScale::GLToScreen(const glm::vec2 &coord) const
{
    return glm::vec2{ GLToScreenX(coord.x), GLToScreenY(coord.y) };
}

inline glm::vec2 ScreenScale::ClientToScreen(const glm::vec2 &coord) const
{
    return glm::vec2{ ClientToScreenX(coord.x), ClientToScreenY(coord.y) };
}

inline glm::mat3 ScreenScale::ProjMatrix(void) const
{
    return glm::mat3(1.0f, 0.0f, 0.0f,
                     0.0f, 1.0f, 0.0f,
                     -1.0f, -1.0f, 1.0f) *
           glm::mat3(2.0f / screenWidth_, 0.0f, 0.0f,
                     0.0f, 2.0f / screenHeight_, 0.0f,
                     0.0f, 0.0f, 1.0f);
}

__OWE_END_NAMESPACE__(OWE)
