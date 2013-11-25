#include "enemy.h"

Enemy::Enemy()
{
	Drawable();
	posX = 0;
	posY = 0;
	state = 0;
	setAnimX(0);
	setAnimY(0);
	scoreValue = 0;
	type = XRED;
	bonusProbability = 50;
	direction = LEFT;
	speed = 2;
	originY = posY;
	sinusWidth = 400;
	sinusHeigth = 80;
	life = 50;
}

Enemy::Enemy(int x, int y, int typeXW, int dir)
{
	Drawable();
	this->addTexture("enemy");
	width = atoi(((lev->configurationElements.at("enemy")).at(0)).c_str());
	height = atoi(((lev->configurationElements.at("enemy")).at(1)).c_str());
	posX = x;
	posY = y;
	state = typeXW;
	setAnimX(0);
	setAnimY(typeXW * height);
	direction = dir;
	type = typeXW;
	scoreValue = 200;
	bonusProbability = 50;
	canFire = FALSE;
	minFireRate = 2000;
	maxFireRate = 1000;
	fireRate = minFireRate + (rand() % maxFireRate);
	lastTimeFired = 0;
	life = 50 * (typeXW+1);
	collision = ge->loadTexture("res/Ennemi_mask.png");
	parseAnimationState((lev->configurationElements.at("enemy")).at(2));
	speed = 2;

	originY = y;
	sinusWidth = 400;
	sinusHeigth = 80;
}

Enemy::Enemy(int x, int y, float sinWidth, float sinHeigth, float aSpeed)
{
	Drawable();
	copyFrom(lev->loadedObjects.at("enemy"));

	posX = x;
	posY = y;
	state = 0;
	setAnimX(0);
	setAnimY(0);
	direction = RIGHT;
	type = 0;
	scoreValue = 200;
	bonusProbability = 50;
	canFire = FALSE;
	minFireRate = 2000;
	maxFireRate = 1000;
	fireRate = minFireRate + (rand() % maxFireRate);
	lastTimeFired = 0;
	life = 50;
	collision = ge->loadTexture("res/Ennemi_mask.png");
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
	posY = originY + vy*sinusHeigth;
}

void Enemy::processCollisionWith(Drawable* aDrawable)
{
	if(aDrawable->isHero())
	{
		lev->soundEngine->playSound("xwing_explode");
		lev->createExplosion(this->posX-this->width/2, this->posY - this->height/2, XWING_EXPL);
		dropBonus(this->posX, this->posY);
		Score = Score + scoreValue * (type + 1);
		this->toRemove = TRUE;
		return;
	}
	if (aDrawable->isLaser())
	{
		Laser * aLaser =  dynamic_cast<Laser*>(aDrawable);
		life = life -50;// aLaser->power;
		if (life<=0)
		{
			lev->soundEngine->playSound("xwing_explode");
			lev->createExplosion(this->posX-this->width/2, this->posY - this->height/2, XWING_EXPL);
			Score = Score + scoreValue * (type + 1);
			this->toRemove = TRUE;
			dropBonus(this->posX, this->posY);
		}
		return;
	}
}

void Enemy::dropBonus(int x, int y)
{
	int aNumber = rand() % 100;
	if (aNumber <=  bonusProbability)
	{
		if (aNumber >  bonusProbability / 2)
		{
			lev->createBonus(x, y, 0);
		}
		else
		{
			lev->createBonus(x, y, 1);
		}
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
		float xDiff = lev->hero->posX - posX;
		float yDiff = lev->hero->posY - posY;
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
