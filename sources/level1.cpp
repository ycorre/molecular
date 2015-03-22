/*
 * Contains level 1 functions
 */

#include "include/level1.h"

Level1::Level1()
{
	cameraSpeed = 1;
}

void Level1::loadLevel(Hero * aHero)
{
	loadLevelConfiguration("conf/Level1.json");
	loadBackGround();

	soundEngine->playMusic("hybridQuarks");

	hud = new HUD(graphicEngine);
	cameraSpeed = 1;

	hero = aHero;
	hero->setTexture(loadedObjects.at("atom"));

	levelState = LEVEL_PLAYING;
	ending = fading = exiting = false;
	activeBlockingWave = 0;
}

void Level1::drawLevel()
{
	//Move the background
	moveBackGround();
	checkEvent();

	//Make sure the hero stays on screen
	physicEngine->stayOnScreen(hero, make_pair(SCREEN_WIDTH, SCREEN_HEIGHT));

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
			}
			if((*anElement)->isBonus() || (*anElement)->isLaser())
			{
				checkCollision(*anElement);
			}
		}
	}

	if(!activeBlockingWave)
		levelPosition++;

	//Check if we are at a point where we should instantiate a new wave
	if(enemyWaves.find(levelPosition) != enemyWaves.end())
	{
		for(vector<EnemyWave *>::iterator aWave = enemyWaves.at(levelPosition).begin(); aWave != enemyWaves.at(levelPosition).end(); ++aWave)
		{
			(*aWave)->launchWave();
			activeWaves.push_back(*aWave);
			if((*aWave)->blocking)
				activeBlockingWave++;
		}

		enemyWaves.erase(levelPosition);
	}

	//Animate the elements of the active waves
	for(list<EnemyWave *>::iterator aWave = activeWaves.begin(); aWave != activeWaves.end(); aWave++)
	{
		(*aWave)->animate();
		if(!(*aWave)->active)
		{
			if((*aWave)->blocking)
				activeBlockingWave--;

			delete (*aWave);
			activeWaves.erase(aWave--);
		}
	}

	//Winning conditions
	if(levelPosition >= 15800 && hero->state != DEAD)
	{
		//Level won
		finishLevel();
	}
}

bool Level1::checkEnemyCollision(Drawable * anElement)
{
	if(physicEngine->collisionDetection(hero, anElement))
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
		if((*aLaser)->display && physicEngine->collisionDetection(*aLaser, anElement))
		{
			anElement->processCollisionWith(*aLaser);
			(*aLaser)->processCollisionWith(anElement);
			return true;
		}
	}
	return false;
}

bool Level1::checkCollision(Drawable * anElement)
{
	if (hero->state == DEAD)
	{
		return false;
	}

	if(anElement->isBonus())
	{
		magnetBonus(anElement);
	}

	if(hero->shielded)
	{
		if(physicEngine->collisionDetection(hero->shield, anElement))
		{
			anElement->processCollisionWith(hero);
			hero->processCollisionWith(anElement);
			return 1;
		}
	}
	else {
		if(physicEngine->collisionDetection(hero, anElement))
		{
			anElement->processCollisionWith(hero);
			hero->processCollisionWith(anElement);
			return true;
		}
	}

	return false;
}

void Level1::finishLevel()
{
	//First call: init
	if(!ending)
	{
		exiting = true;
		hero->setState(EXITING);
		fading = false;
	}

	//Play Victory sound: TADA!!!


	//Get hero out of the screen to the right
	if(exiting)
	{
		//Start fading out
		if (hero->posX >= SCREEN_WIDTH - 300)
		{
			graphicEngine->startFadingOut(3);
			fading = true;
			exiting = false;
		}
	}

	//Fade out
	if(fading)
	{
		if (graphicEngine->isFading == false)
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

