/*================================================================
Filename: SimpleAABBBlock.cpp
Date: 2017.11.13
Created by AirGuanZ
================================================================*/
#include "Include\SimpleAABBBlock.h"

using namespace glm;
using namespace OWE;

SimpleAABBBlock::SimpleAABBBlock(const vec2 &LB, const vec2 &RT, Texture2DView tex)
    : LB_(LB), RT_(RT), tex_(tex)
{

}

SimpleAABBBlock::~SimpleAABBBlock(void)
{
    //do nothing
}

void SimpleAABBBlock::Update(double time)
{
    //do nothing
}

void SimpleAABBBlock::Draw(const ScreenScale &scale)
{
    ImmediateRenderer::DrawTexturedBoxWithScreenTrans(
        LB_, RT_, vec2(0.0f), vec2(1.0f),
        tex_, scale,
        ImmediateRenderer::RenderMode::AlphaTest);
}

std::vector<OWE::BoundingArea> SimpleAABBBlock::GetBoundingAreas(void) const
{
    return { BoundingArea(BoundingArea::AABB(LB_.x, LB_.y, RT_.x, RT_.y)) };
}

bool SimpleAABBBlock::IsDead(void) const
{
    return false;
}
