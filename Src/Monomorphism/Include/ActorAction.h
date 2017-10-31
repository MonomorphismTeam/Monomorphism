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
        using TexSeq = std::vector<OWE::Texture2D>;
        using KpSeq = std::vector<double>;
        
        ActorAction(void)
            : loop_(false), time_(0.0), idx_(0),
            texSeq_(nullptr), kpSeq_(nullptr)
        {

        }

        void Tick(double time)
        {
            time_ += time;
            while(idx_ + 1 < static_cast<int>(kpSeq_->size()) && time_ >= kpSeq_->at(idx_))
                ++idx_;
            if(loop_ && time_ >= kpSeq_->back())
            {
                time_ -= std::floor(time_ / kpSeq_->back()) * kpSeq_->back();
                idx_ = 0;
                while(idx_ + 1 < static_cast<int>(kpSeq_->size()) && time_ >= kpSeq_->at(idx_))
                    ++idx_;
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
            return !loop_ && time_ >= kpSeq_->back();
        }

        OWE::Texture2DView CurrentTex(void) const
        {
            assert(0 <= idx_ && idx_ < static_cast<int>(texSeq_->size()));
            return texSeq_->at(idx_);
        }

        double CurrentTime(void) const
        {
            return time_;
        }

        int CurrentTexIdx(void) const
        {
            return idx_;
        }

        void SetData(const TexSeq *texSeq, const KpSeq *kpSeq)
        {
            if(!texSeq || !kpSeq)
            {
                Clear();
                return;
            }
            assert(!texSeq->empty() && texSeq->size() == kpSeq->size());
            texSeq_ = texSeq;
            kpSeq_  = kpSeq;
        }

        void Clear(void)
        {
            texSeq_ = nullptr;
            kpSeq_  = nullptr;
        }

    private:
        bool loop_;

        double time_;
        int idx_;

        const TexSeq *texSeq_;
        const KpSeq *kpSeq_;
    };
}

#endif //__ACTOR_ACTION_H__
