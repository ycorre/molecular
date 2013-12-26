#include"explosion.h"

Explosion::Explosion()
{
	name = "explosion";
	width = 1;
	height = 1;
	posX = 0;
	posY = 0;
	setAnimX(0);
	setAnimY(0);
}


Explosion::Explosion(int x, int y)
{
	name = "explosion";
	copyFrom(lev->loadedObjects.at("expl001Sp_Fire"));
	posX = x - width/2;
	posY = y - height / 2;
	setAnimX(0);
	setAnimY(0);

	smoke.copyFrom(lev->loadedObjects.at("expl001Sp_Smoke"));
	smoke.posX = x - width/2;;
	smoke.posY = y - height/2;;
	smoke.setAnimX(0);
	smoke.setAnimY(0);
}

Explosion::~Explosion()
{

}

void Explosion::animate()
{
	updateAnimationFrame();
	if (currentAnimation->hasEnded)
	{
		display = FALSE;
	}

	smoke.updateAnimationFrame();
	if (smoke.currentAnimation->hasEnded)
	{
		toRemove = TRUE;
		smoke.toRemove = TRUE;
	}
	smoke.processDisplay();
}
