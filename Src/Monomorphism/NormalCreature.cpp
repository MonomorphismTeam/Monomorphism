#include"Include\NormalCreature.h"
#include<OWE.h>

NormalCreature::NormalCreature(int hp, glm::vec2 lp, glm::vec2 rp,
	OWE::BoundingArea &_area, std::string filepath)
	: area_(_area)
{
	hp_ = 0;
	area_ = _area;
	Are.push_back(_area);
	
}

NormalCreature::~NormalCreature(void)
{
	//Improve
}

void NormalCreature::Update(glm::vec2 playerPoi)
{
	//Improve
	
}

void NormalCreature::Draw(void)
{
	//Improve
}

void NormalCreature::setArea(OWE::BoundingArea c)
{
	area_ = c;
}


void NormalCreature::SetRelation(CreatureRelation c)
{
	relation_ = c;
}

void NormalCreature::SetHp(int hp)
{
	hp_ = hp;
}

std::vector<OWE::BoundingArea> NormalCreature::GetBoundingAreas(void) const
{
	return Are;
}

CreatureRelation NormalCreature::RelationwithActor(void) const
{
	return relation_;
}

void NormalCreature::Attack1(double)
{

}