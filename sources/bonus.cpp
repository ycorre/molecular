/*
 * Contains bonus functions
 */

#include "bonus.h"

using namespace std;

Bonus::Bonus()
{
	type = 0;
	texture = ge->loadTexture("res/laser_c.png");
	width = texture->w;
	height = texture->h;
	posX = 0;
	posY = 0;
	state = 0;
	animX = 0;
	animY = 0;
	scoreValue = 200;
}

Bonus::Bonus(int x, int y, int bType)
{
	type = bType;
	if (bType == BONUS_LIFE)
	{
		texture = ge->textures.at("bonusLife");
		width = atoi(((lev->configurationElements.at("bonusLife")).at(0)).c_str());
		height = atoi(((lev->configurationElements.at("bonusLife")).at(1)).c_str());
		nbFrames = parseAnimationState((lev->configurationElements.at("bonusLife")).at(2));
		nbFrames.clear();
		nbFrames.push_back(20);
	}
	if (bType == BONUS_FIRERATE)
	{
		texture = ge->textures.at("bonusFireRate");
		width = atoi(((lev->configurationElements.at("bonusFireRate")).at(0)).c_str());
		height = atoi(((lev->configurationElements.at("bonusFireRate")).at(1)).c_str());
		nbFrames = parseAnimationState((lev->configurationElements.at("bonusFireRate")).at(2));
		nbFrames.clear();
		nbFrames.push_back(20);
	}
	posX = x;
	posY = y;
	state = 0;
	animX = 0;
	animY = 0;
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
