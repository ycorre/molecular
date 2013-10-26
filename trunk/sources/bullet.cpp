/*
 * Contains bullet functions
 */

#include "laser.h"

Bullet::Bullet()
{
	type = 0;
	power = 50;
	direction = 0;
	angle = 0;
	speed = 1;
	texture = ge->loadTexture("res/bullet.png");
	width = texture->w;
	height = texture->h;
	posX = 0;
	posY = 0;
	state = 0;
	animX = 0;
	animY = 0;
}

Bullet::Bullet(int x, int y, int dir, int aType)
{
	power = 50;
	direction = dir;
	type = aType;
	if (type == BLUE_BULLET)
	{
		texture = ge->textures.at("bullet");
		width = atoi(((lev->configurationElements.at("bullet")).at(0)).c_str());
		height =  atoi(((lev->configurationElements.at("bullet")).at(1)).c_str());
		nbFrames = parseAnimationState((lev->configurationElements.at("bullet")).at(2));
	}
	posX = x;
	posY = y;
	state = 0;
	animX = 0;
	animY = 0;
}

Bullet::Bullet(int x, int y, int dir, int aType, int anAngle, int aSpeed)
{
	power = 50;
	direction = dir;
	type = aType;
	if (type == BLUE_BULLET)
	{
		texture = ge->textures.at("bullet");
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
	animX = 0;
	animY = 0;
}

void Bullet::animate()
{

	//Compute the next movement vector
	float vx, vy;
	vx = (speed)*cos(angle);
	vy = (speed)*sin(angle);
	posX = posX + vx;
	posY = posY + vy;

/*
	if (direction == RIGHT)
	{
		//posX = posX + 2;
		posX = posX * cos(30);
	}
	else
	{
		posX = posX - 2;
		//posX = posX - (2 * cos(75));
		//posY = posY - (5 * sin(75));
	}*/

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

