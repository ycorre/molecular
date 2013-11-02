#include "enemy.h"

Enemy::Enemy()
{
	Drawable();
	texture = ge->textures.at("xwing");
	width = texture->w;
	height = texture->h;
	posX = 0;
	posY = 0;
	state = 0;
	animX = 0;
	animY = 0;
	scoreValue = 0;
	type = XRED;
	bonusProbability = 10;
	direction = LEFT;
	life = 50;
}


Enemy::Enemy(int x, int y, int typeXW, int dir)
{
	Drawable();
	texture = ge->textures.at("xwing");
	width = atoi(((lev->configurationElements.at("xwing")).at(0)).c_str());
	height = atoi(((lev->configurationElements.at("xwing")).at(1)).c_str());
	posX = x;
	posY = y;
	state = typeXW;
	animX = 0;
	animY = typeXW * height;
	direction = dir;
	type = typeXW;
	scoreValue = 200;
	bonusProbability = 10;
	canFire = FALSE;
	fireRate = 2500;
	lastTimeFired = 0;
	life = 50 * (typeXW+1);
	nbFrames = parseAnimationState((lev->configurationElements.at("xwing")).at(2));
}

void Enemy::animate()
{
	updateAnimationFrame();
	if(direction == RIGHT)
	{
		posX = posX + 2;
	}
	else
	{
		posX = posX - 2;
	}
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
		Laser * aLaser =  static_cast<Laser*>(aDrawable);
		life = life - aLaser->power;
		if (life<=0)
		{
			lev->soundEngine->playSound("xwing_explode");
			lev->createExplosion(this->posX-this->width/2, this->posY - this->height/2, XWING_EXPL);
			Score = Score + scoreValue * (type + 1);
			this->toRemove = TRUE;
			dropBonus(this->posX, this->posY);
		}
		else
		{
			lev->createExplosion(aLaser->posX - aLaser->width/2, aLaser->posY-this->height, SPARK);
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
		lev->soundEngine->playSound("xwing_fire");

		lev->activeElements.push_back(new Laser(posX + 30, posY + 30, LEFT, RED_LASER));
		canFire = 0;
	}
}

void Enemy::checkFire()
{
	if (lev->isOnScreen(this))
	{
		unsigned int nextFireTime = lastTimeFired + fireRate;
		if (nextFireTime < GameTimer)
		{
			canFire = 1;
			lastTimeFired = GameTimer;
		}
	}
}
