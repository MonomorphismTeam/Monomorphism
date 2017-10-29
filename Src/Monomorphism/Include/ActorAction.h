/*================================================================
Filename: ActorAction.h
Date: 2017.10.29
Created by AirGuanZ
================================================================*/
#ifndef __ACTOR_ACTION_H__
#define __ACTOR_ACTION_H__

#include <vector>
#include <glm\glm.hpp>
#include <OWE.h>

namespace _ActorAux
{
    class ActorAction
    {
    public:
        using TexSeq = std::vector<OWE::Texture2DView>;
        using KpSeq = std::vector<double>;
        using ColSeq = std::vector<OWE::BoundingArea>;

        ActorAction(void)
            : loop_(false), time_(0.0), idx_(0)
        {

        }

        void Tick(double time)
        {
            time_ += time;
            while(idx_ + 1 < static_cast<int>(kpSeq_.size()) && time >= kpSeq_[idx_])
                ++idx_;
            if(loop_ && time_ >= kpSeq_.back())
            {
                time_ -= std::floor(time_ / kpSeq_.back()) * kpSeq_.back();
                for(idx_ = 0; idx_ + 1 < static_cast<int>(kpSeq_.size()) && time >= kpSeq_[idx_]; ++idx_)
                    ; //do nothing
            }
        }

        void Restart(void)
        {
            time_ = 0.0;
            idx_ = 0;
        }

        bool IsLoopEnabled(void) const
        {
            return loop_;
        }

        void EnableLoop(bool enable)
        {
            if(!loop_ && enable)
                Restart();
            loop_ = enable;
        }

        bool End(void) const
        {
            return !loop_ && time_ >= kpSeq_.back();
        }

        OWE::Texture2DView CurrentTex(void) const
        {
            assert(0 <= idx_ && idx_ < static_cast<int>(texSeq_.size()));
            return texSeq_[idx_];
        }

        double CurrentTime(void) const
        {
            return time_;
        }

        int CurrentTexIdx(void) const
        {
            return idx_;
        }

        void SetData(const TexSeq &texSeq, const KpSeq &kpSeq, const ColSeq &colSeq)
        {
            assert(!texSeq.empty() && texSeq.size() == kpSeq.size() && kpSeq.size() == colSeq.size());
            texSeq_ = texSeq;
            kpSeq_  = kpSeq;
            colSeq_ = colSeq;
        }

        void Clear(void)
        {
            texSeq_.clear();
            kpSeq_.clear();
            colSeq_.clear();
        }

    private:
        bool loop_;

        double time_;
        int idx_;

        TexSeq texSeq_;
        KpSeq kpSeq_;
        ColSeq colSeq_;
    };
}

#endif //__ACTOR_ACTION_H__
