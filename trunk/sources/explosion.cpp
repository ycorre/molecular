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
	int aRand = rand() % 2;
	if(aRand)
	{
		copyFrom(lev->loadedObjects.at("expl001Sp_Fire"));
	}
	else
	{
		copyFrom(lev->loadedObjects.at("expl002Sp_Fire"));
	}
	posX = x - width/2;
	posY = y - height / 2;
	setAnimX(0);
	setAnimY(0);

	if(aRand)
	{
		smoke.copyFrom(lev->loadedObjects.at("expl001Sp_Smoke"));
	}
	else
	{
		smoke.copyFrom(lev->loadedObjects.at("expl002Sp_Smoke"));
	}

	smoke.posX = x - width/2;
	smoke.posY = y - height/2;
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
		display = false;
	}

	smoke.updateAnimationFrame();
	if (smoke.currentAnimation->hasEnded)
	{
		toRemove = true;
		smoke.toRemove = true;
	}
	smoke.processDisplay();
}
