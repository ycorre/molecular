#include "enemy.h"



Copper::Copper(Json::Value aConfig)
{
	copyFrom(lev->loadedObjects.at("e005Sp"));

	posX = aConfig.get("posX", -1.0).asFloat();
	posX = posX + SCREEN_WIDTH;

	posY = aConfig.get("posY", -1.0).asFloat();
	if(posY == -1.0)
		posY = 30 + rand() % (GAMEZONE_HEIGHT - width - 30);

	scoreValue = aConfig.get("scoreValue", 1000).asInt();
	bonusProbability = aConfig.get("bonusProbability", 0).asInt();
	life = aConfig.get("life", 1000).asInt();
	speed = aConfig.get("speed", 2.0).asFloat();
	damage = aConfig.get("damage", 3000).asInt();
	lifeTime = aConfig.get("lifeTime", 3000).asInt();
	xLimit = aConfig.get("xLimit", SCREEN_WIDTH - 100).asFloat();
	currentLifeTime = 0;
	lifePhase = COPPER_ARRIVING;
	canFire = false;
	yDestination = -1;
	invincible = true;
	ySpeed = 0;
	spinningAngle = 0;
	spinningCounter = 0;
	activeCannons = 3;

	setAnimX(0);
	setAnimY(0);

	cannons.push_back(new CopperCannon(this, 87, 51));
	cannons.push_back(new CopperCannon(this, 33, 20));
	cannons.push_back(new CopperCannon(this, 33, 82));

	currentAnimation->setFrameTo(15);
	currentAnimation->currentFrame = 15;
}

void Copper::animate()
{
	for(vector<CopperCannon *>::iterator aCanon = cannons.begin(); aCanon != cannons.end(); ++aCanon)
	{
		(*aCanon)->animate();
	}

	//If we have no cannons left then we become vulnerable
	if(!activeCannons)
		invincible = false;

	currentLifeTime++;
	bool stillShooting = false;

	switch(lifePhase)
	{
		case COPPER_ARRIVING:
			if(posX <= xLimit)
				nextPhase();

		case COPPER_LEAVING:
			posX = posX - 3;
			break;

		case COPPER_STEADY:
			move();
			break;

		case COPPER_FIRE:
			for(vector<CopperCannon *>::iterator aCanon = cannons.begin(); aCanon != cannons.end(); ++aCanon)
			{
				if((*aCanon)->activated)
				{
					stillShooting = true;
				}
			}

			if(!stillShooting)
				nextPhase();

			break;

		case COPPER_SPIN:
			if(updateAnimationFrame())
			{
				for(vector<CopperCannon *>::iterator aCanon = cannons.begin(); aCanon != cannons.end(); ++aCanon)
				{
					(*aCanon)->copperAngle = (*aCanon)->copperAngle - (3.6 * (PI/180));
				}

				spinningCounter = (spinningCounter + 1) % spinningAngle;
				if(!spinningCounter)
				{
					nextPhase();
				}
			}
			break;

		default:
			break;
	}

	//If it is here since lifetime frame, move out
	if(currentLifeTime >= lifeTime)
	{
		lifePhase = COPPER_LEAVING;
	}
}

void Copper::nextPhase()
{
	lifePhase = rand() % 3;

	switch(lifePhase)
	{
		//Move vertically
		case COPPER_STEADY:
			if(yDestination == -1)
			{
				//Pull a y coordinate as destination,
				//we want it at least one width away from our current position
				do{
					yDestination =  30 + rand() % (GAMEZONE_HEIGHT - width - 30);
				} while(std::abs((int)(posY - yDestination)) < width);

				ySpeed = (posY - yDestination) / 50.0;
			}
			break;

		//Fire the cannons
		case COPPER_FIRE:
			//If we have some cannons left that is
			if(activeCannons)
			{
				int activatedCannons;
				//Pull the number of cannons to activate
				activatedCannons = 1 + rand() % activeCannons;
				int i;
				for(i = 0; i < activatedCannons; i++)
				{
					while(cannons.at(i)->destroyed){
						i++;
					}
					cannons.at(i)->activated = true;
					cannons.at(i)->startShootingAngle = cannons.at(i)->shootingAngle;
				}
			}
			else	//reroll
			{
				nextPhase();
			}
			break;

		//Let's spin! WEEEEEEEEEEEEEEEE!!!
		case COPPER_SPIN:
			//Pull an angle between 36 and 360
			//Given as a number of frames since 1 frame = 3.6 deg
			spinningAngle = 10 + rand() % 90;
			spinningCounter = 0;
			break;

		default:
			break;
	}

}

void Copper::move()
{
	posY = posY - ySpeed;

	if(std::abs((int)(posY - yDestination)) < std::abs((int)(ySpeed) + 1))
	{
		yDestination = -1;
		nextPhase();
	}
}

void Copper::processCollisionWith(Drawable * aDrawable)
{
	if (aDrawable->isLaser())
	{
		Shoot * aLaser =  dynamic_cast<Shoot*>(aDrawable);

		if(invincible)
		{
			for(vector<CopperCannon *>::iterator aCanon = cannons.begin(); aCanon != cannons.end(); ++aCanon)
			{
				if(!(*aCanon)->destroyed)
				{
					if(aLaser->display && CurrentLevel->pe->collisionDetection(aLaser, (*aCanon)))
					{
						(*aCanon)->processCollisionWith(aLaser);
						aLaser->processCollisionWith(*aCanon);
					}
				}
			}
		}
		else
		{
			int lifeValue = life;
			life = max(0, life - aLaser->power);
			if(aLaser->isPhoton())
			{
				HadronAmmo * aPhoton =  dynamic_cast<HadronAmmo *>(aDrawable);
				aPhoton->removeEnergy(lifeValue - life);
			}
			if (life<=0)
			{
				lev->soundEngine->playSound("xwing_explode");
				lev->createExplosion(posX + width/2, posY + height/2);
				lev->createExplosion(posX, posY);
				lev->createExplosion(posX + 2*width/3, posY + height/3);
				lev->createExplosion(posX + width/3, posY + 2*height/3);
				Score = Score + scoreValue * (type + 1);
				toRemove = true;
				dropBonus(posX, posY);
			}
			else
			{
				startBlinkingWhite(4);
			}
		}
	}
}

/*
 * CopperCanon functions
 */
CopperCannon::CopperCannon(Copper * aCopper, float x, float y)
{
	copyFrom(lev->loadedObjects.at("e005CanonSp"));

	shootingAngle = startShootingAngle = 0;
	copper = aCopper;
	shiftX = copper->width/2 - width/2;
	shiftY = copper->height/2 - height/2;
	posX = copper->posX + x - width/2;
	posY = copper->posY + y - height/2;
	float xDiff = (copper->posX + copper->width/2) - (posX + width/2);
	float yDiff = (copper->posY + copper->height/2) - (posY + height/2);

	copperAngle = atan2(yDiff, xDiff);
	posX = copper->posX + shiftX + 36 * cos(copperAngle);
	posY = copper->posY + shiftY + 36 * sin(copperAngle);

	destroyed = false;
	life = 750;

	activated = false;

	int j = 35;
	for(int i = 360; i > 0; i--)
	{
		cannonAngles.push_back(j);

		if(i % 5 == 0)
		{
			j = (j - 1 + 72) % 72;
		}
	}
}

void CopperCannon::animate()
{
	posX = copper->posX + shiftX + 36 * cos(copperAngle);
	posY = copper->posY + shiftY + 36 * sin(copperAngle);

	if(updateAnimationFrame() && !destroyed)
	{
		//Cannot perform modulo on negative number
		//Thus do +355 instead of -5
		shootingAngle = (shootingAngle + 355) % 360;

		if(activated)
		{
			fire();
			if(shootingAngle == startShootingAngle)
			{
				activated = false;
			}
		}
		else
		{
			float xDiff = (CurrentLevel->hero->posX + CurrentLevel->hero->width/2) - (posX + width/2);
			float yDiff = (CurrentLevel->hero->posY + CurrentLevel->hero->height/2) - (posY + height/2);
			int angle = ((int)(atan2(yDiff, xDiff) * (180/PI) + 360)) % 360;
			currentAnimation->setFrameTo(cannonAngles.at(angle));
		}
	}

	processDisplay();
}

void CopperCannon::fire()
{
	CurrentLevel->activeElements.push_back(new CopperAmmo(posX + width/2, posY + height/2, shootingAngle * (PI/180), 4));
}

void CopperCannon::processCollisionWith(Drawable * aDrawable)
{
	if(!destroyed)
	{
		if (aDrawable->isLaser())
		{
			Shoot * aLaser =  dynamic_cast<Shoot*>(aDrawable);
			int lifeValue = life;
			life = max(0, life - aLaser->power);
			if(aLaser->isPhoton())
			{
				HadronAmmo * aPhoton =  dynamic_cast<HadronAmmo *>(aDrawable);
				aPhoton->removeEnergy(lifeValue - life);
			}
			if (life<=0)
			{
				lev->soundEngine->playSound("xwing_explode");
				lev->createExplosion(posX + width/2, posY + height/2);
				Score = Score + scoreValue * (type + 1);
				destroyed = true;
				activated = false;
				copper->activeCannons--;
				setAnimation("canoff");
			}
			else
			{
				startBlinkingWhite(4);
			}
		}
	}
}
