/*
 * Contains laser functions
 */

#include "laser.h"

//80x34
Laser::Laser()
{
	type = 0;
	power = 50;
	direction = 0;
	posX = 0;
	posY = 0;
	toBlend = TRUE;
	setAnimX(0);
	setAnimY(0);
	collision = ge->loadTexture("res/ElectronMask.png");
}

Laser::Laser(int x, int y, int dir, int aType, Weapon * aWeapon)
{
	power = 50;
	direction = dir;
	type = aType;
	firingWeapon = aWeapon;
	if (type == GREEN_LASER)
	{
		copyFrom(lev->loadedObjects.at("electron"));
		collision = ge->loadTexture("res/ElectronMask.png");

		trail = new AnimatedDrawable();
		trail->copyFrom(lev->loadedObjects.at("electronTrail"));
	}
	trail->posX = x + width/2 - 30;
	trail->posY = y + 10;
	trail->toBlend = TRUE;
	trail->setAnimX(0);
	trail->setAnimY(0);
	posX = x;
	posY = y;
	scaleX = 0.5;
	scaleY = 0.5;
	rotZ = -1.0;
	rotationAngle = -8 + (rand() % 16);
	angle = rotationAngle * (PI / 180.0);
	toBlend = TRUE;
	setAnimX(0);
	setAnimY(0);
	setAnimation("static");
}

void Laser::animate()
{

	trail->currentAnimation->width = trail->width;
	trail->updateAnimationFrame();

	if (display)
	{

		//Compute the next movement vector
		//Handle by the physic engine ?
		float vx, vy;
		vx = (20)*cos(angle);
		vy = (20)*sin(angle);

		posX = posX + vx;
		posY = posY + vy;

	/*	if (direction == RIGHT)
		{
			posX = posX + 20;
		}
		else
		{
			posX = posX - 5;
		}*/
	}
	/*trail->currentAnimation->width = min(1200, (int)(this->posX - trail->posX + 124));
	trail->setAnimX(trail->getAnimX());
	trail->processDisplay();*/

	if (trail->currentAnimation->hasEnded)
	{
		display = FALSE;
		toRemove = TRUE;
		trail->display = FALSE;
		trail->toRemove = TRUE;
	}
}

void Laser::processCollisionWith(Drawable* aDrawable)
{
	firingWeapon->createImpact(posX, posY - 80);

	if (aDrawable->isHero())
	{
		Hero * myHero = dynamic_cast<Hero*>(aDrawable);
		if (!myHero->invincible)
		{
			this->toRemove = TRUE;
		}
	}
}


/*
 * Photon function
 */
Photon::Photon(int x, int y, int dir, int aType, Weapon * aWeapon)
{
	direction = dir;
	type = aType;
	firingWeapon = aWeapon;
	power = firingWeapon->power;
	if (type == GREEN_LASER)
	{
		copyFrom(lev->loadedObjects.at("photon"));
		collision = ge->loadTexture("res/ElectronMask.png");

		trail = new AnimatedDrawable();
		trail->copyFrom(lev->loadedObjects.at("photonTrail"));
	}
	trail->posX = x + 96;
	trail->posY = y - 28;
	trail->toBlend = TRUE;
	trail->setAnimX(0);
	trail->setAnimY(0);
	posX = x + 50;
	posY = y - 50;
	toBlend = TRUE;
	setAnimX(0);
	setAnimY(0);
	setAnimation("static");
}

void Photon::animate()
{
	float previousWidth = trail->currentAnimation->width;
	trail->currentAnimation->width = trail->width;
	trail->updateAnimationFrame();

	if (display)
	{
		posX = posX + 8.75;
		updateAnimationFrame();
		trail->currentAnimation->width = min(trail->width, (int)(this->posX - trail->posX + 124));
	}




	if(!display)
	{
		trail->currentAnimation->width = max(0.0, previousWidth - 8.5);

		trail->posX = trail->posX + 8.5;
	}

	trail->setAnimX(trail->getAnimX());




	trail->processDisplay();

	if (trail->currentAnimation->hasEnded)
	{
		display = FALSE;
		toRemove = TRUE;
		trail->display = FALSE;
		trail->toRemove = TRUE;
	}
}

void Photon::processCollisionWith(Drawable* aDrawable)
{

}
