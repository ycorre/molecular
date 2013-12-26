#include "enemy.h"

Mitraille::Mitraille()
{
	copyFrom(lev->loadedObjects.at("mitraille"));
	setAnimX(0);
	setAnimY(0);
	scoreValue = 500;
	direction = UP;
	type = 0;
	bonusProbability = 50;
	canFire = FALSE;
	fireRate = 150;
	lastTimeFired = GameTimer;
	life = 250;
	posRafale = 0;
	rafaleRate = 500;
	setAngleAndSpeed();

	minFireRate = 2000;
	maxFireRate = 1000;
	//fireRate = minFireRate + (rand() % maxFireRate);
}

Mitraille::Mitraille(int x, int y)
{
	copyFrom(lev->loadedObjects.at("mitraille"));
	posX = x;
	posY = y;
	setAnimX(0);
	setAnimY(0);
	direction = RIGHT;
	type = 0;
	bonusProbability = 50;
	scoreValue = 200;
	canFire = FALSE;
	fireRate = 250;
	lastTimeFired = 0;
	rafaleRate = 1500;
	posRafale = 0;
	life = 500;
}


void Mitraille::checkFire()
{
	if (lev->isOnScreen(this))
	{
		unsigned int nextFireTime;
		if (posRafale == 9)
		{
			nextFireTime = lastTimeFired + rafaleRate;
		}
		else
		{
			nextFireTime = lastTimeFired + fireRate;
		}

		if (nextFireTime < GameTimer)
		{
			canFire = TRUE;
			posRafale = (posRafale + 1) % 10;
			lastTimeFired = GameTimer;
		}
	}
}

void Mitraille::fire()
{
	static float angle = 220;
	checkFire();
	if (canFire)
	{
		lev->soundEngine->playSound("enemyGun");

		if (posRafale == 9)
		{
			angle = 220;
		}

		//Shoot toward the hero
		//Compute the angle
		//float xDiff = lev->hero->posX - posX;
		//float yDiff = lev->hero->posY - posY;
		//float angle = atan2(yDiff, xDiff);=

		angle = angle - 5;
		float radAngle = angle * (PI / 180.0);
		lev->activeElements.push_back(new Bullet(posX + 30, posY + 30, radAngle, 3));
		canFire = FALSE;
	}
}
