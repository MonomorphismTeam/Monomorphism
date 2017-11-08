#pragma once
//#pragma once
#include <OWE.h>
#include "BlockArea.h"

#define __NORMAL_BLOCK_H__

class NormalBlock : BlockArea
{
public:
	enum class STATUS {
		IsBlock = 1,
		NotBlock
	};
	NormalBlock(std::string);
	~NormalBlock();
	void Draw(OWE::ScreenScale&);
	std::vector<OWE::BoundingArea> GetBoundingAreas(void);
	void AddArea(OWE::BoundingArea, glm::vec2, glm::vec2);
	//bool deleteArea(OWE::BoundingArea);
	void SetTexture(std::string filename);

private:
	OWE::Texture2D blockTexture;
	void _AddAreaPoint(glm::vec2 &, glm::vec2 &);

	std::vector<OWE::BoundingArea> blockarea;
	std::vector<glm::vec2> _lpSet;
	std::vector<glm::vec2> _rpSet;
	STATUS Status;

};

