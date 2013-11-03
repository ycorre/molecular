/*
 * Contains level 1 functions
 */

#include "level1.h"

void Level1::loadLevel(Hero * aHero)
{
	activeElements.clear();
	loadConf();
	loadObject();
	createWave1();
	hud = new HUD(ge);
	hud->loadHUDElements("conf/hud.conf");
	cameraSpeed = 1;
	hero =  aHero;
	hero->getTexture("tie");
	hero->width = atoi(((configurationElements.at("tie")).at(0)).c_str());
	hero->height = atoi(((configurationElements.at("tie")).at(1)).c_str());
	hero->nbFrames = parseAnimationState((configurationElements.at("tie")).at(2));
	levelState = LEVEL_PLAYING;
	ending = fading = exiting = FALSE;

	soundEngine->playSound("start_ambient");
}

void Level1::loadObject()
{
	loadTextures();
	loadBackGround();
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
	Level::loadBackGround();
	bg.animX = 0;
	
	soundEngine->addSound("sound/xwing_explode.wav", "xwing_explode");
	soundEngine->addSound("sound/xwing_fire.wav", "xwing_fire");
	soundEngine->addSound("sound/tie_fire.wav", "tie_fire");
	soundEngine->addSound("sound/tie_explode.wav", "tie_explode");
	soundEngine->addSound("sound/tie_hit.wav", "tie_hit");
}


void Level1::drawLevel()
{
	checkEvent();

	//Make sure the hero stays on screen
	pe->stayOnScreen(hero, make_pair(SCREEN_WIDTH, GAMEZONE_HEIGHT));

	//Set the list of elements to display
	for (std::list<Drawable *>::iterator anElement = activeElements.begin() ; anElement != activeElements.end(); ++anElement)
	{
		(*anElement)->animate();
		(*anElement)->processDisplay();
	}

	//Display the HUD
	hud->displayUI();
	hud->displayHealth(hero->health);
	hud->displayLife(hero->nbLife);
	hud->displayScore(Score);

	//Animate the hero
	hero->animate();

	//Move the background
	moveBackGround();
	bg.animX = bg.animX + cameraSpeed;

	if(ending)
	{
		finishLevel();
	}

}

//function that handle the events (enemies apparitions, collision checks,  etc...)
void Level1::checkEvent()
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
			if((*anElement)->isBonus() || (*anElement)->isLaser())
			{
				checkCollision(*anElement);
			}
		}
	}

	//Winning conditions
	if(bg.animX >= 6000 - SCREEN_WIDTH && hero->state != DEAD)
	{
		//Level won
		finishLevel();
	}
}

//TODO Standardize the loading and the description of the patterns of enemies
void Level1::createWave1()
{
	int i;
	int xc[5] = {1040, 1140, 1140, 1240, 1240};
	int yc[5] = {180, 290, 90, 350, 10};
	int xc3[5] = {2540, 2740, 2740, 2840, 2840};
	int yc3[5] = {170, 290, 90, 350, 10};
	int xc2[5] = {4040, 4140, 4140, 4240, 4240};

	int xc5[5] = {5840, 5940, 5940, 6040, 6040};
	int xc4[5] = {5640, 5740, 5740, 5840, 5840};

	int xc6[5] = {7840, 8840, 8840, 8040, 8040};
	int xc7[5] = {9040, 8140, 8140, 9040, 9040};
	int xc8[5] = {8240, 9240, 9240, 8440, 8440};


	int xPatrolRed1[5] = {1840, 1960, 2080, 2200, 2320};
	int xPatrolRed2[5] = {3340, 3460, 3580, 3700, 3820};

	int yPatrolDown[1] = {0};
	int yPatrolUp[1] = {GAMEZONE_HEIGHT - 100};

	int xPatrolBlue[5] = {4700, 4820, 4940, 5060, 5180};
	int xPatrolBlue2[5] = {7000, 7120, 7240, 7360, 7480};


	for (i=0; i < 5; i++)
	{
		//Regular
		activeElements.push_back(new Enemy(xc[i], yc[i], XRED, 1));
		activeElements.push_back(new Enemy(xc2[i], yc[i], XYELLOW, 1));
		activeElements.push_back(new Enemy(xc3[i], yc3[i], XBLUE, 1));

		activeElements.push_back(new Enemy(xc4[i], yc[i], XRED, 1));
		activeElements.push_back(new Enemy(xc5[i], yc[i], XBLUE, 1));

		activeElements.push_back(new Enemy(xc6[i], yc[i], XRED, 1));
		activeElements.push_back(new Enemy(xc7[i], yc[i], XYELLOW, 1));
		activeElements.push_back(new Enemy(xc8[i], yc[i], XBLUE, 1));

		//Patrol
		activeElements.push_back(new Patrol(xPatrolRed1[i], yPatrolDown[0], XRED, DOWN));
		activeElements.push_back(new Patrol(xPatrolRed2[i], yPatrolUp[0], XRED, UP));
		activeElements.push_back(new Patrol(xPatrolRed2[i], yPatrolDown[0], XRED, DOWN));
		activeElements.push_back(new Patrol(xPatrolBlue[i], yPatrolDown[0], XBLUE, DOWN));
		activeElements.push_back(new Patrol(xPatrolBlue2[i], yPatrolDown[0], XBLUE, DOWN));
		activeElements.push_back(new Patrol(xPatrolBlue2[i], yPatrolUp[0], XBLUE, UP));

	}
}

int Level1::checkEnemyCollision(Drawable * anElement)
{
	if(pe->collisionDetection(hero, anElement))
	{
		if(!(hero->invincible || hero->state == DEAD))
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

void Level1::finishLevel()
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
		if (hero->posX >= SCREEN_WIDTH - 300)
		{
			fading = TRUE;
			exiting = FALSE;
		}
	}

	//Fade out
	if(fading)
	{
		ge->fadeOut();
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

