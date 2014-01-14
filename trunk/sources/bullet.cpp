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
	width = 0;
	height = 0;
	posX = 0;
	posY = 0;
	toBlend = true;
	setAnimX(0);
	setAnimY(0);
}

Bullet::Bullet(int x, int y, float anAngle, int aSpeed)
{
	power = 50;
	copyFrom(lev->loadedObjects.at("shoot"));

	//Conversion from degree to radian
	angle = anAngle; //180 * (PI / 180.0);
	speed = aSpeed;
	posX = x;
	posY = y;
	toBlend = true;
	setAnimX(0);
	setAnimY(0);
	//setAnimation("animDefaut");
}

void Bullet::animate()
{
	//updateAnimationFrame();
	//Compute the next movement vector
	//Handle by the physic engine ?
	float vx, vy;
	vx = speed * cos(angle);
	vy = speed * sin(angle);

	posX = posX + vx;
	posY = posY + vy;

	if(!lev->isOnScreen(this))
	{
		toRemove = true;
	}
}

void Bullet::processCollisionWith(Drawable* aDrawable)
{
	if (aDrawable->isHero())
	{
		Hero * myHero = dynamic_cast<Hero*>(aDrawable);
		if (!myHero->invincible)
		{
			toRemove = true;
		}
	}
}

/*
 * CadmiumAmmo functions
 */
CadmiumAmmo::CadmiumAmmo(int x, int y, float anAngle, float aSpeed)
{
	power = 50;
	copyFrom(lev->loadedObjects.at("e_Cadmium_Shoot"));

	halo.copyFrom(lev->loadedObjects.at("e_Cadmium_ShootHalo"));
	halo.posX = x;
	halo.posY = y;
	halo.setAnimX(0);
	halo.setAnimY(0);

	//Conversion from degree to radian
	angle = anAngle; //180 * (PI / 180.0);
	speed = aSpeed;
	posX = x;
	posY = y;

	setAnimX(0);
	setAnimY(0);
}

void CadmiumAmmo::animate()
{
	//Compute the next movement vector
	//Handle by the physic engine ?
	float vx, vy;
	vx = speed * cos(angle);
	vy = speed * sin(angle);

	posX = posX + vx;
	posY = posY + vy;

	halo.posX = halo.posX + vx;
	halo.posY = halo.posY + vy;

	if(!lev->isOnScreen(this))
	{
		toRemove = true;
		halo.toRemove = true;
	}

	halo.processDisplay();
}

void CadmiumAmmo::processCollisionWith(Drawable* aDrawable)
{
	if (aDrawable->isHero())
	{
		Hero * myHero = dynamic_cast<Hero*>(aDrawable);
		if (!myHero->invincible)
		{
			toRemove = true;
		}
	}
}

/*
 * CopperAmmo functions
 */
CopperAmmo::CopperAmmo(int x, int y, float anAngle, float aSpeed)
{
	power = 50;
	copyFrom(lev->loadedObjects.at("e005_Shoot"));
	posX = x - width/2;
	posY = y - height/2;

	halo.copyFrom(lev->loadedObjects.at("e005_Halo"));
	halo.posX = posX - 18;
	halo.posY = posY - 18;
	halo.setAnimX(0);
	halo.setAnimY(0);

	//Conversion from degree to radian
	angle = anAngle; //180 * (PI / 180.0);
	speed = aSpeed;

	setAnimX(0);
	setAnimY(0);
}

void CopperAmmo::animate()
{
	//Compute the next movement vector
	//Handle by the physic engine ?
	float vx, vy;
	vx = speed * cos(angle);
	vy = speed * sin(angle);

	posX = posX + vx;
	posY = posY + vy;

	halo.posX = (halo.posX + vx);
	halo.posY = (halo.posY + vy);

	if(!lev->isOnScreen(this))
	{
		toRemove = true;
		halo.toRemove = true;
	}

	halo.processDisplay();
}

void CopperAmmo::processCollisionWith(Drawable* aDrawable)
{
	if (aDrawable->isHero())
	{
		Hero * myHero = dynamic_cast<Hero*>(aDrawable);
		if (!myHero->invincible)
		{
			toRemove = true;
		}
	}
}

