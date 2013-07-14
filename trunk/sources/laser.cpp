/*
 * Contains level 1 functions
 */

#include "laser.h"

Laser::Laser()
{
	type = 0;
	power = 50;
	direction = 0;
	texture = ge->loadTexture("res/laser_c.png");
	width = texture->w;
	height = texture->h;
	posX = 0;
	posY = 0;
	state = 0;
	animX = 0;
	animY = 0;
}

Laser::Laser(int x, int y, int dir, int aType)
{
	power = 50;
	direction = dir;
	type = aType;
	if (type == GREEN_LASER)
	{
		texture = ge->textures.at("laserG");
		width = atoi(((lev->configurationElements.at("laserG")).at(0)).c_str());
		height =  atoi(((lev->configurationElements.at("laserG")).at(1)).c_str());
		nbFrames = parseAnimationState((lev->configurationElements.at("laserG")).at(2));
	}
	if (type == RED_LASER)
	{
		texture = ge->textures.at("laserR");
		width = atoi(((lev->configurationElements.at("laserR")).at(0)).c_str());
		height =  atoi(((lev->configurationElements.at("laserR")).at(1)).c_str());
		nbFrames = parseAnimationState((lev->configurationElements.at("laserR")).at(2));
	}
	posX = x;
	posY = y;
	state = 0;
	animX = 0;
	animY = 0;
}

void Laser::animate()
{
	if (direction == RIGHT)
	{
		posX = posX + 10;
	}
	else
	{
		posX = posX - 10;
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

