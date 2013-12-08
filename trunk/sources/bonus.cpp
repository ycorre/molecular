/*
 * Contains bonus functions
 */

#include "bonus.h"


Bonus::Bonus()
{
	type = 0;
	name = "bonus";
	width = texture->w;
	height = texture->h;
	posX = 0;
	posY = 0;
	setAnimX(0);
	setAnimY(0);
	scoreValue = 200;
}

Bonus::Bonus(int x, int y, int bType)
{
	type = bType;
	name = "bonus";

	if (bType == BONUS_LIFE)
	{
		copyFrom(lev->loadedObjects.at("bonusLife"));
	}
	if (bType == BONUS_FIRERATE)
	{
		copyFrom(lev->loadedObjects.at("bonusDiamond"));
	}
	posX = x;
	posY = y;
	setAnimX(0);
	setAnimY(0);
	scoreValue = 200;
}

void Bonus::animate()
{
	updateAnimationFrame();
	posX = posX - 2;
}

void Bonus::processCollisionWith(Drawable* aDrawable)
{
	if (aDrawable->isHero())
	{
		Score = Score + scoreValue;
		this->toRemove = TRUE;
	}
}
