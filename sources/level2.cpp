/*
 * Contains level 2 functions
 */

#include "level2.h"

Level2::Level2()
{
	bombGenerationRate = 1500;
	lastTimeBomb = 0;
}

void Level2::loadLevel(Hero * aHero)
{
	activeElements.clear();
	loadLevelConfiguration("conf/Level1.json");
	loadBackGround();

	hud = new HUD(ge);
	cameraSpeed = 1;
	bombGenerationRate = 1500;
	lastTimeBomb = 0;
	levelState = LEVEL_PLAYING;

	hero = aHero;
	hero->setTexture(loadedObjects.at("atom"));

	soundEngine->playMusic("hybridQuarks");

	ending = fading = exiting = false;
}

void Level2::drawLevel()
{
	moveBackGround();
	checkEvent();

	pe->stayOnScreen(hero, make_pair(SCREEN_WIDTH, GAMEZONE_HEIGHT));

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

	hero->animate();
	hud->displayUI(hero);

	if(ending)
	{
		finishLevel();
	}
}

//function that handle the events (enemies apparitions, collision checks,  etc...)
void Level2::checkEvent()
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
			if((*anElement)->isBonus() ||(*anElement)->isLaser())
			{
				checkCollision(*anElement);
			}
		}
	}

	//Generate Bomb
	if(generateBomb())
	{
		//activeElements.push_back(new Bomb());
		activeElements.push_back(new Pyroxene(BIG_PYROXENE));
	}

	//Winning conditions
	/*if(background.getAnimX() >= 6900 - SCREEN_WIDTH && hero->state != DEAD)
	{
		//Level won
		finishLevel();
	}*/
}


bool Level2::checkEnemyCollision(Drawable * anElement)
{
	if(pe->collisionDetection(hero, anElement))
	{
		if(!(hero->invincible || hero->state == DEAD))
		{
			anElement->processCollisionWith(hero);
			hero->processCollisionWith(anElement);
			return true;
		}
	}

	hero->getLasers();
	for (list<Shoot*>::iterator aLaser = hero->shoots.begin(); aLaser != hero->shoots.end(); ++aLaser)
	{
		Shoot * aL = *aLaser;
		if(aL->display && pe->collisionDetection(aL, anElement))
		{
			anElement->processCollisionWith(aL);
			aL->processCollisionWith(anElement);
			return true;
		}
	}
	return false;
}

bool Level2::checkCollision(Drawable * anElement)
{
	if (hero->state == DEAD)
	{
		return false;
	}

	if(anElement->isBonus())
	{
		magnetBonus(anElement);
	}

	if(pe->collisionDetection(hero, anElement))
	{
		anElement->processCollisionWith(hero);
		hero->processCollisionWith(anElement);
		return true;
	}

	return false;
}

bool Level2::generateBomb()
{
	unsigned int nextBombTime = lastTimeBomb + bombGenerationRate;
	if (nextBombTime < GameTimer)
	{
		lastTimeBomb = GameTimer;
		return true;
	}

	return false;
}

void Level2::finishLevel()
{
	//First call: init
	if(!ending)
	{
		exiting = true;
		hero->setState(EXITING);
		fading = false;
		bombGenerationRate = 1000000;
	}

	//Play Victory sound


	//Get hero out of the screen to the right
	if(exiting)
	{
		//Start fading out
		if (hero->posX >= SCREEN_WIDTH - 300)
		{
			ge->startFadingOut(3);
			fading = true;
			exiting = false;
		}
	}

	//Fade out
	if(fading)
	{
		if (!ge->isFading)
		{
			fading = false;
		}
	}

	ending = true;

	//Make sure all the events have taken place
	if(!fading && !exiting)
	{
		//Aaaand we are out!
		endLevel();
	}
}

