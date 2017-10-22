/*================================================================
Filename: Animation.h
Date: 2017.10.22
Created by AirGuanZ
================================================================*/
#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include <vector>

#include "ImmediateRenderer.h"
#include "ScreenScale.h"
#include "Texture2D.h"

__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(_AnimationAux)

using TextureSeq = std::vector<Texture2DView>;
using KeypntsSeq = std::vector<float>;

struct _AnimationData
{
    TextureSeq texSeq;
    KeypntsSeq kpSeq;
};

class _Animation
{
public:
    using Data = _AnimationData;

    _Animation(const Data *data = nullptr);

    void SetTexData(const Data *data);
    bool IsAvailable(void) const;
    const Data *GetTexData(void) const;
    
    glm::vec2 GetLBPosition(void) const;
    void SetLBPosition(const glm::vec2 &LB);

    glm::vec2 GetRTPosition(void) const;
    void SetRTPosition(const glm::vec2 &RT);

    void Move(const glm::vec2 &deltaPos);

    void Restart(void);
    void Tick(double deltaTime);

    double GetTime(void) const;
    void SetTime(double time);

    void Draw(
        const ScreenScale &scale,
        ImmediateRenderer::RenderMode mode = ImmediateRenderer::RenderMode::AlphaTest,
        const ImmediateRenderer::RenderDesc &desc = ImmediateRenderer::RenderDesc(0.5f));

private:
    glm::vec2 LB_;
    glm::vec2 RT_;

    double time_;
    int idx_;
    const Data *data_;
};

__OWE_END_NAMESPACE__(_AnimationAux)

using Animation = _AnimationAux::_Animation;

__OWE_END_NAMESPACE__(OWE)

#include "Animation.inl"

#endif //__ANIMATION_H__
