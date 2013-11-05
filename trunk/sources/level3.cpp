/*
 * Contains level 3 functions
 */

#include "level3.h"

void Level3::loadLevel(Hero * aHero)
{
	activeElements.clear();
	loadConf();
	loadObject();
	hud = new HUD(ge);
	hud->loadHUDElements("conf/hud.conf");
	cameraSpeed = 1;
	hero = aHero;
	hero->getTexture("tie");
	hero->width = atoi(((configurationElements.at("tie")).at(0)).c_str());
	hero->height = atoi(((configurationElements.at("tie")).at(1)).c_str());
	hero->nbFrames = parseAnimationState((configurationElements.at("tie")).at(2));
	hero->resetHero();
	theFalcon = new Falcon();
	levelState = LEVEL_PLAYING;
	activeElements.push_back(theFalcon);
	ending = fading = exiting = FALSE;
}

void Level3::loadObject()
{
	loadTextures();
	loadBackGround();
}

//Load all textures used for the level at the beginning
//Texture and objects are specified in the level configuration file
void Level3::loadTextures()
{
	for (map<string, vector<string> >::iterator anElement = configurationElements.begin(); anElement != configurationElements.end(); ++anElement)
	{
		ge->textures.insert(make_pair(anElement->first, ge->loadTexture((anElement->second).back())));
	}
}

void Level3::loadBackGround()
{
	Level::loadBackGround();
	string path = "res/decor.png";
	//bg.loadTexture(path);
	background.width = SCREEN_WIDTH;
	background.height = SCREEN_HEIGHT;
	background.posX = 0;
	background.posY = 0;
	background.state = 0;
	//background.setAnimX(0);
	//background.setAnimY(0);
	//activeElements.push_back(&bg);

	soundEngine->addSound("sound/xwing_explode.wav", "xwing_explode");
	soundEngine->addSound("sound/xwing_fire.wav", "xwing_fire");
	soundEngine->addSound("sound/tie_fire.wav", "tie_fire");
	soundEngine->addSound("sound/tie_explode.wav", "tie_explode");
	soundEngine->addSound("sound/tie_hit.wav", "tie_hit");
}


void Level3::drawLevel()
{
	checkEvent();

	pe->stayOnScreen(hero, make_pair(SCREEN_WIDTH, GAMEZONE_HEIGHT));

	for (std::list<Drawable *>::iterator anElement = activeElements.begin() ; anElement != activeElements.end(); ++anElement)
	{
		(*anElement)->animate();
		(*anElement)->processDisplay();
	}
	hud->displayUI();
	hud->displayHealth(hero->health);
	hud->displayLife(hero->nbLife);
	hud->displayScore(Score);

	theFalcon->displayHealth();
	theFalcon->healthDisplay->processDisplay();

	hero->animate();
	background.setAnimX(background.getAnimX() + cameraSpeed);

	moveBackGround();

	if(ending)
	{
		finishLevel();
	}
}

//function that handle the events (enemies apparitions, collision checks,  etc...)
void Level3::checkEvent()
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
}


int Level3::checkEnemyCollision(Drawable * anElement)
{

	if(pe->collisionDetection(hero, anElement))
	{
		if(!(hero->invincible || hero->state == DEAD))
		{
			hero->processCollisionWith(anElement);
			anElement->processCollisionWith(hero);
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

int Level3::checkCollision(Drawable * anElement)
{
	if (hero->state == DEAD)
	{
		return 0;
	}

	if(pe->collisionDetection(hero, anElement))
	{
		anElement->processCollisionWith(hero);
		hero->processCollisionWith(anElement);
		return 1;
	}
	return 0;
}

void Level3::createExplosion(int x, int y, int type)
{
	activeElements.push_back(new Explosion(x, y, type));
}

void Level3::createBonus(int x, int y, int type)
{
	activeElements.push_back(new Bonus(x, y, type));
}

void Level3::finishLevel()
{
	//First call: init
	if(!ending)
	{
		exiting = TRUE;
		hero->setState(EXITING);
		fading = FALSE;
	}

	//Play Victory sound


	//Get hero out of the screen to the right
	if(exiting)
	{
		//Start fading out
		if (hero->posX >= SCREEN_WIDTH - 100)
		{
			ge->startFadingOut(3);
			fading = TRUE;
			exiting = FALSE;
		}
	}

	//Fade out
	if(fading)
	{
		if (ge->isFading == FALSE)
		{
			fading = FALSE;
		}
	}

	ending = TRUE;

	//Make sure all the events have taken place
	if(!fading && !exiting)
	{
		//Aaaand we are out!
		endLevel();
	}
}

//Load all the configuration elements from a text file
//Format: elementName;width;height;listOfAnimationStatus$nbOfFramesInAnAnimation;pathToTextureFile
void Level3::loadConf()
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


