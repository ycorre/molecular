/*
 * Contains level 2 functions
 */

#include "level2.h"

Level2::Level2()
{

}

void Level2::loadLevel(Hero * aHero)
{
	activeElements.clear();
	loadConf();
	loadObject();
	hud = new HUD(ge);
	cameraSpeed = 1;
	bombGenerationRate = 1500;
	lastTimeBomb = 0;
	levelState = LEVEL_PLAYING;

	hero = aHero;
	hero->setTexture();

	ending = fading = exiting = FALSE;
}

void Level2::loadObject()
{
	loadTextures();
	loadBackGround();
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
	Level::loadBackGround();

	background.width = SCREEN_WIDTH;
	background.height = SCREEN_HEIGHT;
	background.posX = 0;
	background.posY = 0;
	background.state = 0;

	soundEngine->addSound("sound/xwing_explode.wav", "xwing_explode");
	soundEngine->addSound("sound/xwing_fire.wav", "xwing_fire");
	soundEngine->addSound("sound/tie_fire.wav", "tie_fire");
	soundEngine->addSound("sound/tie_explode.wav", "tie_explode");
	soundEngine->addSound("sound/tie_hit.wav", "tie_hit");
	soundEngine->addSound("sound/Mitraille_attack.wav", "mitAttack");
	soundEngine->addSound("sound/Mitraille_loop.wav", "mitLoop");
	soundEngine->addSound("sound/EnnemiGun01.wav", "enemyGun");
	soundEngine->addSound("sound/Mitraille_release.wav", "mitRelease");
	soundEngine->sounds.at("mitLoop")->setLoop(-1);
}


void Level2::drawLevel()
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
	hud->displayMassPotential(hero->massPotential);
	hud->displayRadioPotential(hero->radioactivePotential);

	hero->animate();

	moveBackGround();
	background.setAnimX(background.getAnimX() + cameraSpeed);

	if(ending)
	{
		finishLevel();
	}
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
	}

	//Winning conditions
	if(background.getAnimX() >= 6900 - SCREEN_WIDTH && hero->state != DEAD)
	{
		//Level won
		finishLevel();
	}
}


int Level2::checkEnemyCollision(Drawable * anElement)
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

	for (list<Laser*>::iterator aLaser = hero->getLasers()->begin(); aLaser != hero->getLasers()->end(); ++aLaser)
	{
		Laser * aL = *aLaser;
		if(pe->collisionDetection(aL, anElement))
		{
			anElement->processCollisionWith(aL);
			aL->processCollisionWith(anElement);
			aL->display = TRUE;
			hero->getLasers()->erase(aLaser++);
			return 1;
		}
	}
	return 0;
}

int Level2::checkCollision(Drawable * anElement)
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

void Level2::createExplosion(int x, int y, int type)
{
	activeElements.push_back(new Explosion(x, y, type));
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

