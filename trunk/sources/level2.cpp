/*
 * Contains level 2 functions
 */

#include "level2.h"

void Level2::loadLevel()
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

void Level2::loadObject()
{
	loadBackGround();
	loadTextures();
}

//Load all textures used for the level at the beginning
//Texture and objects are specified in the level configuration file
void Level2::loadTextures()
{
	for (map<string, vector<string> >::iterator anElement = configurationElements.begin(); anElement != configurationElements.end(); ++anElement)
	{
		ge->textures.insert(make_pair(anElement->first, ge->loadTexture((anElement->second).back())));
	}
}

void Level2::loadBackGround()
{
	string path = "res/decor.png";
	bg.texture = ge->loadTexture(path);
	bg.width = SCREEN_WIDTH;
	bg.height = SCREEN_HEIGHT;
	bg.posX = 0;
	bg.posY = 0;
	bg.state = 0;
	bg.animX = 0;
	bg.animY = 0;
	activeElements.push_back(&bg);
}


void Level2::drawLevel()
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

//function that handle the events (enemies apparitions, collision checks,  etc...)
void Level2::checkEvent()
{
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
	if(generateAsteroid())
	{
		activeElements.push_back(new Asteroid(ASTER_NORMAL));
	}

}


int Level2::checkEnemyCollision(Drawable * anElement)
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

int Level2::checkCollision(Drawable * anElement)
{
	if(pe->collisionDetection(hero, anElement))
	{
		anElement->processCollisionWith(hero);
		hero->processCollisionWith(anElement);
		return 1;
	}
	return 0;
}

void Level2::createExplosion(int x, int y, int type)
{
	activeElements.push_back(new Explosion(x, y, type));
}

void Level2::createBonus(int x, int y, int type)
{
	activeElements.push_back(new Bonus(x, y, type));
}

//Load all the configuration elements from a text file
//Format: elementName;width;height;listOfAnimationStatus$nbOfFramesInAnAnimation;pathToTextureFile
void Level2::loadConf()
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

int Level2::generateAsteroid()
{
	unsigned int thing = lastTimeAsteroid + asteroidRate;
	if (thing <  SDL_GetTicks())
	{
		lastTimeAsteroid = SDL_GetTicks();
		return TRUE;
	}
	return FALSE;
}
