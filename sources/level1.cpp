/*
 * Contains level 1 functions
 */

#include "level1.h"


/*Level1::Level1()
{
	Level();
	asteroidRate = 1000;
	lastTimeAsteroid = 0;
}*/

void Level1::loadLevel()
{
	loadConf();
	loadObject();
	hud = new HUD();
	hud->ge = ge;
	hud->loadHUDElements("conf/hud.conf");
	cameraSpeed = 1;
	asteroidRate = 1000;
	lastTimeAsteroid = 0;
	hero = new Hero();
}

void Level1::loadObject()
{
	loadBackGround();
	loadTextures();
}

//Load all textures used for the level at the beginning
//Texture and objects are specified in the level configuration file
void Level1::loadTextures()
{
	for (map<string, vector<string> >::iterator anElement = configurationElements.begin(); anElement != configurationElements.end(); ++anElement)
	{
		ge->textures.insert(make_pair(anElement->first, ge->loadTexture((anElement->second).back())));
	}
}

void Level1::loadBackGround()
{
	string path = "res/decor.png";
	bg.texture = ge->loadTexture(path);
	bg.width = SCREEN_WIDTH;
	bg.height = GAMEZONE_HEIGHT;
	bg.posX = 0;
	bg.posY = 0;
	bg.state = 0;
	bg.animX = 0;
	bg.animY = 0;
	activeElements.push_back(&bg);
}


void Level1::drawLevel()
{
	checkEvent();

	pe->stayOnScreen(hero, make_pair(SCREEN_WIDTH, GAMEZONE_HEIGHT));

	for (std::list<Drawable *>::iterator anElement = activeElements.begin() ; anElement != activeElements.end(); ++anElement)
	{
		(*anElement)->animate();
		if((*anElement)->display)
		{
			ge->toDisplay.push_back(*anElement);
		}
	}
	hud->displayUI();
	hud->displayHealth(hero->life);
	hero->animate();
	bg.animX = bg.animX + cameraSpeed;

}


int first = 1;
//function that handle the events (enemies apparitions, collision checks,  etc...)
void Level1::checkEvent()
{
	if (first)
	{
		createWave1();
		first = 0;
	}

	for (std::list<Drawable *>::iterator anElement = activeElements.begin() ; anElement != activeElements.end(); ++anElement)
		{
			if((*anElement)->toRemove)
			{
				activeElements.erase(anElement++);
			}
			else
			{
				if((*anElement)->isEnemy())
				{
					checkEnemyCollision(*anElement);
					Enemy * anEnemy = static_cast<Enemy *>(*anElement);
					anEnemy->fire();
				}
				if((*anElement)->isBonus() ||(*anElement)->isLaser())
				{
					checkCollision(*anElement);
				}
			}
		}

	//Generate Asteroid ?
	/*if(generateAsteroid())
	{
		activeElements.push_back(new Asteroid(ASTER_NORMAL));
	}*/

}

void Level1::createWave1()
{
	int i;
	int xc[5] = {1040, 1140, 1140, 1240, 1240};
	int yc[5] = {180, 290, 90, 350, 10};
	//int xc2[5] = {1340, 1740, 1740, 1840, 1840};
	//int yc2[5] = {170, 290, 90, 350, 10};
	int xc3[5] = {2540, 2740, 2740, 2840, 2840};
	int yc3[5] = {170, 290, 90, 350, 10};
	int xc2[5] = {4040, 4140, 4140, 4240, 4240};

	//int xc4[4] = {1340, 1440, 1540, 1640};
	int xc4[5] = {1840, 1960, 2080, 2200, 2320};
	int yc4[1] = {0};
	int yc4bis[1] = {GAMEZONE_HEIGHT - 100};
	int xc5[5] = {3340, 3460, 3580, 3700, 3820};

	for (i=0; i< 5; i++)
	{
		activeElements.push_back(new Enemy(xc[i], yc[i], 0, 1));
		activeElements.push_back(new Enemy(xc2[i], yc[i], 2, 1));
		activeElements.push_back(new Enemy(xc3[i], yc3[i], 1, 1));
		//activeElements.push_back(new Patrol(xc4[i], yc4bis[0], 0, UP));
		activeElements.push_back(new Patrol(xc4[i], yc4[0], 0, DOWN));
		activeElements.push_back(new Patrol(xc5[i], yc4bis[0], 0, UP));
		activeElements.push_back(new Patrol(xc5[i], yc4[0], 0, DOWN));
		//activeElements.push_back(new Asteroid(ASTER_NORMAL));
		//^'50;5;640;170;740;290;740;90;840;10;840;350'
	}
}

int Level1::checkEnemyCollision(Drawable * anElement)
{
	if(pe->collisionDetection(hero, anElement))
	{
		if(!hero->invincible)
		{
			anElement->processCollisionWith(hero);
			hero->processCollisionWith(anElement);
			return 1;
		}
	}

	for (list<Laser*>::iterator aLaser = hero->lasers.begin(); aLaser != hero->lasers.end(); ++aLaser)
	{
		Laser * aL = *aLaser;
		if(pe->collisionDetection(aL, anElement))
		{
			anElement->processCollisionWith(aL);
			aL->toRemove = TRUE;
			hero->lasers.erase(aLaser++);
			return 1;
		}
	}
	return 0;
}

int Level1::checkCollision(Drawable * anElement)
{
	if(pe->collisionDetection(hero, anElement))
	{
		anElement->processCollisionWith(hero);
		hero->processCollisionWith(anElement);
		return 1;
	}
	return 0;
}

void Level1::createExplosion(int x, int y, int type)
{
	activeElements.push_back(new Explosion(x, y, type));
}

void Level1::createBonus(int x, int y, int type)
{
	activeElements.push_back(new Bonus(x, y, type));
}

//Load all the configuration elements from a text file
//Format: elementName;width;height;listOfAnimationStatus$nbOfFramesInAnAnimation;pathToTextureFile
void Level1::loadConf()
{
	ifstream file;
	string line;
	string fileName = "conf/l1.conf";
	string token;
	string type;
	vector<string> confElements;
	string confElement;

	file.open(fileName.c_str());
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
}

int Level1::generateAsteroid()
{
	unsigned int thing = lastTimeAsteroid + asteroidRate;
	if (thing <  SDL_GetTicks())
	{
		lastTimeAsteroid = SDL_GetTicks();
		return TRUE;
	}
	return FALSE;
}
