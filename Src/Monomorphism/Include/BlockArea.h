/*================================================================
Filename: BlockArea.h
Date: 2017.10.20
Created by AirGuanZ
================================================================*/
#ifndef __BLOCK_AREA_H__
#define __BLOCK_AREA_H__

#include <vector>
#include <OWE.h>

class BlockArea
{
public:
    virtual ~BlockArea(void) { }

    virtual void Update(void) const = 0;
    virtual void Draw(void) const = 0;

    virtual std::vector<OWE::BoundingArea> GetBoundingAreas(void) const = 0;

    virtual bool IsDead(void) const = 0;
};

#endif //__BLOCK_AREA_H__
