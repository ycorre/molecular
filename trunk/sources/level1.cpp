/*
 * Contains level 1 functions
 */

#include "level1.h"

Level1::Level1()
{
	cameraSpeed = 1;
}

void Level1::loadLevel(Hero * aHero)
{
	activeElements.clear();
	loadLevelConfiguration("conf/Level1.json");
	loadEnemies();
	loadBackGround();
	instantiateEnemies();

	soundEngine->playMusic("hybridQuarks");

	hud = new HUD(ge);
	cameraSpeed = 1;

	hero = aHero;
	hero->setTexture(loadedObjects.at("atom"));

	levelState = LEVEL_PLAYING;
	ending = fading = exiting = FALSE;
}

void Level1::drawLevel()
{
	//Move the background
	moveBackGround();
	checkEvent();

	//Make sure the hero stays on screen
	pe->stayOnScreen(hero, make_pair(SCREEN_WIDTH, GAMEZONE_HEIGHT));

	//Set the list of elements to display
	for (list<Drawable *>::iterator anElement = activeElements.begin() ; anElement != activeElements.end(); ++anElement)
	{
		(*anElement)->animate();
		(*anElement)->processDisplay();
	}

	//Display the HUD
	hud->displayUI(hero);

	for (list<Effect *>::iterator anEffect = activeEffects.begin(); anEffect != activeEffects.end(); ++anEffect)
	{
		if((*anEffect)->animateEffect())
		{
			activeEffects.erase(anEffect--);
		}
	}

	//Animate the hero
	hero->animate();

	//If we are in the finishing sequence
	if(ending)
	{
		finishLevel();
	}
}

//function that handle the events (enemies apparitions, collision checks,  etc...)
void Level1::checkEvent()
{
	for (list<Drawable *>::iterator anElement = activeElements.begin() ; anElement != activeElements.end(); ++anElement)
	{
		if((*anElement)->toRemove)
		{
			delete (*anElement);
			anElement =	activeElements.erase(anElement);
		}
		else
		{
			if((*anElement)->isEnemy())
			{
				checkEnemyCollision(*anElement);
				Enemy * anEnemy = dynamic_cast<Enemy *>(*anElement);
				anEnemy->fire();
			}
			if((*anElement)->isBonus() || (*anElement)->isLaser())
			{
				checkCollision(*anElement);
			}
		}
	}

	//Winning conditions
	/*if(background.getAnimX() >= 15800 - SCREEN_WIDTH && hero->state != DEAD)
	{
		//Level won
		finishLevel();
	}*/
}

int Level1::checkEnemyCollision(Drawable * anElement)
{
	if(pe->collisionDetection(hero, anElement))
	{
		if(!(hero->invincible || hero->state == DEAD))
		{
			anElement->processCollisionWith(hero);
			hero->processCollisionWith(anElement);
			return TRUE;
		}
	}

	hero->getLasers();
	for (list<Laser*>::iterator aLaser = hero->shoots.begin(); aLaser != hero->shoots.end(); ++aLaser)
	{
		if((*aLaser)->display && pe->collisionDetection(*aLaser, anElement))
		{
			anElement->processCollisionWith(*aLaser);
			(*aLaser)->processCollisionWith(anElement);
			return TRUE;
		}
	}
	return FALSE;
}

int Level1::checkCollision(Drawable * anElement)
{
	if (hero->state == DEAD)
	{
		return FALSE;
	}

	if(pe->collisionDetection(hero, anElement))
	{
		anElement->processCollisionWith(hero);
		hero->processCollisionWith(anElement);
		return TRUE;
	}

	return FALSE;
}

//Load enemies from a configuration files
//Format is on the first line:
//sinus wave width;sinus wave height;speed
//The next lines: enemies position
//x;y
void Level1::loadEnemies()
{
	enemyConfigurationElements.clear();
	ifstream file;
	string line;
	string fileName = "conf/enemyL1.conf";
	string token;
	string type;
	vector<string> confElements;
	string confElement;

	file.open(fileName.c_str());
	while(getline(file, line))
	{
		if(!line.empty()) //Ignore empty lines
		{
			confElements.clear();
			istringstream myLine(line);

			while(getline(myLine, token, ';'))
			{
				confElements.push_back(token);
			}
			enemyConfigurationElements.push_back(confElements);
		}
	}

}

void Level1::instantiateEnemies()
{
	vector<string> confCharacteristics = enemyConfigurationElements.front();

	float sinWidth = atof(confCharacteristics.at(0).c_str());
	float sinHeight = atof(confCharacteristics.at(1).c_str());
	float speed = atof(confCharacteristics.at(2).c_str());
	//Delete the first element
	enemyConfigurationElements.pop_front();

	for (list<vector<string> >::iterator anEnemy = enemyConfigurationElements.begin(); anEnemy != enemyConfigurationElements.end(); ++anEnemy)
	{
		int pX = atoi((anEnemy->at(0)).c_str());
		int pY = atoi((anEnemy->at(1)).c_str());
		activeElements.push_back(new Enemy(pX, pY, sinWidth, sinHeight, speed));
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

	//Play Victory sound: TADA!!!


	//Get hero out of the screen to the right
	if(exiting)
	{
		//Start fading out
		if (hero->posX >= SCREEN_WIDTH - 300)
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

