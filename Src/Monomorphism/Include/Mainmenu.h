#pragma once

#include<OWE.h>
#include<fstream>
#include<vector>

#include "KeyDown.h"

namespace menu
{
	using namespace OWE;
	class Mainmenu
	{
	public:
		Mainmenu(const std::string&);
		//~Mainmenu();
		void callmainloop(ScreenScale &screenscale);
	private:
		bool _down = 0;
		std::vector<Texture2D> loadpic_;
		void draw(ScreenScale &screenscale);
		const int fuctionnumber = 3; 
		const int textnum = 3;
		int selected = 0; //1 ~ 4, 0表示一个特殊的textture
		//void _functionup();
		//void _functiondown();
		bool runselectedfunctions();
		void mainloop(ScreenScale &screenscale);
		void input();

        KeyDown up_;
        KeyDown down_;
	};

	inline Mainmenu::Mainmenu(const std::string &configfile)
	{
		loadpic_.clear();
		std::ifstream fin(configfile);
		for (int i = 0; i < textnum; i++)
		{
			std::string filepath;
			float tmpo, tmpq, tmpo1, tmp2;
            fin >> filepath;
			Texture2D tmpTeux;
			_Texture2DAux::Desc des;
			_Texture2DAux::_LoadTexture2DFromFile(filepath, des, tmpTeux);
			
			loadpic_.push_back(std::move(tmpTeux));

			selected = 0;
		}

	}
}

