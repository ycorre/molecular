/*
 * Contains bullet functions
 */

#include "laser.h"

//35x18
Bullet::Bullet()
{
	name = "bullet";
	power = 50;
	angle = 0;
	speed = 1;
	this->loadTexture("res/bullet.png");
	width = texture->w;
	height = texture->h;
	posX = 0;
	posY = 0;
	toBlend = TRUE;
	setAnimX(0);
	setAnimY(0);
}

Bullet::Bullet(int x, int y, float anAngle, int aSpeed)
{
	name = "bullet";
	power = 50;
	copyFrom(lev->loadedObjects.at("shoot"));
	collision = ge->loadTexture("res/Shoot_Col.png");

	//Conversion from degree to radian
	angle = anAngle; //180 * (PI / 180.0);
	speed = aSpeed;
	posX = x;
	posY = y;
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

	if(!lev->isOnScreen(this))
	{
		this->toRemove = TRUE;
	}
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

