#include"include/explosion.h"

Explosion::Explosion()
{
	name = "explosion";
	width = 1;
	height = 1;
	posX = 0;
	posY = 0;
}


Explosion::Explosion(int x, int y)
{
	name = "explosion";
	int aRand = rand() % 2;
	if(aRand)
	{
		copyFrom(CurrentLevel->loadedObjects.at("expl001Sp_Fire"));
	}
	else
	{
		copyFrom(CurrentLevel->loadedObjects.at("expl002Sp_Fire"));
	}
	posX = x;
	posY = y;

	if(aRand)
	{
		smoke.copyFrom(CurrentLevel->loadedObjects.at("expl001Sp_Smoke"));
	}
	else
	{
		smoke.copyFrom(CurrentLevel->loadedObjects.at("expl002Sp_Smoke"));
	}

	smoke.posX = x;
	smoke.posY = y;
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
