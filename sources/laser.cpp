/*
 * Contains laser functions
 */

#include "include/laser.h"

//80x34
Shoot::Shoot()
{
	name = "laser";
	power = 50;
	posX = 0;
	posY = 0;
	toBlend = true;
	angle = 0.0f;
	firingWeapon = NULL;
	collision = NULL;
	speed = 10;
}

Shoot::Shoot(int x, int y, Weapon * aWeapon)
{
	power = 50;
	firingWeapon = aWeapon;
	copyFrom(CurrentLevel->loadedObjects.at("electron"));

	posX = x;
	posY = y;
	scaleX = 0.5;
	scaleY = 0.5;
	rotZ = -1.0;
	rotationAngle = -4.0f +	static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/8.0f));
	angle = rotationAngle * (PI / 180.0);
	speed = 10;
	toBlend = true;
}

void Shoot::animate()
{
	if (display)
	{
		//Compute the next movement vector
		//Handle by the physic engine ?
		float vx, vy;
		vx = speed * cos(angle);
		vy = speed * sin(angle);

		posX = posX + vx;
		posY = posY + vy;
	}

	if(!CurrentLevel->isOnScreen(this))
	{
		display = false;
		toRemove = true;
	}
}

void Shoot::processCollisionWith(Drawable * aDrawable)
{
	CurrentLevel->createParticleEffect(posX + getWidthBoundary()/2 - 13, posY, "electronSpark");

	int soundNumber = 1 + rand() % 8;
	stringstream soundName;
	soundName << "eImp" << soundNumber;
	CurrentLevel->soundEngine->playSound(soundName.str(), posX);

	toRemove = true;
}

/*
 * ElectronAmmo functions
 */
ElectronAmmo::ElectronAmmo(int x, int y, Weapon * aWeapon)
{
	firingWeapon = aWeapon;
	power = firingWeapon->power;
	copyFrom(CurrentLevel->loadedObjects.at("electron"));

	posX = x;
	posY = y;
	scaleX = 0.5;
	scaleY = 0.5;
	rotZ = -1.0;
	speed = 20;
	rotationAngle = -4.0f +	static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/8.0f));
	angle = rotationAngle * (PI / 180.0);

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
 * Hadron Particle
 */
HadronParticle::HadronParticle(float x, float y, float aScale, float anAngle, int aLoad, Weapon * aWeapon)
{
	firingWeapon = aWeapon;
	power = aLoad;
	copyFrom(CurrentLevel->loadedObjects.at("hadronParticle"));
	speed = 10.0;

	angle = anAngle * (PI/180.0);
	scaleX = scaleY = (float) power / 200.0f;
	speed = speed + (float) power / 33.0f;

	star.copyFrom(CurrentLevel->loadedObjects.at("hadronStar"));
	star.scaleX = star.scaleY = scaleX * 2.0f;
	star.posX = x;
	star.posY = y;

	posX = x;
	posY = y;
}

void HadronParticle::removeEnergy(int anEnergyValue)
{
	power = power - anEnergyValue;

	if(power < 1.0)
	{
		display = false;
		toRemove = true;
	}
}

void HadronParticle::animate()
{
	float vx, vy;
	vx = speed * cos(angle);
	vy = speed * sin(angle);

	posX = posX + vx;
	posY = posY + vy;

	star.posX = posX;
	star.posY = posY;
	star.updateAnimationFrame();
	star.processDisplay();

	if(!CurrentLevel->isOnScreen(this))
	{
		display = false;
		toRemove = true;
	}
}

//Override the collision function with a collision that do nothing so that
//it does not remove the photon
void HadronParticle::processCollisionWith(Drawable * aDrawable)
{

}

/*
 * Lazer Functions
 */
Lazer::Lazer()
{
	power = 1;
	firingWeapon = NULL;

	copyFrom(CurrentLevel->loadedObjects.at("j_bA_Sustain"));

	lightning.copyFrom(CurrentLevel->loadedObjects.at("j_bA_Sustain"));

	attack.copyFrom(CurrentLevel->loadedObjects.at("j_bA_Attack"));
	release.copyFrom(CurrentLevel->loadedObjects.at("j_bA_Release"));

	toBlend = true;
}

Lazer::Lazer(int x, int y, Weapon * aWeapon)
{
	power = 5;
	firingWeapon = aWeapon;

	copyFrom(CurrentLevel->loadedObjects.at("j_bA_Sustain"));
	width = SCREEN_WIDTH;

	lightning.copyFrom(CurrentLevel->loadedObjects.at("jSp_bEclair"));
	lightning.display = false;

	burningFlames.copyFrom(CurrentLevel->loadedObjects.at("jSp_bCrame"));

	attack.copyFrom(CurrentLevel->loadedObjects.at("j_bA_Attack"));
	release.copyFrom(CurrentLevel->loadedObjects.at("j_bA_Release"));

	lightning.posX = x;
	lightning.posY = y;

	burningFlames.posX = x;
	burningFlames.posY = y;

	attack.posX = x + width;
	attack.posY = y;

	release.posX = x - 12;
	release.posY = y;

	posX = x + 47 + width/2;
	posY = y;
	toBlend = true;
}

void Lazer::animate(float x, float y, Enemy * anHitEnemy, float xImpactPos, float yImpactPos)
{
	//If we have a collision with an enemy
	if(anHitEnemy)
	{
		//Shorten the lazer down to the collision point
		width = max(1.0f, xImpactPos - attack.width/2);

		//Display flames
		burningFlames.posX = x + xImpactPos - 7;
		burningFlames.posY = y;
		burningFlames.updateAnimationFrame();
		burningFlames.processDisplay();
	}

	//Shorten the lightnings equally
	lightning.currentAnimation->width = min(1200.0f, width + attack.width/2);
	lightning.updateAnimationFrame();

	lightning.posX = x + width/2;
	lightning.posY = y;

	//Update opacity
	attack.updateAnimationFrame();
	attack.posX = x + width + attack.width/2;
	attack.posY = y;

	//Update opacity
	release.updateAnimationFrame();
	//Reset the width
	release.width = 95;
	release.currentAnimation->width = release.width;
	release.posX = x;
	release.posY = y;

	posX = x + width/2 + attack.width/4;
	posY = y;

	//If the lazer is shortened so that the width of the sustain part is zero
	//Then we must also shorten the release part to make sure it does not overlap the attack part
	if(attack.posX - attack.width/2 <= release.posX + release.width/2)
	{
		float widthDiff = ((release.posX + release.width/2) - (attack.posX - attack.width/2));
		release.width = release.width - widthDiff;
		release.currentAnimation->width = release.width;
		release.posX = release.posX - widthDiff/2;
	}

	width = max(0.0f, width - 47.5f);
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
			lightning.display = false;
			currentAnimation->width = width;
			break;

		case WEAPON_SUPERIOR:
			attack.setAnimation("level2");
			release.setAnimation("level2");
			setAnimation("level2");
			lightning.display = false;
			currentAnimation->width = width;
			break;

		case WEAPON_FURIOUS:
			attack.setAnimation("level3");
			release.setAnimation("level3");
			setAnimation("level3");
			lightning.display = true;
			currentAnimation->width = width;
			break;

		default:
			break;
	}
}

void Lazer::processCollisionWith(Drawable * aDrawable)
{

}
