#pragma once
#include<OWE.h>
#include"Creature.h"
enum class  CreatureRelation
{
	HOSTILE = 1,
	NEUTRAL,
	FRIENDLY
};

class NormalCreature : Creature
{
public:
	enum class Status
	{
		DEAD = 1,
		FLOATING,
		ATTACKING,
		FROOZEN,
		
	};


	NormalCreature(int, glm::vec2, glm::vec2, OWE::BoundingArea &, std::string);


	//add constructor with all 
	~NormalCreature(void);// May add kill here
	void SetHp(int);
	void Update(glm::vec2);
	void Draw(void);// temp Void need add 
	std::vector<OWE::BoundingArea> GetBoundingAreas(void) const;

	void Attack1(double);

	void setArea(OWE::BoundingArea);


	CreatureRelation RelationwithActor(void) const;

	void SetRelation(CreatureRelation);

	//void deleteArea(OWE::BoundingArea);

	//setPosition
	//

private:
	int hp_;
	OWE::BoundingArea area_;//onluAABB

	glm::vec2 lp_, rp_;
	std::vector<OWE::BoundingArea> Are;

	OWE::Texture2D texture_;
	CreatureRelation relation_;
	double attack1CoolDown_;
	Status status;
	//weapon
	//position_ + velocity_
	// using weapon to add attack mode

	//0.012f //0.025f
};