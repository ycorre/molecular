/*
 * Contains laser functions
 */

#include "laser.h"

//80x34
Laser::Laser()
{
	name = "laser";
	power = 50;
	posX = 0;
	posY = 0;
	toBlend = TRUE;
	angle = 0.0f;
	setAnimX(0);
	setAnimY(0);
	collision = ge->loadTexture("res/action/characters/j_eMask.png");
	firingWeapon = NULL;
}

Laser::Laser(int x, int y, Weapon * aWeapon)
{
	power = 50;
	firingWeapon = aWeapon;
	copyFrom(lev->loadedObjects.at("electron"));
	collision = ge->loadTexture("res/action/characters/j_eMask.png");
	trail.copyFrom(lev->loadedObjects.at("electronTrail"));
	trail.posX = x + width/2 - 30;
	trail.posY = y + 10;
	trail.toBlend = TRUE;
	trail.setAnimX(0);
	trail.setAnimY(0);
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
	setAnimation("animDefaut");
}

void Laser::animate()
{
	trail.currentAnimation->width = trail.width;
	trail.updateAnimationFrame();

	if (display)
	{
		//Compute the next movement vector
		//Handle by the physic engine ?
		float vx, vy;
		vx = (20)*cos(angle);
		vy = (20)*sin(angle);

		posX = posX + vx;
		posY = posY + vy;
	}

	/*trail.currentAnimation->width = min(1200, (int)(this->posX - trail.posX + 124));
	trail.setAnimX(trail.getAnimX());
	trail.processDisplay();*/

	if (trail.currentAnimation->hasEnded)
	{
		display = FALSE;
		toRemove = TRUE;
		trail.display = FALSE;
		trail.toRemove = TRUE;
	}
}

void Laser::processCollisionWith(Drawable * aDrawable)
{
    //firingWeapon->createImpact(posX + 120, posY + 16);
    ParticleEffect * aParticleEffect = new ParticleEffect();
    aParticleEffect->createImpactFrom(posX + 120, posY + 16);
    lev->ge->particleEffects.push_back(aParticleEffect);
	display = FALSE;

	if (aDrawable->isHero())
	{
		Hero * myHero = dynamic_cast<Hero*>(aDrawable);
		if (!myHero->invincible)
		{
			this->toRemove = TRUE;
		}
	}
}


/*
 * Photon functions
 */

Photon::Photon()
{
	power = 50;//firingWeapon->power;
	copyFrom(lev->loadedObjects.at("photon"));
	collision = ge->loadTexture("res/action/characters/j_eMask.png");
	trail.copyFrom(lev->loadedObjects.at("photonTrail"));
	trail.toBlend = TRUE;
	trail.setAnimX(0);
	trail.setAnimY(0);
	toBlend = TRUE;
	setAnimX(0);
	setAnimY(0);
	setAnimation("animDefaut");
}

Photon::Photon(int x, int y, Weapon * aWeapon)
{
	firingWeapon = aWeapon;
	power = firingWeapon->power;
	copyFrom(lev->loadedObjects.at("photon"));
	collision = ge->loadTexture("res/action/characters/j_eMask.png");
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
	setAnimation("animDefaut");
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
		updateAnimationFrame();
		trail.currentAnimation->width = min(trail.width, (int)(this->posX - trail.posX + 124));
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
