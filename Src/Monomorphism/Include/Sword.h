/*================================================================
Filename: Sword.h
Date: 2017.11.3
Created by AirGuanZ
================================================================*/
#ifndef __SWORD_H__
#define __SWORD_H__

#include <string>
#include <vector>

#include <OWE.h>
#include "Weapon.h"

class Sword : public Weapon
{
public:
    Sword(const std::string &texFilename, const glm::vec2 &texSize);
    ~Sword(void);

    void Restart(void);
    void Update(double time);
    void Draw(const Actor &actor, const OWE::ScreenScale &scale);

    std::vector<OWE::BoundingArea> GetDamageAreas(void);

    bool Busy(void) const;

private:
    OWE::Texture2D tex_;
    glm::vec2 texSize_;

    //使用冷却时间
    double busy_;

    //上一次更新时的旋转角和位移
    //用于构造两次更新间挥动区域的DamageArea
    float lastDelta_;
    glm::vec2 lastPos_;
};

#endif //__SWORD_H__
