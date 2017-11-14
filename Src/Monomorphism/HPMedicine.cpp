#include "Include\HPMedicine.h"
#include "Include\Actor.h"
#include <Texture2D.h>
#include <OWE.h>

HPMedicine::HPMedicine(double _StayTime, bool _Dead, bool _Broken, glm::vec2 _lp, glm::vec2 _rp, OWE::Texture2DView &t)
{
	StayTime = _StayTime;
	Dead = _Dead;
	Broken = _Broken;
	lp = _lp;
	rp = _rp;
    _texture = t;
}

void HPMedicine::Update(double time)
{
	StayTime -= time;
	if ( StayTime < 0 )
		HPMedicine::Broken = true;
}

std::vector<OWE::BoundingArea> HPMedicine::GetBoundingAreas(void) const
{
    OWE::BoundingArea::AABB area_(lp.x, lp.y, rp.x, rp.y);
    return {OWE::BoundingArea(area_)};
}

void HPMedicine::SetPosition(glm::vec2 _lp, glm::vec2 _rp) 
{
	rp = _rp;
	lp = _lp;
}

void HPMedicine:: InteractWithActor(Actor *actor)
{
    
    if(HPMedicine::Broken == 0)
    {
        actor->SetHP(actor->GetHP() + 40.0f);
    }
    Broken = true;
}

void HPMedicine::Draw(const OWE::ScreenScale &screenscale)  
{
	if (StayTime > 0) 
	{
		OWE::ImmediateRenderer::DrawTexturedBoxWithScreenTrans(lp, rp, glm::vec2(0,0),
            glm::vec2(1,1), _texture, screenscale,OWE::ImmediateRenderer::RenderMode::AlphaTest);
	}

}


bool HPMedicine::IsDead(void) const
{
    if(Broken) return true;
    else return false;

}