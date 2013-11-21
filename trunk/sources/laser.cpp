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
		this->addTexture("electron");
		width = atoi(((lev->configurationElements.at("electron")).at(0)).c_str());
		height =  atoi(((lev->configurationElements.at("electron")).at(1)).c_str());
		parseAnimationState((lev->configurationElements.at("electron")).at(2));
		collision = ge->loadTexture("res/ElectronMask.png");
		trail = new FrameDrawable();
		trail->addTexture("electronTrail");
		trail->setWidth(atoi(((lev->configurationElements.at("electronTrail")).at(0)).c_str()));
		trail->height =  atoi(((lev->configurationElements.at("electronTrail")).at(1)).c_str());
		trail->parseAnimationState((lev->configurationElements.at("electronTrail")).at(2));
	}
	if (type == RED_LASER)
	{
		this->addTexture("shoot");
		width = atoi(((lev->configurationElements.at("shoot")).at(0)).c_str());
		height =  atoi(((lev->configurationElements.at("shoot")).at(1)).c_str());
		parseAnimationState((lev->configurationElements.at("shoot")).at(2));
		collision = ge->loadTexture("res/Shoot_Col.png");
	}
	trail->posX = x + width - 30;
	trail->posY = y + 10;
	trail->toBlend = TRUE;
	trail->state = 0;
	trail->setAnimX(0);
	trail->setAnimY(0);
	posX = x;
	posY = y;
	toBlend = TRUE;
	state = 0;
	setAnimX(0);
	setAnimY(0);
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
		trail->setAnimX(trail->getAnimX());
	}

	trail->updateAnimationFrame();
	ge->toDisplay.push_back(trail);

	if (trail->currentFrame == 0)
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
