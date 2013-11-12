/*
 * Contains bullet functions
 */

#include "laser.h"

//35x18
Bullet::Bullet()
{
	type = 0;
	power = 50;
	direction = 0;
	angle = 0;
	speed = 1;
	this->loadTexture("res/bullet.png");
	width = texture->w;
	height = texture->h;
	posX = 0;
	posY = 0;
	state = 0;
	setAnimX(0);
	setAnimY(0);
	hitBoxWidth = 26;
	hitBoxHeight = 4;
	hitBoxX = posX + 6;
	hitBoxY = posY + 7;
}

Bullet::Bullet(int x, int y, int dir, int aType, int anAngle, int aSpeed)
{
	power = 50;
	direction = dir;
	type = aType;
	if (type == BLUE_BULLET)
	{
		this->addTexture("bullet");
		width = atoi(((lev->configurationElements.at("bullet")).at(0)).c_str());
		height =  atoi(((lev->configurationElements.at("bullet")).at(1)).c_str());
		nbFrames = parseAnimationState((lev->configurationElements.at("bullet")).at(2));
	}
	//Conversion from degree to radian
	angle = anAngle * (PI / 180.0);
	speed = aSpeed;
	posX = x;
	posY = y;
	state = 0;
	setAnimX(0);
	setAnimY(0);
	hitBoxWidth = 24;
	hitBoxHeight = 4;
	hitBoxX = posX + 6;
	hitBoxY = posY + 7;
}

void Bullet::animate()
{
	//Compute the next movement vector
	//Handle by the physic engine ?
	float vx, vy;
	vx = (speed)*cos(angle);
	vy = (speed)*sin(angle);

	posX = posX + vx;
	posY = posY + vy;

	hitBoxX = posX + 6;
	hitBoxY = posY + 7;
}

void Bullet::processCollisionWith(Drawable* aDrawable)
{
	if (aDrawable->isHero())
	{
		Hero * myHero = static_cast<Hero*>(aDrawable);
		if (!myHero->invincible)
		{
			this->toRemove = TRUE;
		}
	}
}

