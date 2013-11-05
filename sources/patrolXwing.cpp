#include "enemy.h"

Patrol::Patrol()
{
//	Enemy();
}

Patrol::Patrol(int x, int y, int typeXW, int dir)
{
	//Enemy(x,y,type,dir);
	Drawable();
	this->getTexture("xwing");
	width = atoi(((lev->configurationElements.at("xwing")).at(0)).c_str());
	height = atoi(((lev->configurationElements.at("xwing")).at(1)).c_str());
	posX = x;
	posY = y;
	state = typeXW;
	setAnimX(0);
	setAnimY(typeXW * height);
	direction = dir;
	type = typeXW;
	bonusProbability = 10;
	scoreValue = 200;
	canFire = FALSE;
	fireRate = 1250;
	lastTimeFired = 0;
	life = 50 * (typeXW+1);
	nbFrames = parseAnimationState((lev->configurationElements.at("xwing")).at(2));
}

void Patrol::animate()
{
	updateAnimationFrame();
	//self mX > 480 & (mY > 1) ifTrue:[self moveLeft: 2] ifFalse:[self moveLeft: 3. self moveUp: 2]
	//self mX > 480 & (mY < (480- self height )) ifTrue:[self moveLeft: 2] ifFalse:[self moveLeft: 3. self moveDown: 2]
	if(direction == UP)
	{
		if (posY > 1 && posX < SCREEN_WIDTH - width)
		{
			posY = posY - 2;
			posX = posX - 1;
		}
		posX = posX - 2;
	}
	else
	{
		if (posY < GAMEZONE_HEIGHT - height && posX < SCREEN_WIDTH - width)
		{
			posY = posY + 2;
			posX = posX - 1;
		}
		posX = posX - 2;
	}
}



