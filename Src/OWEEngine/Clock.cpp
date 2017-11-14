/*================================================================
Filename: Clock.cpp
Date: 2017.10.16
Created by AirGuanZ
================================================================*/
#include <Windows.h>

#include "Include\Clock.h"

__OWE_BEGIN_NAMESPACE__(OWE)

Clock::Clock(void)
{
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    ratio_ = 1000.0 / freq.QuadPart;

    paused_ = false;
    Restart();
}

void Clock::Restart(void)
{
    LARGE_INTEGER tick;
    QueryPerformanceCounter(&tick);

    startTime_ = ratio_ * tick.QuadPart;
    lastTick_ = startTime_;
    elapsedTime_ = 0.0;
}

void Clock::Tick(void)
{
    LARGE_INTEGER tick;
    QueryPerformanceCounter(&tick);

    InternalFormat thisTick = ratio_ * tick.QuadPart;
    elapsedTime_ = thisTick - lastTick_;
    lastTick_ = thisTick;
}

double Clock::ElapsedTime(void) const
{
    return paused_ ? 0.0 : elapsedTime_;
}

double Clock::TotalTime(void) const
{
    return lastTick_ - startTime_;
}

void Clock::Pause(void)
{
    paused_ = true;
}

void Clock::Continue(void)
{
    paused_ = false;
    Tick();
    Tick();
}

__OWE_END_NAMESPACE__(OWE)
