#include "Include\Creturedamage.h"
#include "Include\Actor.h"
CreaturedamageA::CreaturedamageA(const OWE::BoundingArea &area, float damage, double restLife)
	: area_(area), damage_(damage), restLife_(restLife)
{

}

CreaturedamageA::~CreaturedamageA(void)
{
	//do nothing
}

void CreaturedamageA::Update(double time)
{
	restLife_ -= time;
}

void CreaturedamageA::Draw(const OWE::ScreenScale &scale)
{
	//do nothing
}

std::vector<OWE::BoundingArea> CreaturedamageA::GetBoundingAreas(void) const
{
	return area_;
}

void CreaturedamageA::Damage(Creature * monster)
{
	//do nothing
}

void CreaturedamageA::Damage(Actor *actor)
{
	actor->SetHP(actor->GetHP - damage_);
}

bool CreaturedamageA::IsDead(void) const
{
	if (restLife_ < 0) return true;
	else return false;
}