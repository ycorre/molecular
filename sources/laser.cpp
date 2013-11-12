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

Laser::Laser(int x, int y, int dir, int aType)
{
	power = 50;
	direction = dir;
	type = aType;
	if (type == GREEN_LASER)
	{
		this->addTexture("electron");
		width = atoi(((lev->configurationElements.at("electron")).at(0)).c_str());
		height =  atoi(((lev->configurationElements.at("electron")).at(1)).c_str());
		nbFrames = parseAnimationState((lev->configurationElements.at("electron")).at(2));
		collision = ge->loadTexture("res/ElectronMask.png");
	}
	if (type == RED_LASER)
	{
		this->addTexture("shoot");
		width = atoi(((lev->configurationElements.at("shoot")).at(0)).c_str());
		height =  atoi(((lev->configurationElements.at("shoot")).at(1)).c_str());
		nbFrames = parseAnimationState((lev->configurationElements.at("shoot")).at(2));
		collision = ge->loadTexture("res/Shoot_Col.png");
	}
	posX = x;
	posY = y;
	toBlend = TRUE;
	state = 0;
	setAnimX(0);
	setAnimY(0);
}

void Laser::animate()
{
	//updateAnimationFrame();
	if (direction == RIGHT)
	{
		posX = posX + 20;
	}
	else
	{
		posX = posX - 5;
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

