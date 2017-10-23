/*================================================================
Filename: TextureAnimation.h
Date: 2017.10.22
Created by AirGuanZ
================================================================*/
#ifndef __TEXTURE_ANIMATION_H__
#define __TEXTURE_ANIMATION_H__

#include <vector>

#include "ImmediateRenderer.h"
#include "ScreenScale.h"
#include "Texture2D.h"

__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(_TextureAnimationAux)

using TextureSeq = std::vector<Texture2DView>;
using KeypntsSeq = std::vector<float>;

//一组动画数据是一个(纹理，时间)序列
//设texSeq包含n幅图像，第一幅图像从time = 0
//时刻开始播放，kpSeq给出前n - 1次切换图像的时间点，
//time >= kpSeq[n - 1]时维持最后一幅图像
struct _TextureAnimationData
{
    TextureSeq texSeq;
    KeypntsSeq kpSeq;
};

//一个动画是一组动画数据以及用于指示当前播放时间的time变量
//时间的更新需要手动掉用_Animation::Tick进行
class _TextureAnimation
{
public:
    using Data = _TextureAnimationData;

    //对不为空的数据，需满足0 <= kpSeq.size() = texSeq.size() - 1
    _TextureAnimation(const Data *data = nullptr);

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

    void Draw(
        const ScreenScale &scale,
        ImmediateRenderer::RenderMode mode = ImmediateRenderer::RenderMode::AlphaTest,
        const ImmediateRenderer::RenderDesc &desc = ImmediateRenderer::RenderDesc(0.5f));

private:
    glm::vec2 LB_;
    glm::vec2 RT_;

    int idx_;
    double time_;
    const Data *data_;
};

__OWE_END_NAMESPACE__(_TextureAnimationAux)

using TextureAnimation = _TextureAnimationAux::_TextureAnimation;

__OWE_END_NAMESPACE__(OWE)

#include "TextureAnimation.inl"

#endif //__TEXTURE_ANIMATION_H__
