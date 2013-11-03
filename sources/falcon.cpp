#include "enemy.h"

Falcon::Falcon()
{
	Drawable();
	this->getTexture("falcon");
	width = atoi(((lev->configurationElements.at("falcon")).at(0)).c_str());
	height = atoi(((lev->configurationElements.at("falcon")).at(1)).c_str());
	posX = 600;
	posY = 100;
	state = 0;
	animX = 0;
	animY = 0 * height;
	direction = 0;
	type = 0;
	bonusProbability = 10;
	canFire = FALSE;
	fireRate = 500;
	lastTimeFired = 0;
	fireCounter = 0;
	life = 5000;
	currentPhase = FALCON_PHASE_1;
	initialLife = life;
	nbFrames = parseAnimationState((lev->configurationElements.at("falcon")).at(2));
	scoreValue = 5000;

	healthDisplay = new Drawable();
	healthDisplay->loadTexture("res/redBox.png");
	healthDisplay->width = 1000;
	healthDisplay->height = 25;
	healthDisplay->posX = 100;
	healthDisplay->posY = 540;
	healthDisplay->animX = 0;
	initialHealthDisplayWidth = healthDisplay->width;
}

void Falcon::animate()
{
	if (direction == UP)
	{
		if (posY > -(height / 2))
		{
			posY = posY - 3;
		}
		else
		{
			direction = DOWN;
		}
	}
	else
	{
		if (posY < GAMEZONE_HEIGHT - (height / 2))
		{
			posY = posY + 3;
		}
		else
		{
			direction = UP;
		}
	}
}


void Falcon::processCollisionWith(Drawable* aDrawable)
{
	if(aDrawable->isHero())
	{
		lev->createExplosion(this->posX + this->width/2, this->posY - this->height/2, XWING_EXPL);
		return;
	}
	if (aDrawable->isLaser())
	{
		Laser * aLaser =  static_cast<Laser*>(aDrawable);
		life = life - aLaser->power;
		if (life<=0)
		{
			lev->createExplosion(this->posX + this->width/2 - 125, this->posY + (this->height/2) -50, XWING_EXPL);
			dropBonus(this->posX, this->posY);
			this->toRemove = TRUE;
			Score = Score + scoreValue * (type + 1);
			lev->finishLevel();
		}
		else
		{
			lev->createExplosion(aLaser->posX, aLaser->posY - aLaser->height, SPARK);
		}
		return;
	}
}

void Falcon::fire()
{
	checkFire();

	if (canFire)
	{
		//lev->soundEngine->PlaySound("sound/xwing_fire.wav");
		firePattern();
		fireCounter++;
		canFire = 0;
	}
}

void Falcon::checkFire()
{
	if (lev->isOnScreen(this))
	{
		unsigned int interval = lastTimeFired + fireRate;
		if (interval <  GameTimer)
		{
			canFire = 1;
			lastTimeFired = GameTimer;
		}
	}

	if (life <= ((2 * initialLife) / 3))
	{
		currentPhase = FALCON_PHASE_2;
		fireRate = 500;
	}
	if (life <= (initialLife / 3))
	{
		currentPhase = FALCON_PHASE_3;
		fireRate = 900;
	}
}

void Falcon::firePattern()
{
	switch(currentPhase)
	{
		case FALCON_PHASE_1:
			lev->activeElements.push_back(new Bullet(posX + 30, posY + height/2, LEFT, BLUE_BULLET, 180, 7));
			break;

		case FALCON_PHASE_2:
			lev->activeElements.push_back(new Bullet(posX + 30, posY + height/2, LEFT, BLUE_BULLET, 180, 10));
			if(fireCounter % 2 == 0)
			{
				lev->activeElements.push_back(new Bullet(posX + 30, posY + height/2, LEFT, BLUE_BULLET, 215, 3));
				lev->activeElements.push_back(new Bullet(posX + 30, posY + height/2, LEFT, BLUE_BULLET, 145, 3));
			}
			break;

		case FALCON_PHASE_3:
			lev->activeElements.push_back(new Bullet(posX + 30, posY + height/2, LEFT, BLUE_BULLET, 225, 3));
			lev->activeElements.push_back(new Bullet(posX + 30, posY + height/2, LEFT, BLUE_BULLET, 190, 3));
			lev->activeElements.push_back(new Bullet(posX + 30, posY + height/2, LEFT, BLUE_BULLET, 180, 3));
			lev->activeElements.push_back(new Bullet(posX + 30, posY + height/2, LEFT, BLUE_BULLET, 150, 3));
			lev->activeElements.push_back(new Bullet(posX + 30, posY + height/2, LEFT, BLUE_BULLET, 135, 3));

			//Create sparks randomly to show damage
			int x,y;
			x = this->posX + (rand() % this->width) - 75;
			y = this->posY + (rand() % this->height) - 75;
			lev->createExplosion(x, y, SPARK);
			break;

		default:
			break;
	}
}

//Display a health bar proportional to the remaining life of the falcon
void Falcon::displayHealth()
{
	healthDisplay->width = (life * initialHealthDisplayWidth) / initialLife;
}
