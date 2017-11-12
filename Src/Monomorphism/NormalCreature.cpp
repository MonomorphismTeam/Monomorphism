#include"Include\NormalCreature.h"
#include<OWE.h>

NormalCreature::NormalCreature(int hp, glm::vec2 lp, glm::vec2 rp,
	OWE::BoundingArea &_area, std::string filepath)
	: area_(_area)
{
	lp_ = lp;
	rp_ = rp;
	hp_ = 0;
	area_ = _area;
	Are.push_back(_area);
	status = NormalCreature::Status::STANDING;
	standingtime = 0;
	//loading Texture;
	OWE::_Texture2DAux::Desc tmp;
	OWE::_Texture2DAux::_LoadTexture2DFromFile(filepath, tmp, texture_);
}

NormalCreature::~NormalCreature(void)
{
	Are.clear();
	//improve
}

void NormalCreature::Update(glm::vec2 playerPoi, double time)
{
	//Improve
	//move towards 
	if (status == NormalCreature::Status::STANDING)
		_updateSTANDING(playerPoi, time);
	else if (status == NormalCreature::Status::FLOATING)
		_updateFLOATING(playerPoi, time);
	else if (status == NormalCreature::Status::DEAD)
		{_updateDEAD(time);return;}
	//gravity;
	//attack;
	if (attack1CoolDown_ < 0) 
	{
		Attack1();
	}
	attack1CoolDown_ -= time;
	if (status == NormalCreature::Status::STANDING)standingtime += time;

	velocity_ -= glm::vec2(0, time * 0.025);

	if (hp_ <= 0) status = NormalCreature::Status::DEAD;
}
// ---- update---------
void NormalCreature::_updateSTANDING(glm::vec2 playerPoi, double time)
{
	const float xSpeed = 0.012;
	if (relation_ == CreatureRelation::HOSTILE )
	{
		glm::vec2 temp(0, 0);
		if (playerPoi.x > (lp_.x))
		{
			velocity_.x = xSpeed;
		}
		else
		{
			velocity_.x = -xSpeed;
		}

		//转化为跳跃状态判定
		if (playerPoi.y < (lp_.y) && standingtime > standtim)
		{
			velocity_.y = xSpeed;
			standingtime = 0;
			status = NormalCreature::Status::FLOATING;
		}
	}
}

void NormalCreature::_updateFLOATING(glm::vec2 playerPoi, double time)
{
	const float xSpeed = 0.012;
	if (relation_ == CreatureRelation::HOSTILE)
	{
		glm::vec2 temp(0, 0);
		if (playerPoi.x > (lp_.x))
		{
			velocity_.x = xSpeed;
		}
		else
		{
			velocity_.x = -xSpeed;
		}
	}//change status in main loop
}


void NormalCreature::_updateDEAD(double time)
{
	//play dead anime        
	deadlasttime -= time;
	if (deadlasttime < 0)
	{
		//improve : dead here
	}
	//~NormalCreature();
}

// update ends

void NormalCreature::Setposition(glm::vec2 poi)
{
	glm::vec2 lp1 = lp_;
	glm::vec2 rp1 = rp_;
	lp_ = poi;
	rp_ = poi + (rp1 - lp1);
}
void NormalCreature::Draw(const OWE::ScreenScale &screenscale)
{
	if (velocity_.x > 0)//heading left
	{
		OWE::ImmediateRenderer::DrawTexturedBox(lp_, rp_, glm::vec2(0, 0), glm::vec2(1, 1),
			texture_.GetTextureView(), screenscale);
	}
	else
	{
		OWE::ImmediateRenderer::DrawTexturedBox(lp_, rp_, glm::vec2(1, 0), glm::vec2(0, 1),
			texture_.GetTextureView(), screenscale);
	}
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

void NormalCreature::Attack1()
{
	const double attack1Col = 2500; //2500ms default
	//improve : create damage area
	

	//
	attack1CoolDown_ = attack1Col;
}