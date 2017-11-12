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

