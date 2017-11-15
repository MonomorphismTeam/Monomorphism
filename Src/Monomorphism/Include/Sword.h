/*================================================================
Filename: Sword.h
Date: 2017.11.4
Created by AirGuanZ
================================================================*/

/*
    最基本的攻击类武器
    相当于什么也没有 = =
*/

#ifndef __SWORD_H__
#define __SWORD_H__

#include <string>
#include <glm\glm.hpp>
#include <OWE.h>

#include "Weapon.h"

class Sword : public Weapon
{
public:
    Sword(void);
    ~Sword(void);

    void Restart(void);
    void Update(const Actor &actor, double time);
    void Draw(const Actor &actor, const OWE::ScreenScale &scale);

    std::vector<DamageArea*> GetDamageAreas(void) const;

    bool Busy(void) const;

private:
    bool restarted_;
    std::vector<DamageArea*> damageAreas_;
};

#endif //__SWORD_H__
