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
	this->loadTexture("res/Electron.png");
	width = getTexture()->w;
	height = getTexture()->h;
	posX = 0;
	posY = 0;
	state = 0;
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
	trail->state = 0;
	trail->setAnimX(0);
	trail->setAnimY(0);
	posX = x;
	posY = y;
	scaleX = 0.5;
	scaleY = 0.5;
	toBlend = TRUE;
	state = 0;
	setAnimX(0);
	setAnimY(0);
	setAnimation("static");
}

void Laser::animate()
{
	if (display)
	{
		if (direction == RIGHT)
		{
			posX = posX + 20;
		}
		else
		{
			posX = posX - 5;
		}
		trail->width = min(1200, (int)(this->posX - trail->posX + 124));
		//trail->currentAnimation->width = min(1200, (int)(this->posX - trail->posX + 124));
		trail->setAnimX(trail->getAnimX());
	}

	trail->updateAnimationFrame();
	ge->toDisplay.push_back(trail);

	if (trail->currentAnimation->hasEnded)
	{
		display = FALSE;
		toRemove = TRUE;
		trail->display = FALSE;
		trail->toRemove = TRUE;
		impacts.clear();
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
