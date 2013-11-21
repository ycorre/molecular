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
	toBlend = TRUE;
	setAnimX(0);
	setAnimY(0);
}

Bullet::Bullet(int x, int y, float anAngle, int aSpeed)
{
	power = 50;
	direction = 0;
	type = 0;

	this->addTexture("shoot");
	width = atoi(((lev->configurationElements.at("shoot")).at(0)).c_str());
	height =  atoi(((lev->configurationElements.at("shoot")).at(1)).c_str());
	parseAnimationState((lev->configurationElements.at("shoot")).at(2));
	collision = ge->loadTexture("res/Shoot_Col.png");

	//Conversion from degree to radian
	angle = anAngle; //180 * (PI / 180.0);
	speed = aSpeed;
	posX = x;
	posY = y;
	state = 0;
	toBlend = TRUE;
	setAnimX(0);
	setAnimY(0);
}

void Bullet::animate()
{
	updateAnimationFrame();
	//Compute the next movement vector
	//Handle by the physic engine ?
	float vx, vy;
	vx = (speed)*cos(angle);
	vy = (speed)*sin(angle);

	posX = posX + vx;
	posY = posY + vy;
}

void Bullet::processCollisionWith(Drawable* aDrawable)
{
	if (aDrawable->isHero())
	{
		Hero * myHero = dynamic_cast<Hero*>(aDrawable);
		if (!myHero->invincible)
		{
			this->toRemove = TRUE;
		}
	}
}

