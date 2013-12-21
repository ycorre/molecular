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
	loadTextures();
	loadBackGround();

	hud = new HUD(ge);
	cameraSpeed = 1;
	bombGenerationRate = 1500;
	lastTimeBomb = 0;
	levelState = LEVEL_PLAYING;

	hero = aHero;
	hero->setTexture(loadedObjects.at("atom"));

	soundEngine->playMusic("hybridQuarks");

	ending = fading = exiting = FALSE;
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

	hud->displayUI(hero);

	for (list<Effect *>::iterator anEffect = activeEffects.begin(); anEffect != activeEffects.end(); ++anEffect)
	{
		if((*anEffect)->animateEffect())
		{
			activeEffects.erase(anEffect--);
		}
	}

	hero->animate();

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

	//Generate Bomb ?
	if(generateBomb())
	{
		activeElements.push_back(new Bomb());
		activeElements.push_back(new Mitraille());
	}

	//Winning conditions
	/*if(background.getAnimX() >= 6900 - SCREEN_WIDTH && hero->state != DEAD)
	{
		//Level won
		finishLevel();
	}*/
}


int Level2::checkEnemyCollision(Drawable * anElement)
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
		Laser * aL = *aLaser;
		if(aL->display && pe->collisionDetection(aL, anElement))
		{
			anElement->processCollisionWith(aL);
			aL->processCollisionWith(anElement);
			return TRUE;
		}
	}
	return FALSE;
}

int Level2::checkCollision(Drawable * anElement)
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

//We only generate better weapons since health are useless here
void Level2::createBonus(int x, int y, int type)
{
	activeElements.push_back(new Bonus(x, y, BONUS_FIRERATE));
}

int Level2::generateBomb()
{
	unsigned int nextBombTime = lastTimeBomb + bombGenerationRate;
	if (nextBombTime < GameTimer)
	{
		lastTimeBomb = GameTimer;
		return TRUE;
	}

	return FALSE;
}

void Level2::finishLevel()
{
	//First call: init
	if(!ending)
	{
		exiting = TRUE;
		hero->setState(EXITING);
		fading = FALSE;
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
			fading = TRUE;
			exiting=FALSE;
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

