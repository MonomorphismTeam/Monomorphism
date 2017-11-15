#pragma once

#include "DamageArea.h"
#include <OWE.h>
#include <vector>
#define _CREATURE_DAMAGE__
class CreaturedamageA : public DamageArea
{
public:
    CreaturedamageA(const OWE::BoundingArea &area, float damage, double restLife);
    ~CreaturedamageA(void);

    void Update(double time);
    void Draw(const OWE::ScreenScale &scale);

    std::vector<OWE::BoundingArea> GetBoundingAreas(void) const;

    void Damage(Creature *monster);
    void Damage(Actor *actor);

    bool IsDead(void) const;

private:

    OWE::BoundingArea area_;
    float damage_;
    double restLife_;
};

class CreaturedamageB : public DamageArea
{
public:
    enum class Creator
    {
        ACTOR,
        MONSTER
    };
    CreaturedamageB(glm::vec2, glm::vec2, float damage, double restLife, bool, float, OWE::Texture2DView tex);//0 = actor 
    ~CreaturedamageB(void);

    void Update(double time);
    void Draw(const OWE::ScreenScale &scale);
    void DrawLight(const OWE::ScreenScale &scale);

    std::vector<OWE::BoundingArea> GetBoundingAreas(void) const;

    void Damage(Creature *);
    void Damage(Actor *actor);

    bool IsDead(void) const;

private:

    glm::vec2 velocity_;
    glm::vec2 lp_;
    glm::vec2 sizet_;
    float damage_;
    float speed_;
    Creator creat_;
    double restLife_;
    OWE::Texture2DView text_;
};

