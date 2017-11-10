#include "Include\HPMedicine.h"
#include "Include\Actor.h"
#include <Texture2D.h>
#include <OWE.h>

HPMedicine::HPMedicine(double _StayTime, bool _Dead, bool _Broken, glm::vec2 _lp, glm::vec2 _rp, OWE::BoundingArea &_area, std::string filepath)
{
	StayTime = _StayTime;
	Dead = _Dead;
	Broken = _Broken;
	lp = _lp;
	rp = _rp;
	OWE::_Texture2DAux::Desc tmp;
	OWE::_Texture2DAux::_LoadTexture2DFromFile(filepath, tmp, _texture);
}

void HPMedicine::Update()
{
	StayTime --;
	if ( StayTime < 0 )
		HPMedicine::Broken = true;
}



void HPMedicine::SetPosition(glm::vec2 _lp, glm::vec2 _rp) 
{
	rp = _rp;
	lp = _lp;
}

void HPMedicine:: InteractWithActor(Actor *actor)
{
	if(HPMedicine::Broken != 0)
		//Actor::AddHP();
}

void HPMedicine::Draw(OWE::ScreenScale &screenscale) 
{
	if (StayTime > 0) 
	{
		OWE::ImmediateRenderer::DrawTexturedBox(lp, rp, glm::vec2(0,0), glm::vec2(1,1), _texture.GetTextureView(), screenscale);
	}

}