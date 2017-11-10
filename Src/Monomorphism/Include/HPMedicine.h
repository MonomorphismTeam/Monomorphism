#ifndef __HPMedicine_H__
#define __HPMedicine_H__

#include "Include\Item.h"
#include<OWE.h>
#include <Texture2D.h>

class HPMedicine: Item
{
public:
	double StayTime;
	HPMedicine(double _StayTime, bool _Dead, bool _Broken, glm::vec2 _lp, glm::vec2 _rp, OWE::BoundingArea &_area, std::string filepath);
	// Dead表示共有的属性标示装备与否，Broken表示StayTime=0
	bool Dead;
	bool Broken;
	glm::vec2 lp;
	glm::vec2 rp;
	OWE::Texture2D _texture;
	void SetPosition(glm::vec2 _lp, glm::vec2 _rp);
	void Update(void);
	void Draw(OWE::ScreenScale &screenscale);
	 std::vector<OWE::BoundingArea> GetBoundingAreas(void) const = 0;
	void InteractWithActor(Actor *actor) = 0;
};

#endif //_HPMedicine_