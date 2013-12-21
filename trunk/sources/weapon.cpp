/*
 * Contains weapon functions
 */

#include "weapon.h"

//80x34
Weapon::Weapon()
{
	power = 50;
	lastTimeFired = 0;
	fireRate = 80;
	canFire = TRUE;
	maxFireRate = 80;
	load = -1;
	loadable = FALSE;
	isFiring = FALSE;
	level = 0;
}

void Weapon::fire(Hero * aHero)
{

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
	for (list<Laser*>::iterator aLaser = shoots.begin(); aLaser != shoots.end(); ++aLaser)
	{
		Laser * aL = (*aLaser);
		aL->animate();
		if (aL->toRemove)
		{
			delete (*aLaser);
			shoots.erase(aLaser++);
		}
		else
		{
			aL->processDisplay();
		}
	}
}

void Weapon::createImpact(float x, float y)
{
        ParticleEffect * aParticleEffect = new ParticleEffect();
        aParticleEffect->createImpactFrom(x, y);
       // lev->ge->particleEffects.push_back(aParticleEffect);
}

void Weapon::upgrade()
{

}

Electron::Electron()
{
	name = "electronGun";
	power = 50;
	lastTimeFired = 0;
	fireRate = 40;
	canFire = TRUE;
	maxFireRate = 40;
	load = -1;
	loadable = FALSE;
}

void Electron::fire(Hero * aHero)
{
	isFiring = TRUE;
	checkFire();
	if (canFire)
	{
		shoots.push_back(new Laser(aHero->posX - 64, aHero->posY + 16, this));
		canFire = FALSE;
	}
}

Hadron::Hadron()
{
	name = "hadronGun";
	power = 250;
	lastTimeFired = 0;
	fireRate = 750;
	canFire = TRUE;
	maxFireRate = 80;
	load = 0;
	loadable = TRUE;
}

void Hadron::fire(Hero * aHero)
{
	isFiring = TRUE;
	checkFire();
	if (canFire)
	{
		shoots.push_back(new Photon(aHero->posX - 64, aHero->posY + 16, this));
		canFire = FALSE;
	}
}


Baryon::Baryon()
{
	name = "baryonGun";
	power = 50;
	lastTimeFired = 0;
	fireRate = 80;
	canFire = TRUE;
	maxFireRate = 80;
	load = -1;
	loadable = FALSE;
}

Plasma::Plasma()
{
	name = "plasmaGun";
	power = 50;
	lastTimeFired = 0;
	fireRate = 80;
	canFire = TRUE;
	maxFireRate = 80;
	load = 0;
	loadable = TRUE;
}
