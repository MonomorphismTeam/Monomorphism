#include <cstdlib>
#include <OWE.h>
#include "Include\Mainmenu.h"
#include "Include\World.h"

using namespace menu;

void Mainmenu::callmainloop(ScreenScale &screenscale)
{
	_down = 0;
	selected = 1;
	mainloop(screenscale);
}

void Mainmenu::draw(ScreenScale &screenscale)
{
    OWE::RenderContext::GetInstance().ClearColorAndDepth();
    OWE::ImmediateRenderer::DrawTexturedBox(glm::vec2(0.0f), glm::vec2(screenscale.ScreenWidth(), screenscale.ScreenHeight()),
        glm::vec2(0.0f), glm::vec2(1.0f), loadpic_[selected], screenscale);
    OWE::RenderContext::GetInstance().Present();
	//improve: select + background;
}

bool Mainmenu::runselectedfunctions()
{
    _down = false;
	if (selected == 0)
	{
        //创建世界
        if(World::IsInstanceAvailable())
            World::DelInstance();
        World::InitInstance();
        World::GetInstance().InitializeScene(128, 0);
        World::GetInstance().Run();
        return true;
	}
	else if (selected == 1)
	{
		//LOAD
        return true;
	}
	else if (selected == 2)
	{
        return false;
	}
}

void Mainmenu::mainloop(ScreenScale &screenscale)
{
    do
    {
        while(!_down)
        {
            draw(screenscale);
            OWE::RenderContext::GetInstance().DoEvents();
            input();
        }
    } while(runselectedfunctions());
}

void Mainmenu::input()
{
    InputManager &im = InputManager::GetInstance();
	if (up_.Update(im.IsKeyPressed(KEY_CODE::KEY_UP)))
	{
        selected--;
        if(selected < 0)
            selected += fuctionnumber;
	}else
	if (down_.Update(im.IsKeyPressed(KEY_CODE::KEY_DOWN)))
	{
        selected++;
        selected %= fuctionnumber;
	}else
	if (im.IsKeyPressed(KEY_CODE::KEY_ENTER))
	{
		_down = 1;
	}
}
