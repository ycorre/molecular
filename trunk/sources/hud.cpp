/*
 * Contains the menu function
 */

#include "hud.h"

HUD::HUD()
{
	string path = "res/ui.png";
	backGround.texture = ge->loadTexture(path);
	backGround.width = SCREEN_WIDTH;
	backGround.height = SCREEN_HEIGHT - GAMEZONE_HEIGHT;
	backGround.posX = 0;
	backGround.posY = GAMEZONE_HEIGHT;
	backGround.state = 0;
	backGround.animX = 0;
	backGround.animY = 0;

}

void HUD::loadHUDElements(string path)
{
	//parse the configuration file
	ifstream file;
	string line;
	string fileName = "conf/hud.conf";
	string token;
	string type;
	vector<string> confElements;
	string confElement;

	file.open(path.c_str());
	while(getline(file, line))
	{
		if(line.size()!=0) //Ignore empty lines
		{
			confElements.clear();
			istringstream myLine(line);
			getline(myLine, type, ';');

			while(getline(myLine, token, ';'))
			{
				confElements.push_back(token);
			}
			configurationElements.insert(make_pair(type, confElements));
		}
	}

	//instantiate the corresponding drawable elements
	for (std::map<string, vector<string> >::iterator anElement = configurationElements.begin() ; anElement != configurationElements.end(); ++anElement)
	{
		Drawable * tmp = new Drawable();

		tmp->width = atoi(((configurationElements.at(anElement->first)).at(0)).c_str());
		tmp->height = atoi(((configurationElements.at(anElement->first)).at(1)).c_str());
		tmp->posX = atoi(((configurationElements.at(anElement->first)).at(2)).c_str());
		tmp->posY = GAMEZONE_HEIGHT + atoi(((configurationElements.at(anElement->first)).at(3)).c_str());
		tmp->texture = ge->loadTexture((configurationElements.at(anElement->first)).at(4));

		hudElements.insert(make_pair(anElement->first, *tmp));
		//ge->textures.insert(make_pair(anElement->first, ge->loadTexture((anElement->second).back())));
	}

}

void HUD::displayUI()
{
	ge->toDisplay.push_back(&backGround);
}

void HUD::displayHealth(int life)
{
	int i;
	for(i=0; i< life; i++)
	{
		stringstream ss;
		ss <<"life"<<i+1;
		ge->toDisplay.push_back(&hudElements.at(ss.str()));
	}
}

