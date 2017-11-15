#include <cstdlib>
#include <fstream>
#include <OWE.h>

#include "Include\Mainmenu.h"
#include "Include\ResourceNames.h"
#include "Include\World.h"

using namespace menu;

void Mainmenu::callmainloop(ScreenScale &screenscale)
{
	_down = 0;
	selected = 0;
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
        World::GetInstance().InitializeScene(static_cast<SceneGenerator::SeedType>(rand()), 0);
        World::GetInstance().Run();
        return true;
	}
	else if (selected == 1)
	{
        std::ifstream fin(SAVE_FILENAME, std::ifstream::in);
        if(!fin)
            throw OWE::FatalError("Failed to load saving file: " + std::string(SAVE_FILENAME));
        SceneGenerator::SeedType worldSeed;
        World::StageNumber stage;
        if(!(fin >> worldSeed >> stage))
            throw OWE::FatalError("Save is broken!");
        fin.close();

        if(World::IsInstanceAvailable())
            World::DelInstance();
        World::InitInstance();
        World::GetInstance().InitializeScene(worldSeed, stage);
        World::GetInstance().Run();
        return true;
	}
	else if (selected == 2)
	{
        return false;
	}
    return true;
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
