/*
 * Contains laser functions
 */

#include "laser.h"

//80x34
Shoot::Shoot()
{
	name = "laser";
	power = 50;
	posX = 0;
	posY = 0;
	toBlend = TRUE;
	angle = 0.0f;
	setAnimX(0);
	setAnimY(0);
	firingWeapon = NULL;
	collision = NULL;
}

Shoot::Shoot(int x, int y, Weapon * aWeapon)
{
	power = 50;
	firingWeapon = aWeapon;
	copyFrom(lev->loadedObjects.at("electron"));

	posX = x;
	posY = y;
	scaleX = 0.5;
	scaleY = 0.5;
	rotZ = -1.0;
	rotationAngle = -4.0f +	static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/8.0f));
	angle = rotationAngle * (PI / 180.0);
	toBlend = TRUE;
	setAnimX(0);
	setAnimY(0);
}

void Shoot::animate()
{
	if (display)
	{
		//Compute the next movement vector
		//Handle by the physic engine ?
		float vx, vy;
		vx = 20 * cos(angle);
		vy = 20 * sin(angle);

		posX = posX + vx;
		posY = posY + vy;
	}

	if(!lev->isOnScreen(this))
	{
		display = FALSE;
		toRemove = TRUE;
	}
}

void Shoot::processCollisionWith(Drawable * aDrawable)
{
    //firingWeapon->createImpact(posX + 120, posY + 16);
    ParticleEffect * aParticleEffect = new ParticleEffect();
    aParticleEffect->createImpactFrom(posX + 77, posY + 16);
    lev->ge->particleEffects.push_back(aParticleEffect);

	display = FALSE;

	if (aDrawable->isHero())
	{
		Hero * myHero = dynamic_cast<Hero*>(aDrawable);
		if (!myHero->invincible)
		{
			toRemove = TRUE;
		}
	}
}

/*
 * ElectronAmmo functions
 */
ElectronAmmo::ElectronAmmo(int x, int y, Weapon * aWeapon)
{
	firingWeapon = aWeapon;
	power = firingWeapon->power;
	copyFrom(lev->loadedObjects.at("electron"));

	posX = x;
	posY = y;
	scaleX = 0.5;
	scaleY = 0.5;
	rotZ = -1.0;
	rotationAngle = -4.0f +	static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/8.0f));// -4 + (rand() % 8);
	angle = rotationAngle * (PI / 180.0);
	toBlend = TRUE;
	setAnimX(0);
	setAnimY(0);

	switch(firingWeapon->powerLevel)
	{
		case WEAPON_STANDARD:
			setAnimation("level1");
			break;

		case WEAPON_SUPERIOR:
			setAnimation("level2");
			break;

		case WEAPON_FURIOUS:
			setAnimation("level3");
			break;

		default:
			break;
	}
}

/*
 * Photon functions
 */
Photon::Photon()
{
	power = 50;//firingWeapon->power;
	firingWeapon = NULL;
	copyFrom(lev->loadedObjects.at("photon"));
	trail.copyFrom(lev->loadedObjects.at("photonTrail"));
	trail.toBlend = TRUE;
	trail.setAnimX(0);
	trail.setAnimY(0);
	toBlend = TRUE;
	setAnimX(0);
	setAnimY(0);
	//setAnimation("animDefaut");
}

Photon::Photon(int x, int y, Weapon * aWeapon)
{
	firingWeapon = aWeapon;
	power = firingWeapon->power;
	copyFrom(lev->loadedObjects.at("photon"));
	trail.copyFrom(lev->loadedObjects.at("photonTrail"));
	trail.posX = x + 96;
	trail.posY = y - 28;
	trail.toBlend = TRUE;
	trail.setAnimX(0);
	trail.setAnimY(0);
	posX = x + 50;
	posY = y - 50;
	toBlend = TRUE;
	setAnimX(0);
	setAnimY(0);
}

void Photon::setParam(int x, int y, Weapon * aWeapon)
{
	firingWeapon = aWeapon;
	posX = x + 50;
	posY = y - 50;
	trail.posX = x + 96;
	trail.posY = y - 28;
}

void Photon::animate()
{
	float previousWidth = trail.currentAnimation->width;
	trail.currentAnimation->width = trail.width;
	trail.updateAnimationFrame();

	if (display)
	{
		posX = posX + 8.75;
	//	updateAnimationFrame();
		trail.currentAnimation->width = min(trail.width, (int)(posX - trail.posX + 124));
	}

	if(!display)
	{
		trail.currentAnimation->width = max(0.0, previousWidth - 8.5);
		trail.posX = trail.posX + 8.5;
	}

	trail.setAnimX(trail.getAnimX());
	trail.processDisplay();

	if (trail.currentAnimation->hasEnded)
	{
		display = FALSE;
		toRemove = TRUE;
		trail.display = FALSE;
		trail.toRemove = TRUE;
	}
}

void Photon::processCollisionWith(Drawable * aDrawable)
{

}

void Photon::removeEnergy(int anEnergyValue)
{
	power = power - anEnergyValue;

	if(power <= 0)
	{
		display = FALSE;
		toRemove = TRUE;
	}
}

/*
 * Lazer Functions
 */
Lazer::Lazer()
{
	power = 1;
	firingWeapon = NULL;

	copyFrom(lev->loadedObjects.at("j_bA_Sustain"));

	lightning.copyFrom(lev->loadedObjects.at("j_bA_Sustain"));
	lightning.setAnimX(0);
	lightning.setAnimY(0);

	attack.copyFrom(lev->loadedObjects.at("j_bA_Attack"));
	release.copyFrom(lev->loadedObjects.at("j_bA_Release"));

	toBlend = TRUE;
	setAnimX(0);
	setAnimY(0);
}

Lazer::Lazer(int x, int y, Weapon * aWeapon)
{
	power = 5;
	firingWeapon = aWeapon;

	copyFrom(lev->loadedObjects.at("j_bA_Sustain"));
	width = SCREEN_WIDTH;

	lightning.copyFrom(lev->loadedObjects.at("jSp_bEclair"));
	lightning.setAnimX(0);
	lightning.setAnimY(0);

	burningFlames.copyFrom(lev->loadedObjects.at("jSp_bCrame"));
	burningFlames.setAnimX(0);
	burningFlames.setAnimY(0);

	attack.copyFrom(lev->loadedObjects.at("j_bA_Attack"));
	release.copyFrom(lev->loadedObjects.at("j_bA_Release"));

	lightning.posX = x + 30;
	lightning.posY = y + 20;

	burningFlames.posX = x + 30;
	burningFlames.posY = y + 20;

	attack.posX = x + width;
	attack.posY = y - 16;
	attack.setAnimX(0);
	attack.setAnimY(0);

	release.posX = x - 12;
	release.posY = y - 16;
	release.setAnimX(0);
	release.setAnimY(0);

	posX = x + 93;
	posY = y - 16;
	toBlend = TRUE;
	setAnimX(0);
	setAnimY(0);
}

void Lazer::animate(float x, float y, Enemy * anHitEnemy, float xImpactPos, float yImpactPos)
{
	//If we have a collision with an enemy
	if(anHitEnemy)
	{
		//Shorten the lazer down to the collision point
		width = max(1.0f, xImpactPos);

		//Display flames
		burningFlames.posX = posX + xImpactPos - burningFlames.width/2;
		burningFlames.posY = y + 32 - burningFlames.height/2;
		burningFlames.updateAnimationFrame();
		burningFlames.processDisplay();
	}

	//Shorten the lightnings equally
	lightning.currentAnimation->width = min(1200, width + 48);
	lightning.updateAnimationFrame();

	lightning.posX = x + 30;
	lightning.posY = y + 20;

	//Update opacity
	attack.updateAnimationFrame();
	attack.posX = x + width + 83 - 48;
	attack.posY = y - 16;

	//Update opacity
	release.updateAnimationFrame();
	//Reset the width
	release.width = 95;
	release.currentAnimation->width = release.width;
	release.posX = x - 12;
	release.posY = y - 16;

	posX = x + 83;
	posY = y - 16;

	//If the lazer is shorten so that the width of the sustain part is zero
	//Then we must also shorten the release part to make sure it does not overlap the attack part
	if(attack.posX < release.posX + release.width)
	{
		release.width = release.width - ((release.posX + release.width) - attack.posX);
		release.currentAnimation->width = release.width;
	}

	width = max(0.0f, width - 48.0f);
	currentAnimation->width = width;
	updateAnimationFrame();

	//Display
	processDisplay();
	lightning.processDisplay();
	release.processDisplay();
	attack.processDisplay();
}

void Lazer::upgrade(int aLevel)
{
	power = firingWeapon->power;
	switch(aLevel)
	{
		case WEAPON_STANDARD:
			attack.setAnimation("level1");
			release.setAnimation("level1");
			setAnimation("level1");
			currentAnimation->width = width;
			break;

		case WEAPON_SUPERIOR:
			attack.setAnimation("level2");
			release.setAnimation("level2");
			setAnimation("level2");
			currentAnimation->width = width;
			break;

		case WEAPON_FURIOUS:
			attack.setAnimation("level3");
			release.setAnimation("level3");
			setAnimation("level3");
			currentAnimation->width = width;
			break;

		default:
			break;
	}
}
