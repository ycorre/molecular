/*
 * Contains weapon functions
 */

#include "weapon.h"

//80x34
Weapon::Weapon()
{
	power = 50;
	this->loadTexture("res/Electron.png");
	width = getTexture()->w;
	height = getTexture()->h;
	posX = 0;
	posY = 0;
	state = 0;
	toBlend = TRUE;
	lastTimeFired = 0;
	fireRate = 80;
	canFire = TRUE;
	maxFireRate = 80;
	setAnimX(0);
	setAnimY(0);
}

void Weapon::fire(Hero * aHero)
{
	isFiring = TRUE;
	checkFire();
	if (canFire)
	{
		shoots.push_back(new Laser(aHero->posX - 64, aHero->posY + 16, RIGHT, GREEN_LASER, this));
		canFire = FALSE;
	}
}


void Weapon::checkFire()
{
	unsigned int nextFireTime = lastTimeFired + fireRate;
	if (nextFireTime < GameTimer)
	{
		canFire = TRUE;
		lastTimeFired = GameTimer;
	}
}

void Weapon::animateLasers()
{
	for (std::list<Laser *>::iterator aLaser = shoots.begin(); aLaser != shoots.end(); ++aLaser)
	{
		(*aLaser)->animate();
		if ((*aLaser)->toRemove)
		{
			shoots.erase(aLaser++);
		}
		else
		{
			(*aLaser)->processDisplay();
		}
	}

	for (std::list<AnimatedDrawable *>::iterator anImpact = impacts.begin(); anImpact != impacts.end(); ++anImpact)
	{
		if ((*anImpact)->updateAnimationFrame() && (*anImpact)->getAnimX()==0)
		{
			impacts.erase(anImpact++);
		}
		else
		{
			(*anImpact)->processDisplay();
		}
	}
}

void Weapon::createImpact(float x, float y)
{
	AnimatedDrawable * anImpact = new AnimatedDrawable();
	anImpact->copyFrom(lev->loadedObjects.at("electronImpact"));

	anImpact->toBlend = TRUE;
	anImpact->posX = x;
	anImpact->posY = y;
	anImpact->setAnimX(0);
	anImpact->setAnimY((rand() % 10)*anImpact->width);
	impacts.push_back(anImpact);
}

void Weapon::upgrade()
{

}
