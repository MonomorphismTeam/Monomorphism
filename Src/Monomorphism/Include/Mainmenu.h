#pragma once
#include<OWE.h>
#include<fstream>
#include<vector>

namespace menu
{
	using namespace OWE;
	class Mainmenu
	{
	public:
		Mainmenu(std::string);
		//~Mainmenu();
		void callmainloop(ScreenScale &screenscale);
	private:
		bool _down = 0;
		std::vector<Texture2D> loadpic_;
		std::vector<glm::vec2> lp_;
		std::vector<glm::vec2> rp_;
		void draw(ScreenScale &screenscale);
		const int fuctionnumber = 3; 
		const int textnum = 4;
		int selected = 0; //1 ~ 4, 0表示一个特殊的textture
		//void _functionup();
		//void _functiondown();
		void runselectedfunctions();
		void mainloop(ScreenScale &screenscale);
		InputManager &im = InputManager::GetInstance();
		void input();
	};

	Mainmenu::Mainmenu(std::string configfile)
	{
		loadpic_.clear(); lp_.clear(); rp_.clear();
		std::ifstream fin(configfile);
		for (int i = 0; i < fuctionnumber; i++)
		{
			std::string filepath;
			float tmpo, tmpq, tmpo1, tmp2;
			fin >> filepath >> tmpo >> tmpq >> tmpo1>> tmp2;
			Texture2D tmpTeux;
			_Texture2DAux::Desc des;
			_Texture2DAux::_LoadTexture2DFromFile(filepath, des, tmpTeux);
			
			loadpic_.push_back(tmpTeux);
			lp_.push_back(glm::vec2(tmpo, tmpq));
			rp_.push_back(glm::vec2(tmpo1, tmp2));

			selected = 1;
		}
	}

}

