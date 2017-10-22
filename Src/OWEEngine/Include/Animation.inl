/*================================================================
Filename: Animation.inl
Date: 2017.10.22
Created by AirGuanZ
================================================================*/
#include <cassert>

__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(_AnimationAux)

inline _Animation::_Animation(const Data *data)
    : LB_(0.0f), RT_(1.0f),
    time_(0.0), idx_(0)
{
    SetTexData(data);
}

inline void _Animation::SetTexData(const _Animation::Data *data)
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

inline bool _Animation::IsAvailable(void) const
{
    return data_ != nullptr;
}

inline const _Animation::Data *_Animation::GetTexData(void) const
{
    return data_;
}

inline glm::vec2 _Animation::GetLBPosition(void) const
{
    return LB_;
}

inline void _Animation::SetLBPosition(const glm::vec2 &LB)
{
    LB_ = LB;
}

inline glm::vec2 _Animation::GetRTPosition(void) const
{
    return RT_;
}

inline void _Animation::SetRTPosition(const glm::vec2 &RT)
{
    RT_ = RT;
}

inline void _Animation::Move(const glm::vec2 &deltaPos)
{
    LB_ += deltaPos;
    RT_ += deltaPos;
}

inline void _Animation::Restart(void)
{
    time_ = 0.0;
    idx_ = 0;
}

inline void _Animation::Tick(double deltaTime)
{
    time_ += deltaTime;
    if(data_ && idx_ < data_->kpSeq.back() && time_ >= data_->kpSeq[idx_])
        ++idx_;
}

inline double _Animation::GetTime(void) const
{
    return time_;
}

inline void _Animation::SetTime(double time)
{
    time_ = time;
}

inline void _Animation::Draw(
    const ScreenScale &scale,
    ImmediateRenderer::RenderMode mode,
    const ImmediateRenderer::RenderDesc &desc)
{
    assert(IsAvailable());
    ImmediateRenderer::DrawTexturedBox(
        LB_, RT_, glm::vec2(0.0f), glm::vec2(1.0f),
        data_->texSeq[idx_], scale, mode, desc);
}

__OWE_END_NAMESPACE__(_AnimationAux)
__OWE_END_NAMESPACE__(OWE)
