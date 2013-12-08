#include "enemy.h"

Mitraille::Mitraille()
{
	copyFrom(lev->loadedObjects.at("mitraille"));
	collision = ge->loadTexture("res/Bomb_Col.png");
	setAnimX(0);
	setAnimY(0);
	scoreValue = 500;
	direction = UP;
	type = 0;
	bonusProbability = 10;
	canFire = FALSE;
	fireRate = 250;
	lastTimeFired = GameTimer;
	life = 350;
	posRafale = 0;
	rafaleRate = 1500;
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
	life = 500;
	collision = ge->loadTexture("res/Bomb_Col.png");
}


void Mitraille::checkFire()
{
	if (lev->isOnScreen(this))
	{
		unsigned int nextFireTime;
		if (posRafale == 10)
		{
			nextFireTime = lastTimeFired + rafaleRate;
			posRafale == 0;
		}
		else
		{
			nextFireTime = lastTimeFired + fireRate;
		}

		if (nextFireTime < GameTimer)
		{
			canFire = TRUE;
			posRafale ++;
			lastTimeFired = GameTimer;
		}
	}
}

void Mitraille::fire()
{
	checkFire();
	if (canFire)
	{
		lev->soundEngine->playSound("enemyGun");

		//Shoot toward the hero
		//Compute the angle
		float xDiff = lev->hero->posX - posX;
		float yDiff = lev->hero->posY - posY;
		float angle = atan2(yDiff, xDiff);

		lev->activeElements.push_back(new Bullet(posX + 30, posY + 30, angle, 3));
		canFire = FALSE;
	}
}
