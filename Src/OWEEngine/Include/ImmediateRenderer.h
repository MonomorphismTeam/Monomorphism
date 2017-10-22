/*================================================================
Filename: ImmediateRenderer.h
Date: 2017.10.22
Created by AirGuanZ
================================================================*/
#ifndef __OWE_IMMEDIATE_RENDERER_H__
#define __OWE_IMMEDIATE_RENDERER_H__

#include "Common.h"
#include "ScreenScale.h"
#include "Texture2DView.h"

__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(_ImmediateRendererAux)

enum class _RenderMode
{
    Basic,
    AlphaTest
};

struct _RenderDesc
{
    _RenderDesc(void) = default;
    _RenderDesc(float minAlpha)
        : alphaThreshold(minAlpha)
    {

    }

    //启用alpha test时的阈值，a分量小于该值的像素被丢弃
    float alphaThreshold = 0.5f;
};

class _ImmediateRenderer
{
public:
    friend class RenderContext;
    using RenderMode = _RenderMode;
    using RenderDesc = _RenderDesc;

    //给定坐标系，在屏幕上绘制一个带纹理的矩形
    static void DrawTexturedBox(
        const glm::vec2 &LB, const glm::vec2 &RT,
        const glm::vec2 &uvLB, const glm::vec2 &uvRT,
        const Texture2DView tex, const ScreenScale &scale,
        RenderMode mode = RenderMode::Basic,
        const RenderDesc &desc = RenderDesc());

private:
    static void Initialize(void);
    static bool IsAvailable(void);
    static void Destroy(void);
};

__OWE_END_NAMESPACE__(_ImmediateRendererAux)

using ImmediateRenderer = _ImmediateRendererAux::_ImmediateRenderer;

__OWE_END_NAMESPACE__(OWE)

#endif //__OWE_IMMEDIATE_RENDERER_H__
