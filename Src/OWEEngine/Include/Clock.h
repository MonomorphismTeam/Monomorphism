/*================================================================
Filename: Clock.h
Date: 2017.10.16
Created by AirGuanZ
================================================================*/
#ifndef __OWE_CLOCK_H__
#define __OWE_CLOCK_H__

#include "Common.h"

__OWE_BEGIN_NAMESPACE__(OWE)

class Clock
{
public:
    Clock(void);

    void Restart(void);

    void Tick(void);

    double ElapsedTime(void) const;

    double TotalTime(void) const;

private:
    using InternalFormat = double;

    InternalFormat startTime_;
    InternalFormat lastTick_;
    InternalFormat elapsedTime_;
    InternalFormat ratio_;
};

__OWE_END_NAMESPACE__(OWE)

#endif //__OWE_CLOCK_H__
