/*
 * Contains weapon functions
 */

#include "weapon.h"

Weapon::Weapon()
{
	power = 50;
	lastTimeFired = 0;
	fireRate = 80;
	canFire = true;
	maxFireRate = 80;
	load = -1;
	maxPower = 0.0;
	loadable = false;
	isFiring = false;
	powerLevel = WEAPON_STANDARD;
	activated = false;
	type = ElectronGun;
}

void Weapon::fire(Hero * aHero)
{

}

void Weapon::checkFire()
{
	unsigned int nextFireTime = lastTimeFired + fireRate;
	if (nextFireTime < GameTimer)
	{
		canFire = true;
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
	CurrentLevel->createParticleEffect(x, y, "electronImpact");
}

void Weapon::upgradeTo(int level)
{

}

void Weapon::checkActivation(Hero * aHero)
{

}

void Weapon::releaseFire(Hero * aHero)
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
	canFire = true;
	maxFireRate = 40;
	load = -1;
	loadable = false;
	powerLevel = WEAPON_STANDARD;
	activated = true;

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
		CurrentLevel->soundEngine->playSound("mitLoop", aHero->posX);
	}

	isFiring = true;
	checkFire();
	if (canFire)
	{
		shoots.push_back(new ElectronAmmo(aHero->posX, aHero->posY, this));
		canFire = false;
	}

	//Display the firing effect
	muzzle.posX = aHero->posX + 12;
	muzzle.posY = aHero->posY;
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
	canFire = true;
	maxFireRate = 80;
	load = 0;
	loadable = true;
	loadFactor = 0.0f;
	maxPower = 1500.0;

	powerLevel = WEAPON_STANDARD;
}

void Hadron::fire(Hero * aHero)
{
	isFiring = true;
	if(powerLevel == WEAPON_FURIOUS)
	{
		load = min(maxPower, load + loadFactor);
	}
	else
	{
		load = min(maxPower * 0.97f, load + loadFactor);
	}
}

void Hadron::releaseFire(Hero * aHero)
{

	//LOad  100 500
	//Number  3 38
	//Size
	int numberOfParticles = 3 + (load / 1500) * 35;
			//(aHero->quarkLevels.at(QuarkU) + aHero->quarkLevels.at(QuarkD)) / 2;
	vector<float> particleAngles = getNormalDistributionNumbers(numberOfParticles, 0, 2, NULL);

	int aLoad;
	for(int i = 0; i < numberOfParticles; i++)
	{
		aLoad = (20 + rand() % 80);
		shoots.push_back(new HadronParticle(aHero->posX, aHero->posY, 0.12f, particleAngles.at(i), aLoad, this));
	}
	load = 0.0f;
}

void Hadron::checkActivation(Hero * aHero)
{
	activated = false;
	float loadPower = (aHero->quarkLevels.at(QuarkU) + aHero->quarkLevels.at(QuarkD)) / 2;
	loadPower = max(1.0f, loadPower);
	loadFactor = maxPower/ (600.0 - (loadPower * (600.0/35.0)));

	if(aHero->quarkLevels.at(QuarkU) > 0 && aHero->quarkLevels.at(QuarkD) > 0)
	{
		activated = true;
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

void Hadron::upgradeTo(int aLevel)
{
	powerLevel = (weaponState) aLevel;
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
	canFire = true;
	maxFireRate = 0;
	load = -1;
	loadable = false;
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
	haloB.display = false;

	hitEnnemy = NULL;
}

//"I’M A’ FIRIN’ MAH LAZOR!!!!!"
void Baryon::fire(Hero * aHero)
{
	isFiring = true;
	if (!mahLazor)
	{
		mahLazor = new Lazer(aHero->posX, aHero->posY, this);
	}

	hitEnnemy = NULL;
	mahLazor->width = 1200;
	mahLazor->posX = aHero->posX + mahLazor->width/2;
	checkForCollision();

	mahLazor->animate(aHero->posX, aHero->posY, hitEnnemy, xImpactPos, yImpactPos);
	if(hitEnnemy)
	{
		hitEnnemy->processCollisionWith(mahLazor);
		CurrentLevel->createParticleEffect(aHero->posX + xImpactPos, aHero->posY, "lazerSpark");
	}

	haloA.updateAnimationFrame();
	haloA.posX = aHero->posX;
	haloA.posY = aHero->posY;

	haloB.updateAnimationFrame();
	haloB.posX = aHero->posX;
	haloB.posY = aHero->posY;

	haloA.processDisplay();
	haloB.processDisplay();
}

void Baryon::checkForCollision()
{
	for(list<EnemyWave *>::iterator aWave = CurrentLevel->activeWaves.begin(); aWave != CurrentLevel->activeWaves.end(); aWave++)
	{
		for(list<Enemy *>::iterator anElement = (*aWave)->enemies.begin(); anElement != (*aWave)->enemies.end(); ++anElement)
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
			haloB.display = false;
			break;

		case WEAPON_SUPERIOR:
			powerLevel = WEAPON_SUPERIOR;
			power = 10;
			haloA.setAnimation("level2");
			haloB.display = true;
			break;

		case WEAPON_FURIOUS:
			powerLevel = WEAPON_FURIOUS;
			power = 15;
			haloA.setAnimation("level2");
			haloB.display = true;
			break;

		default:
			break;
	}

	if(mahLazor)
		mahLazor->upgrade(aLevel);
}

void Baryon::checkActivation(Hero * aHero)
{
	activated = false;
	if (aHero->quarkLevels.at(QuarkC) > 0 && aHero->quarkLevels.at(QuarkS) > 0)
	{
		activated = true;
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
	canFire = true;
	maxFireRate = 80;
	load = 0;
	loadable = true;
	powerLevel = WEAPON_STANDARD;
}

void Plasma::fire(Hero * aHero)
{
	isFiring = true;
	shoots.push_back(new HadronAmmo(aHero->posX - 64, aHero->posY, this));
}

void Plasma::checkActivation(Hero * aHero)
{
	activated = false;
	 if (aHero->quarkLevels.at(QuarkC) > 0 && aHero->quarkLevels.at(QuarkS) > 0 &&
		 aHero->quarkLevels.at(QuarkB) > 0 && aHero->quarkLevels.at(QuarkT) > 0 &&
		 aHero->quarkLevels.at(QuarkD) > 0 && aHero->quarkLevels.at(QuarkU) > 0)
	 {
		 activated = true;
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
