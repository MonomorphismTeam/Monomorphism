#include "Include\NormalBlock.h"
#include<OWE.h>

NormalBlock::NormalBlock(std::string filename)
{
	blockarea.clear();
	_lpSet.clear();
	_rpSet.clear();
	SetTexture(filename);
	Status = STATUS::IsBlock;
	//improve
}

NormalBlock::~NormalBlock()
{
	//may play dead action
	blockTexture.~_Texture2D();
	blockarea.clear();
}

void NormalBlock::AddArea(OWE::BoundingArea p, glm::vec2 lp, glm::vec2 rp)// p`s type must be AABB
{
	blockarea.push_back(p);
	_AddAreaPoint(lp, rp);
}

void NormalBlock::SetTexture(std::string filename)
{
	OWE::_Texture2DAux::Desc tmp;
	OWE::_Texture2DAux::_LoadTexture2DFromFile(filename, tmp, blockTexture);
}

std::vector<OWE::BoundingArea> NormalBlock::GetBoundingAreas(void)
{
	return blockarea;
}

void NormalBlock::Draw(OWE::ScreenScale &screenscale)
{
	//now
	std::vector<OWE::BoundingArea>::iterator it;
	std::vector<glm::vec2>::iterator lpit = _lpSet.begin(), rpit = _rpSet.begin();
	for (it = blockarea.begin(); it !=blockarea.end(); it++)
	{
		if (it->GetType() == OWE::BoundingArea::Type::AABB)
		{
			OWE::ImmediateRenderer::DrawTexturedBox(*(lpit), *(rpit), glm::vec2(0,0), glm::vec2(1,1), 
				blockTexture.GetTextureView(), screenscale);
		}
	}
}

void NormalBlock::_AddAreaPoint(glm::vec2 &lp, glm::vec2 &rp)
{
	_lpSet.push_back(lp);
	_rpSet.push_back(rp);
}