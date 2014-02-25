#include "enemy.h"

Copper::Copper(Json::Value aConfig)
{
	copyFrom(CurrentLevel->loadedObjects.at("e005Sp"));

	posX = aConfig.get("posX", -1.0).asFloat();
	posX = posX + SCREEN_WIDTH;

	posY = aConfig.get("posY", -1.0).asFloat();
	if(posY == -1.0)
		posY = GAMEZONE_LIMIT + width/2 + rand() % (GAMEZONE_HEIGHT - (int)width/2 - 30);

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
	collidable = false;
	activeCannons = 3;

	setAnimX(0);
	setAnimY(0);

	cannons.push_back(new CopperCannon(this, 51-87, 51-51));
	cannons.push_back(new CopperCannon(this, 51-33, 51-20));
	cannons.push_back(new CopperCannon(this, 51-33, 51-82));

	currentAnimation->setFrameTo(99);
	currentAnimation->currentFrame = 99;
}

void Copper::animate()
{
	//If we have no cannons left then we become vulnerable
	if(!activeCannons)
	{
		invincible = false;
		collidable = true;
	}

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
					if(currentAnimation->reverse)
					{
						(*aCanon)->copperAngle = (*aCanon)->copperAngle + (3.6 * (PI/180));
					}
					else
					{
						(*aCanon)->copperAngle = (*aCanon)->copperAngle - (3.6 * (PI/180));
					}
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
	/*if(currentLifeTime >= lifeTime)
	{
	//	lifePhase = COPPER_LEAVING;
	}*/
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
				//we want it at least two width away from our current position
				do{
					yDestination = GAMEZONE_LIMIT + width/2 + rand() % (GAMEZONE_HEIGHT - (int)width);
				} while(fabs(posY - yDestination) < width*2);

				moves = computeInertialLinearMove(posX, posY, posX, yDestination, 3);
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
			//Which direction ?
			currentAnimation->reverse = rand() % 2;
			spinningCounter = 0;
			break;

		default:
			break;
	}

}

void Copper::move()
{
	posY = moves.back().second;
	moves.pop_back();

	if(moves.empty())
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

		if(!invincible)
		{
			int lifeValue = life;
			life = max(0, life - aLaser->power);
			if(aLaser->isPhoton())
			{
				HadronParticle * aPhoton =  dynamic_cast<HadronParticle *>(aDrawable);
				aPhoton->removeEnergy(lifeValue - life);
			}
			if(life<=0)
			{
				CurrentLevel->soundEngine->playSound("Explode2", posX);
				CurrentLevel->createEffect(posX, posY, "explosionCopper");
				for(int i = 0; i < 4; i++)
				{
					float x = posX - width/2 + rand() % (int)width;
					float y = posY - height/2 + rand() % (int)height;
					CurrentLevel->createExplosion(x, y);
				}
				ge->startShaking(20, true);
				Score = Score + scoreValue * (type + 1);

				for(vector<CopperCannon *>::iterator aCanon = cannons.begin(); aCanon != cannons.end(); ++aCanon)
				{
					(*aCanon)->toRemove = true;
				}
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

void Copper::addSubpart(EnemyWave * aWave)
{
	for(vector<CopperCannon *>::iterator aCanon = cannons.begin(); aCanon != cannons.end(); ++aCanon)
	{
		aWave->enemies.push_back(*aCanon);
	}
}

/*
 * CopperCanon functions
 */
CopperCannon::CopperCannon(Copper * aCopper, float x, float y)
{
	copyFrom(CurrentLevel->loadedObjects.at("e005CanonSp"));

	shootingAngle = startShootingAngle = 0;
	copper = aCopper;
	posX = copper->posX + x;
	posY = copper->posY + y;
	float xDiff = copper->posX - posX;
	float yDiff = copper->posY - posY;

	copperAngle = atan2(yDiff, xDiff);
	posX = copper->posX + 36 * cos(copperAngle);
	posY = copper->posY - 36 * sin(copperAngle);

	destroyed = false;
	life = 750;

	activated = false;

	int j = 35;
	for(int i = 0; i < 360; i++)
	{
		cannonAngles.push_back(j);

		if(i % 5 == 0)
		{
			j = (j + 1) % 72;
		}
	}
}

void CopperCannon::animate()
{
	posX = copper->posX + 36 * cos(copperAngle);
	posY = copper->posY - 36 * sin(copperAngle);

	if(updateAnimationFrame() && !destroyed)
	{
		//Cannot perform modulo on negative number
		//Thus do +355 instead of -5
		shootingAngle = (shootingAngle + 355) % 360;

		if(activated)
		{
			fire(shootingAngle);
			if(shootingAngle == startShootingAngle)
			{
				activated = false;
			}
		}
		else
		{
			float xDiff = CurrentLevel->hero->posX - posX;
			float yDiff = CurrentLevel->hero->posY - posY;
			int angle = ((int)(atan2(yDiff, xDiff) * (180/PI) + 360)) % 360;
			currentAnimation->setFrameTo(cannonAngles.at(angle));

			if(rand() % 60 < 1)
				fire(angle);
		}
	}

	processDisplay();
}

void CopperCannon::fire(int anAngle)
{
	CurrentLevel->activeElements.push_back(new CopperAmmo(posX, posY, anAngle * (PI/180), 4));
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
				HadronParticle * aPhoton =  dynamic_cast<HadronParticle *>(aDrawable);
				aPhoton->removeEnergy(lifeValue - life);
			}
			if (life<=0)
			{
				CurrentLevel->soundEngine->playSound("Explode1", posX);
				CurrentLevel->createExplosion(posX, posY);
				Score = Score + scoreValue * (type + 1);
				destroyed = true;
				activated = false;
				collidable = false;
				copper->activeCannons--;
				display = false;
			}
			else
			{
				startBlinkingWhite(4);
			}
		}
	}
}
