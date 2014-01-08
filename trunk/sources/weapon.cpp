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
	powerLevel = WEAPON_STANDARD;
	activated = FALSE;
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
	for (list<Shoot*>::iterator aLaser = shoots.begin(); aLaser != shoots.end(); ++aLaser)
	{
		Shoot * aL = (*aLaser);
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

void Weapon::upgradeTo(int level)
{

}

void Weapon::checkActivation(Hero * aHero)
{

}

/*
 * Electron functions
 */
Electron::Electron()
{
	name = "electronGun";
	type = ElectronGun;
	power = 30;
	lastTimeFired = 0;
	fireRate = 50;
	canFire = TRUE;
	maxFireRate = 40;
	load = -1;
	loadable = FALSE;
	powerLevel = WEAPON_STANDARD;
	activated = TRUE;

	muzzle.copyFrom(CurrentLevel->loadedObjects.at("muzzle"));
	muzzle.setAnimX(0);
	muzzle.setAnimY(0);
}

void Electron::fire(Hero * aHero)
{
	//If we just started firing
	if (!isFiring)
	{
		CurrentLevel->soundEngine->playSound("mitAttack");
	}

	//If we are in the middle of a rafale
	if(!CurrentLevel->soundEngine->sounds.at("mitAttack")->isPlaying)
	{
		CurrentLevel->soundEngine->playSound("mitLoop");
	}

	isFiring = TRUE;
	checkFire();
	if (canFire)
	{
		shoots.push_back(new ElectronAmmo(aHero->posX - 32, aHero->posY + 16, this));
		canFire = FALSE;
	}

	//Display the firing effect
	muzzle.posX = aHero->posX - 10;
	muzzle.posY = aHero->posY - 16;
	muzzle.updateAnimationFrame();
	muzzle.processDisplay();
}

void Electron::upgradeTo(int aLevel)
{
	switch(aLevel)
	{
		case WEAPON_STANDARD:
			powerLevel = WEAPON_STANDARD;
			power = 30;
			fireRate = 50;
			muzzle.setAnimation("level1");
			break;

		case WEAPON_SUPERIOR:
			powerLevel = WEAPON_SUPERIOR;
			power = 50;
			fireRate = 40;
			muzzle.setAnimation("level1");
			break;

		case WEAPON_FURIOUS:
			powerLevel = WEAPON_FURIOUS;
			power = 70;
			fireRate = 20;
			muzzle.setAnimation("level3");
			break;

		default:
			break;
	}
}

/*
 * Hadron functions
 */
Hadron::Hadron()
{
	name = "hadronGun";
	type = HadronGun;
	power = 250;
	lastTimeFired = 0;
	fireRate = 750;
	canFire = TRUE;
	maxFireRate = 80;
	load = 0;
	loadable = TRUE;
	powerLevel = WEAPON_STANDARD;
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

void Hadron::checkActivation(Hero * aHero)
{
	activated = FALSE;
	if(aHero->quarkLevels.at(QuarkU) > 0 && aHero->quarkLevels.at(QuarkD) > 0)
	{
		activated = TRUE;
		if (aHero->quarkLevels.at(QuarkU) > 15 && aHero->quarkLevels.at(QuarkD) > 15)
		{
			if (aHero->quarkLevels.at(QuarkU) >= 34 && aHero->quarkLevels.at(QuarkD) >= 34)
			{
				upgradeTo(WEAPON_FURIOUS);
				return;
			}

			upgradeTo(WEAPON_SUPERIOR);
			return;
		}
		upgradeTo(WEAPON_STANDARD);
	}
}

/*
 * Baryon functions
 */
Baryon::Baryon()
{
	name = "baryonGun";
	type = BaryonGun;
	power = 5;
	lastTimeFired = 0;
	fireRate = 0;
	canFire = TRUE;
	maxFireRate = 0;
	load = -1;
	loadable = FALSE;
	mahLazor = NULL;
	xImpactPos = 0;
	yImpactPos = 0;

	haloA.copyFrom(CurrentLevel->loadedObjects.at("j_bHaloA"));
	haloA.setAnimX(0);
	haloA.setAnimY(0);
	haloA.opacity = 0.6;

	haloB.copyFrom(CurrentLevel->loadedObjects.at("j_bHaloB"));
	haloB.setAnimX(0);
	haloB.setAnimY(0);
	haloB.display = FALSE;

	hitEnnemy = NULL;
}

//"I’M A’ FIRIN’ MAH LAZOR!!!!!"
void Baryon::fire(Hero * aHero)
{
	isFiring = TRUE;
	if (!mahLazor)
	{
		mahLazor = new Lazer(aHero->posX, aHero->posY, this);
	}

	hitEnnemy = NULL;
	mahLazor->width = 1200;
	checkForCollision();

	mahLazor->animate(aHero->posX, aHero->posY, hitEnnemy, xImpactPos, yImpactPos);
	if(hitEnnemy)
	{
		hitEnnemy->processCollisionWith(mahLazor);
	    ParticleEffect * aParticleEffect = new ParticleEffect();
	    aParticleEffect->createSparkFrom(aHero->posX + xImpactPos + 77, aHero->posY + 32);
	    CurrentLevel->ge->particleEffects.push_back(aParticleEffect);
	}

	haloA.updateAnimationFrame();
	haloA.posX = aHero->posX - 96;
	haloA.posY = aHero->posY - 96;

	haloB.updateAnimationFrame();
	haloB.posX = aHero->posX - 96;
	haloB.posY = aHero->posY - 96;

	haloA.processDisplay();
	haloB.processDisplay();
}

void Baryon::checkForCollision()
{
	for (list<Drawable *>::iterator anElement = CurrentLevel->activeElements.begin() ; anElement != CurrentLevel->activeElements.end(); ++anElement)
	{
		if((*anElement)->isEnemy())
		{
			if(CurrentLevel->pe->collisionDetection(mahLazor, *anElement))
			{
				if (hitEnnemy)
				{
					if(xImpactPos > CurrentLevel->pe->xImpactPos)
					{
						hitEnnemy = dynamic_cast<Enemy *>(*anElement);
						xImpactPos = CurrentLevel->pe->xImpactPos;
						yImpactPos = CurrentLevel->pe->yImpactPos;
					}
				}
				else
				{
					hitEnnemy = dynamic_cast<Enemy *>(*anElement);
					xImpactPos = CurrentLevel->pe->xImpactPos;
					yImpactPos = CurrentLevel->pe->yImpactPos;
				}
			}
		}
	}
}

void Baryon::upgradeTo(int aLevel)
{
	switch(aLevel)
	{
		case WEAPON_STANDARD:
			powerLevel = WEAPON_STANDARD;
			power = 5;
			haloA.setAnimation("level1");
			haloB.display = FALSE;
			break;

		case WEAPON_SUPERIOR:
			powerLevel = WEAPON_SUPERIOR;
			power = 10;
			mahLazor->power = power;
			haloA.setAnimation("level2");
			haloB.display = TRUE;
			break;

		case WEAPON_FURIOUS:
			powerLevel = WEAPON_FURIOUS;
			power = 15;
			mahLazor->power = power;
			haloA.setAnimation("level2");
			haloB.display = TRUE;
			break;

		default:
			break;
	}

	if(mahLazor)
		mahLazor->upgrade(aLevel);
}

void Baryon::checkActivation(Hero * aHero)
{
	activated = FALSE;
	if (aHero->quarkLevels.at(QuarkC) > 0 && aHero->quarkLevels.at(QuarkS) > 0)
	{
		activated = TRUE;
		if (aHero->quarkLevels.at(QuarkC) > 15 && aHero->quarkLevels.at(QuarkS) > 15)
		{
			if (aHero->quarkLevels.at(QuarkC) >= 34 && aHero->quarkLevels.at(QuarkS) >= 34)
			{
				upgradeTo(WEAPON_FURIOUS);
				return;
			}

			upgradeTo(WEAPON_SUPERIOR);
			return;
		}
	 	upgradeTo(WEAPON_STANDARD);
	}
}

/*
 * Plasma functions
 */
Plasma::Plasma()
{
	name = "plasmaGun";
	type = PlasmaGun;
	power = 50;
	lastTimeFired = 0;
	fireRate = 80;
	canFire = TRUE;
	maxFireRate = 80;
	load = 0;
	loadable = TRUE;
	powerLevel = WEAPON_STANDARD;
}

void Plasma::fire(Hero * aHero)
{
	isFiring = TRUE;
	shoots.push_back(new Photon(aHero->posX - 64, aHero->posY + 16, this));
}

void Plasma::checkActivation(Hero * aHero)
{
	activated = FALSE;
	 if (aHero->quarkLevels.at(QuarkC) > 0 && aHero->quarkLevels.at(QuarkS) > 0 &&
		 aHero->quarkLevels.at(QuarkB) > 0 && aHero->quarkLevels.at(QuarkT) > 0 &&
		 aHero->quarkLevels.at(QuarkD) > 0 && aHero->quarkLevels.at(QuarkU) > 0)
	 {
		 activated = TRUE;
		 if (aHero->quarkLevels.at(QuarkC) > 15 && aHero->quarkLevels.at(QuarkS) > 15)
		 {
			 if (aHero->quarkLevels.at(QuarkC) >= 34 && aHero->quarkLevels.at(QuarkS) >= 34)
			 {
				 upgradeTo(WEAPON_FURIOUS);
				 return;
			 }

			 upgradeTo(WEAPON_SUPERIOR);
			 return;
		 }
		 upgradeTo(WEAPON_STANDARD);
	 }
}
