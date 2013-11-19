/*
 * Contains the menu function
 */

#include "hud.h"

HUD::HUD(GraphicEngine * aGraphicEngine)
{

	ge = aGraphicEngine;
	loadHUDElements("conf/hud.conf");

	backGround = hudElements.at("hud_bkg");
	backGround->width = SCREEN_WIDTH;
	backGround->height = SCREEN_HEIGHT - GAMEZONE_HEIGHT;
	backGround->setAnimX(0);
	backGround->setAnimY(0);

	//TODO put the next two elements in the conf file
	lifeHUD = new Text(ge->availableColors.at("BLUE"), ge->availableFonts.at("lCrystal"));
	lifeHUD->width = 300;
	lifeHUD->height = 300;
	lifeHUD->posX = 820;
	lifeHUD->posY = 758;

	scoreHUD = new Text(ge->availableColors.at("BLUE"), ge->availableFonts.at("lCrystal"));
	scoreHUD->width = 300;
	scoreHUD->height = 300;
	scoreHUD->posX = 1020;
	scoreHUD->posY = 758;

	health = hudElements.at("hud_gauge");
	massLoad = hudElements.at("hud_mass");
	radioLoad = hudElements.at("hud_radio");

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

		hudElements.insert(make_pair(anElement->first, tmp));
	}

}

void HUD::displayUI()
{
	backGround->processDisplay();

	ge->toDisplay.push_back(radioLoad);
}

//Display the current health of the player
void HUD::displayHealth(int healthValue)
{
	static float i = 0.0;

	if(i < healthValue * 3)
	{
		i = i + 0.3;
	}
	if (i > healthValue * 3)
	{
		i = i - 0.3;
	}
	health->setAnimX((int)(i) * health->width);

	health->processDisplay();
}

void HUD::displayLife(int nbLife)
{
	stringstream ss;
	ss << nbLife;
	lifeHUD->write(ss.str());

	lifeHUD->processDisplay();
}

void HUD::displayScore(int score)
{
	stringstream ss;
	ss << score;
	scoreHUD->write(ss.str());

	scoreHUD->processDisplay();
}


void HUD::displayMassPotential(float massPo)
{
	static float i = 0.0;

	if(i < massPo)
	{
		i = i + 0.3;
	}
	if (i > massPo)
	{
		i = i - 0.3;
	}
	massLoad->setAnimY((int)(i) * massLoad->height);

	massLoad->processDisplay();
}

void HUD::displayRadioPotential(float radioPo)
{
	static float i = 0.0;

	if(i < radioPo)
	{
		i = i + 0.3;
	}
	if (i > radioPo)
	{
		i = i - 0.3;
	}
	radioLoad->setAnimY((int)(i) * radioLoad->height);

	radioLoad->processDisplay();
}

