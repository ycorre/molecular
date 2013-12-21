#include"explosion.h"

Explosion::Explosion()
{
	name = "explosion";
	this->loadTexture("res/expl2.png");
	width = texture->w;
	height = texture->h;
	posX = 0;
	posY = 0;
	setAnimX(0);
	setAnimY(0);
	type = XWING_EXPL;
}


Explosion::Explosion(int x, int y, int typeExpl)
{
	name = "explosion";
	copyFrom(lev->loadedObjects.at("explosion"));
	posX = x;
	posY = y;
	type = typeExpl;
	setAnimation("enemyExplode");
	setAnimX(0);
	setAnimY(0);
}

Explosion::~Explosion()
{

}

void Explosion::animate()
{
	updateAnimationFrame();
	if (getAnimX() == 0)
	{
		display = FALSE;
		toRemove = TRUE;
		if(type == TIE_EXPL)
		{
			lev->heroLoseLife();
		}
	}
	posX = posX - lev->cameraSpeed;
}
