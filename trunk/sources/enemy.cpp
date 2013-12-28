#include "enemy.h"

Enemy::Enemy()
{
	name = "Enemy";
	posX = 0;
	posY = 0;
	setAnimX(0);
	setAnimY(0);
	scoreValue = 0;
	type = 0;
	bonusProbability = 90;
	speed = 2;
	originY = posY;
	sinusWidth = 400;
	sinusHeigth = 80;
	life = 50;
	canFire = FALSE;
	lastTimeFired = 0;
	minFireRate = 2000;
	maxFireRate = 1000;
	fireRate = minFireRate + (rand() % maxFireRate);
}

Enemy::Enemy(int x, int y, int typeXW)
{
	name = "Enemy";
	width = atoi(((lev->configurationElements.at("enemy")).at(0)).c_str());
	height = atoi(((lev->configurationElements.at("enemy")).at(1)).c_str());
	posX = x;
	posY = y;
	setAnimX(0);
	setAnimY(typeXW * height);
	type = typeXW;
	scoreValue = 200;
	bonusProbability = 50;
	canFire = FALSE;
	minFireRate = 2000;
	maxFireRate = 1000;
	fireRate = minFireRate + (rand() % maxFireRate);
	lastTimeFired = 0;
	life = 50 * (typeXW+1);
	speed = 2;

	originY = y;
	sinusWidth = 400;
	sinusHeigth = 80;
}

Enemy::Enemy(int x, int y, float sinWidth, float sinHeigth, float aSpeed)
{
	copyFrom(lev->loadedObjects.at("enemy"));

	posX = x;
	posY = y;
	setAnimX(0);
	setAnimY(0);
	type = 0;
	scoreValue = 200;
	bonusProbability = 50;
	canFire = FALSE;
	minFireRate = 2000;
	maxFireRate = 1000;
	fireRate = minFireRate + (rand() % maxFireRate);
	lastTimeFired = 0;
	life = 50;
	speed = aSpeed;

	originY = y;
	sinusWidth = sinWidth;
	sinusHeigth = sinHeigth;
}

void Enemy::animate()
{
	updateAnimationFrame();

	posX = posX - speed;

	//Normalize posX on [0, 2PI]
	//The width of the sinusoid is given by sinusWidth,
	float vx = ((int)posX % (int)sinusWidth)*((2*PI)/sinusWidth);

	//Compute the movement on the Y axis
	float vy = sin(vx);
	posY = originY + vy * sinusHeigth;
}

void Enemy::processCollisionWith(Drawable * aDrawable)
{
	if(aDrawable->isHero())
	{
		lev->soundEngine->playSound("xwing_explode");
		lev->createExplosion(posX + width/2, posY + height/2);
		dropBonus(posX, posY);
		Score = Score + scoreValue * (type + 1);
		toRemove = TRUE;
		return;
	}
	if (aDrawable->isLaser())
	{
		Shoot * aLaser =  dynamic_cast<Shoot*>(aDrawable);
		int lifeValue = life;
		life = max(0, life - aLaser->power);
		if(aLaser->isPhoton())
		{
			Photon * aPhoton =  dynamic_cast<Photon*>(aDrawable);
			aPhoton->removeEnergy(lifeValue - life);
		}
		if (life<=0)
		{
			lev->soundEngine->playSound("xwing_explode");
			lev->createExplosion(posX + width/2, posY + height/2);
			Score = Score + scoreValue * (type + 1);
			toRemove = TRUE;
			dropBonus(posX, posY);
		}
		return;
	}
}

void Enemy::dropBonus(int x, int y)
{
	int aNumber = rand() % 100;
	if (aNumber <=  bonusProbability)
	{
		int aBonus = rand () % 16;
		lev->createBonus(x, y, aBonus);
	}
}

void Enemy::fire()
{
	checkFire();
	if (canFire)
	{
		lev->soundEngine->playSound("enemyGun");

		//Shoot toward the hero
		//Compute the angle
		float xDiff = (lev->hero->posX + lev->hero->width/2) - (posX + width/2);
		float yDiff = (lev->hero->posY + lev->hero->height/2) - (posY + height/2);
		float angle = atan2(yDiff, xDiff);

		lev->activeElements.push_back(new Bullet(posX + 30, posY + 30, angle, 3));
		canFire = FALSE;
	}
}

void Enemy::checkFire()
{
	if (lev->isOnScreen(this))
	{
		unsigned int nextFireTime = lastTimeFired + fireRate;
		if (nextFireTime < GameTimer)
		{
			canFire = TRUE;
			lastTimeFired = GameTimer;
			fireRate = minFireRate + (rand() % maxFireRate);
		}
	}
}

/*
 * Cadmium functions
 */
Cadmium::Cadmium(int x, int y, float sinWidth, float sinHeigth, float aSpeed)
{
	copyFrom(lev->loadedObjects.at("eSp_Cadmium"));

	posX = x;
	posY = y;
	setAnimX(0);
	setAnimY(0);
	scoreValue = 200;
	bonusProbability = 80;
	canFire = FALSE;
	fireRate = 450;
	lastTimeFired = 0;
	life = 50;
	speed = aSpeed;
	angle = 0;
	originY = y;
	sinusWidth = sinWidth;
	sinusHeigth = sinHeigth;

	posRafale = 0;
	rafaleRate = 4000;
}

void Cadmium::checkFire()
{
	if (lev->isOnScreen(this))
	{
		unsigned int nextFireTime;
		if (posRafale == 0)
		{
			nextFireTime = lastTimeFired + rafaleRate;
		}
		else
		{
			nextFireTime = lastTimeFired + fireRate;
		}

		if (posRafale == 0)
		{
			//Shoot toward the hero
			//Compute the angle
			float xDiff = lev->hero->posX - posX;
			float yDiff = lev->hero->posY - posY;
			angle = atan2(yDiff, xDiff);
		}

		if (nextFireTime < GameTimer)
		{
			canFire = TRUE;
			posRafale = (posRafale + 1) % 3;
			lastTimeFired = GameTimer;
		}
	}
}

void Cadmium::fire()
{
	checkFire();
	if (canFire)
	{
		lev->soundEngine->playSound("enemyGun");

		lev->activeElements.push_back(new CadmiumAmmo(posX, posY, angle, 3));
		canFire = FALSE;
	}
}

