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
	toBlend = TRUE;
	lastTimeFired = 0;
	fireRate = 80;
	canFire = TRUE;
	type = 0;
	maxFireRate = 80;
	setAnimX(0);
	setAnimY(0);
}

Weapon::Weapon(string aName, int powerValue, int aFireRate, int aType)
{
	name = aName;
	power = powerValue;
	this->loadTexture("res/Electron.png");
	width = getTexture()->w;
	height = getTexture()->h;
	posX = 0;
	posY = 0;
	toBlend = TRUE;
	lastTimeFired = 0;
	fireRate = aFireRate;
	type = aType;
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
		switch(type)
		{
			case WEAPON_ELECTRON:
				shoots.push_back(new Laser(aHero->posX - 64, aHero->posY + 16, RIGHT, GREEN_LASER, this));
				break;

			case WEAPON_PHOTON:
				shoots.push_back(new Photon(aHero->posX - 64, aHero->posY + 16, RIGHT, GREEN_LASER, this));
				break;
		}

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
			delete (*aLaser);
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
	ParticleEffect * aParticleEffect = new ParticleEffect();
	aParticleEffect->createImpactFrom(x, y);
	lev->ge->particleEffects.push_back(aParticleEffect);
}

void Weapon::upgrade()
{

}
