/*================================================================
Filename: TextureAnimation.inl
Date: 2017.10.22
Created by AirGuanZ
================================================================*/
#include <cassert>

__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(_TextureAnimationAux)

inline _TextureAnimation::_TextureAnimation(const Data *data)
    : LB_(0.0f), RT_(1.0f),
    time_(0.0), idx_(0)
{
    SetTexData(data);
}

inline void _TextureAnimation::SetTexData(const _TextureAnimation::Data *data)
{
#ifdef _DEBUG
    if(data)
    {
        assert(data->texSeq.empty() == false);
        assert(data->kpSeq.size() == data->texSeq.size() - 1);
    }
#endif
    data_ = data;
    Restart();
}

inline bool _TextureAnimation::IsAvailable(void) const
{
    return data_ != nullptr;
}

inline const _TextureAnimation::Data *_TextureAnimation::GetTexData(void) const
{
    return data_;
}

inline glm::vec2 _TextureAnimation::GetLBPosition(void) const
{
    return LB_;
}

inline void _TextureAnimation::SetLBPosition(const glm::vec2 &LB)
{
    LB_ = LB;
}

inline glm::vec2 _TextureAnimation::GetRTPosition(void) const
{
    return RT_;
}

inline void _TextureAnimation::SetRTPosition(const glm::vec2 &RT)
{
    RT_ = RT;
}

inline void _TextureAnimation::Move(const glm::vec2 &deltaPos)
{
    LB_ += deltaPos;
    RT_ += deltaPos;
}

inline void _TextureAnimation::Restart(void)
{
    time_ = 0.0;
    idx_ = 0;
}

inline void _TextureAnimation::Tick(double deltaTime)
{
    time_ += deltaTime;
    while(data_ && idx_ < static_cast<int>(data_->kpSeq.size()) && time_ >= data_->kpSeq[idx_])
        ++idx_;
}

inline double _TextureAnimation::GetTime(void) const
{
    return time_;
}

inline bool _TextureAnimation::End(void) const
{
    return !data_ || time_ >= data_->kpSeq.back();
}

inline void _TextureAnimation::Draw(
    const ScreenScale &scale,
    ImmediateRenderer::RenderMode mode,
    const ImmediateRenderer::RenderDesc &desc)
{
    assert(IsAvailable());
    ImmediateRenderer::DrawTexturedBox(
        LB_, RT_, glm::vec2(0.0f), glm::vec2(1.0f),
        data_->texSeq[idx_], scale, mode, desc);
}

__OWE_END_NAMESPACE__(_TextureAnimationAux)
__OWE_END_NAMESPACE__(OWE)
