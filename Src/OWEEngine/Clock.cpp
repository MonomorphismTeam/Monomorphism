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

    Restart();
}

void Clock::Restart(void)
{
    Tick();
    elapsedTime_ = 0.0;
}

void Clock::Tick(void)
{
    LARGE_INTEGER tick;
    QueryPerformanceCounter(&tick);

    InternalFormat thisTick = ratio_ * tick.QuadPart;
    elapsedTime_ = thisTick = lastTick_;
    lastTick_ = thisTick;
}

double Clock::ElapsedTime(void) const
{
    return elapsedTime_;
}

__OWE_END_NAMESPACE__(OWE)