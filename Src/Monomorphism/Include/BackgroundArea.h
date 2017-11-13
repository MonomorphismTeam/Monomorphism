/*================================================================
Filename: BackgroundArea.h
Date: 2017.11.13
Created by AirGuanZ
================================================================*/
#ifndef __BACKGROUND_AREA_H__
#define __BACKGROUND_AREA_H__

#include <OWE.h>

#include "BlockArea.h"

class BackgroundArea : public BlockArea
{
public:
    ~BackgroundArea(void);

    OWE::TiledTexture &GetBackgroundTexture(void);

    void Update(double time);
    void Draw(const OWE::ScreenScale &scale);

    std::vector<OWE::BoundingArea> GetBoundingAreas(void) const;

    bool IsDead(void) const;

private:
    OWE::TiledTexture bkgd_;
};

#endif //__BACKGROUND_AREA_H__
