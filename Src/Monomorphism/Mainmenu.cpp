#include "Include\mainmenu.h"
#include <cstdlib>
using namespace menu;

void Mainmenu::callmainloop(ScreenScale &screenscale)
{
	_down = 0;
	selected = 1;
	mainloop(screenscale);
}

void Mainmenu::draw(ScreenScale &screenscale)
{
	for (int i = 1; i < textnum; i++)
	{
		OWE::ImmediateRenderer::DrawTexturedBox(lp_[i], rp_[i], glm::vec2(0, 0), glm::vec2(1, 1),
			loadpic_[i].GetTextureView(), screenscale);
	}
	//improve: select + background;
}

void Mainmenu::runselectedfunctions()
{
	if (selected == 0)
	{
		//PLAY
	}
	else if (selected == 1)
	{
		//LOAD
	}
	else if (selected == 2)
	{
		//exit
		//
	}
}

void Mainmenu::mainloop(ScreenScale &screenscale)
{
	while (_down)
	{
		draw(screenscale);
		input();
	}
	runselectedfunctions();
}

void Mainmenu::input()
{
	if (im.IsKeyPressed(KEY_CODE::KEY_UP))
	{
		selected++;
		selected %= fuctionnumber;
	}else
	if (im.IsKeyPressed(KEY_CODE::KEY_DOWN))
	{
		selected--;
		if(selected <=0 )
		selected += fuctionnumber;
	}else
	if (im.IsKeyPressed(KEY_CODE::KEY_ENTER))
	{
		_down = 1;
	}
}