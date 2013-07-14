/*
 * Contains the menu function
 */

#include "menu.h"

Menu::Menu(Ge * aGe, SoundEngine * aSe)
{
	ge = aGe;
	soundEngine = aSe;
	menuColor = ge->availableColors.at("WHITE");
	menuFont = ge->availableFonts.at(0);
	selected = 0;
}

void Menu::loadMenu()
{
	bg = new Drawable();
	bg->texture = ge->loadTexture("res/titre_wide.png");
	bg->width = 1200;
 	bg->height = 600;

 	arrow = new Drawable();
 	arrow->texture = ge->loadTexture("res/zombix2.png");
 	arrow->width = 48;
 	arrow->height = 52;
 	arrow->nbFrames.clear();
 	arrow->nbFrames.push_back(4);

	loadMenuElements("conf/menu.conf");

	arrow->posX = menuElements.at(0)->posX - 70;
	arrow->posY = menuElements.at(0)->posY - 10;
}

void Menu::loadIntro()
{
	logo = new Drawable();
	logo->texture = ge->loadTexture("res/logo.png");
	logo->width = 1200;
 	logo->height = 480;

 	bbox = new Drawable();
 	//bbox->texture = SDL_CreateRGBSurface(0, 1200, 480, 24, 0x00000000, 0x00000000, 0x00000000, 0);
 	//SDL_SetSurfaceBlendMode(bbox->texture, SDL_BLENDMODE_BLEND);
 	bbox->texture = ge->loadTexture("res/bbox.png");
 	bbox->width = 1200;
 	bbox->height = 480;
 	bbox->animX = 0;
 	bbox->animY = 0;
	bbox->texture = SDL_DisplayFormat(bbox->texture);

 	startIntro = SDL_GetTicks();
 	endIntro = startIntro + introLength;
}

void Menu::playIntro()
{
	Uint8 alpha = 0;
	int diff = endIntro - SDL_GetTicks();
	while (diff > 0)
	{
		ge->toDisplay.push_back(logo);

		if(diff<1000)
		{
			alpha = min(255, alpha + 35);
			SDL_SetAlpha(bbox->texture, SDL_SRCALPHA, alpha);
			ge->toDisplay.push_back(bbox);
		}

	   	ge->drawFrame();

	   	diff = endIntro - SDL_GetTicks();
	}
}

void Menu::displayMenu()
{
	ge->toDisplay.push_back(bg);
	for (std::vector<Text *>::iterator anElement = menuElements.begin() ; anElement != menuElements.end(); ++anElement)
	{
		(*anElement)->animate();
		if((*anElement)->display)
		{
			ge->toDisplay.push_back(*anElement);
		}
	}
	arrow->updateAnimationFrame();
	ge->toDisplay.push_back(arrow);
}

void Menu::loadMenuElements(string path)
{
	//parse the configuration file
	ifstream file;
	string line;
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
		Text * tmp = new Text(menuColor, menuFont);

		tmp->width = 300;
		tmp->height = 300;
		tmp->posX = atoi(((configurationElements.at(anElement->first)).at(0)).c_str());
		tmp->posY = atoi(((configurationElements.at(anElement->first)).at(1)).c_str());
		tmp->write(anElement->first);

		menuElements.push_back(tmp);
	}

	sort(menuElements.begin(), menuElements.end(), sortElement);
}

void Menu::selectUp()
{
	menuElements.at(selected)->setColor(ge->availableColors.at("WHITE"));
	selected = (selected - 1 + menuElements.size()) % menuElements.size();
	arrow->posY = menuElements.at(selected)->posY - 10;
	menuElements.at(selected)->setColor(ge->availableColors.at("RED"));
}

void Menu::selectDown()
{
	menuElements.at(selected)->setColor(ge->availableColors.at("WHITE"));
	selected = (selected + 1) % menuElements.size();
	arrow->posY = menuElements.at(selected)->posY - 10;
	menuElements.at(selected)->setColor(ge->availableColors.at("RED"));
}

void Menu::select()
{
	if(selected == 0)
	{
		game->launchLevel("level1");
		return;
	}

	stringstream ss;
	ss <<"level"<<selected;
	game->launchLevel(ss.str());
}

bool sortElement(Text *a, Text* b)
{
	return (a->posY < b->posY);
}
