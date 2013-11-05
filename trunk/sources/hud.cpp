/*
 * Contains the menu function
 */

#include "hud.h"

HUD::HUD(GraphicEngine * aGraphicEngine)
{

	ge = aGraphicEngine;
	//TODO put in conf file ?
	backGround.width = SCREEN_WIDTH;
	backGround.height = SCREEN_HEIGHT - GAMEZONE_HEIGHT;
	string path = "res/ui.png";
	backGround.loadTexture(path);
	backGround.posX = 0;
	backGround.posY = GAMEZONE_HEIGHT;
	backGround.state = 0;
	backGround.setAnimX(0);
	backGround.setAnimY(0);

	//TODO put the next two elements in the conf file
	lifeHUD = new Text(ge->availableColors.at("WHITE"), ge->availableFonts.at(0));
	lifeHUD->width = 300;
	lifeHUD->height = 300;
	lifeHUD->posX = 500;
	lifeHUD->posY = 500;

	scoreHUD = new Text(ge->availableColors.at("WHITE"), ge->availableFonts.at(0));
	scoreHUD->width = 300;
	scoreHUD->height = 300;
	scoreHUD->posX = 800;
	scoreHUD->posY = 500;

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
	//Store them in a map where the key is a unique id given in the conf file
	for (std::map<string, vector<string> >::iterator anElement = configurationElements.begin() ; anElement != configurationElements.end(); ++anElement)
	{
		Drawable * tmp = new Drawable();

		tmp->width = atoi(((configurationElements.at(anElement->first)).at(0)).c_str());
		tmp->height = atoi(((configurationElements.at(anElement->first)).at(1)).c_str());
		tmp->posX = atoi(((configurationElements.at(anElement->first)).at(2)).c_str());
		tmp->posY = GAMEZONE_HEIGHT + atoi(((configurationElements.at(anElement->first)).at(3)).c_str());
		tmp->loadTexture((configurationElements.at(anElement->first)).at(4));

		hudElements.insert(make_pair(anElement->first, *tmp));
		//ge->textures.insert(make_pair(anElement->first, ge->loadTexture((anElement->second).back())));
	}

}

void HUD::displayUI()
{
	ge->toDisplay.push_back(&backGround);
}

//Display the current health of the player
void HUD::displayHealth(int life)
{
	int i;
	for(i=0; i< life; i++)
	{
		stringstream ss;
		ss <<"life"<< i+1;
		ge->toDisplay.push_back(&hudElements.at(ss.str()));
	}
}

void HUD::displayLife(int nbLife)
{
	stringstream ss;
	ss <<"Life: "<< nbLife;
	lifeHUD->write(ss.str());

	ge->toDisplay.push_back(lifeHUD);
}

void HUD::displayScore(int score)
{
	stringstream ss;
	ss <<"Score: "<< score;
	scoreHUD->write(ss.str());

	ge->toDisplay.push_back(scoreHUD);
}

