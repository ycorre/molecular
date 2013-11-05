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
	this->loadTexture("res/laser_c.png");
	width = texture->w;
	height = texture->h;
	posX = 0;
	posY = 0;
	state = 0;
	setAnimX(0);
	setAnimY(0);
	collision = ge->loadTexture("res/laser_cmasktrans.png");
}

Laser::Laser(int x, int y, int dir, int aType)
{
	power = 50;
	direction = dir;
	type = aType;
	if (type == GREEN_LASER)
	{
		this->getTexture("laserG");
		width = atoi(((lev->configurationElements.at("laserG")).at(0)).c_str());
		height =  atoi(((lev->configurationElements.at("laserG")).at(1)).c_str());
		nbFrames = parseAnimationState((lev->configurationElements.at("laserG")).at(2));
		collision = ge->loadTexture("res/laser_cmasktrans.png");
	}
	if (type == RED_LASER)
	{
		this->getTexture("laserR");
		width = atoi(((lev->configurationElements.at("laserR")).at(0)).c_str());
		height =  atoi(((lev->configurationElements.at("laserR")).at(1)).c_str());
		nbFrames = parseAnimationState((lev->configurationElements.at("laserR")).at(2));
		collision = ge->loadTexture("res/laser_dmasktrans.png");
	}
	posX = x;
	posY = y;
	state = 0;
	setAnimX(0);
	setAnimY(0);
}

void Laser::animate()
{
	if (direction == RIGHT)
	{
		posX = posX + 13;
	}
	else
	{
		posX = posX - 7;
	}
}

void Laser::processCollisionWith(Drawable* aDrawable)
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

