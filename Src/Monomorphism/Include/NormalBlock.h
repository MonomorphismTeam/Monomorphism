#pragma once
//#pragma once
#include <OWE.h>
#include "BlockArea.h"

#define __NORMAL_BLOCK_H__

class NormalBlock : public BlockArea
{
public:
	enum class STATUS {
		IsBlock = 1,
		NotBlock
	};
	NormalBlock(std::string);
	~NormalBlock();
    void Update(double time);
	void Draw(const OWE::ScreenScale&);
	std::vector<OWE::BoundingArea> GetBoundingAreas(void) const;
	void AddArea(OWE::BoundingArea, glm::vec2, glm::vec2);
	//bool deleteArea(OWE::BoundingArea);
	void SetTexture(std::string filename);

    bool IsDead(void) const;

private:
	OWE::Texture2D blockTexture;
	void _AddAreaPoint(glm::vec2 &, glm::vec2 &);

	std::vector<OWE::BoundingArea> blockarea;
	std::vector<glm::vec2> _lpSet;
	std::vector<glm::vec2> _rpSet;
	STATUS Status;

};

