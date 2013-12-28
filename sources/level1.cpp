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

	for (list<Effect *>::iterator anEffect = activeEffects.begin(); anEffect != activeEffects.end(); ++anEffect)
	{
		if((*anEffect)->animateEffect())
		{
			activeEffects.erase(anEffect--);
		}
	}

	//Animate the hero
	hero->animate();

	//Display the HUD
	hud->displayUI(hero);

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
	for (list<Shoot*>::iterator aLaser = hero->shoots.begin(); aLaser != hero->shoots.end(); ++aLaser)
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

	if(hero->shielded)
	{
		if(pe->collisionDetection(hero->shield, anElement))
		{
			anElement->processCollisionWith(hero);
			hero->processCollisionWith(anElement);
			return 1;
		}
	}
	else {
		if(pe->collisionDetection(hero, anElement))
		{
			anElement->processCollisionWith(hero);
			hero->processCollisionWith(anElement);
			return TRUE;
		}
	}

	return FALSE;
}

void Level1::instantiateEnemies()
{
	Json::Value enemyConf = configurations.at("enemies");

	float sinWidth = enemyConf[0].get("sinusWidth", 400).asFloat();
	float sinHeight = enemyConf[0].get("sinusHeight", 80).asFloat();
	float speed = enemyConf[0].get("speed", 2.2).asFloat();

	unsigned int index;
	const Json::Value positions = enemyConf[0]["positions"];
	for (index = 0; index < positions.size(); ++index)
	{
		int pX = positions[index].get("posX", 0).asInt();
		int pY = positions[index].get("posY", 0).asInt();
		activeElements.push_back(new Cadmium(pX, pY, sinWidth, sinHeight, speed));
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

