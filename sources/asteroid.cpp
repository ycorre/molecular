#include "asteroid.h"

PyroxeneField::PyroxeneField()
{
	generationRate = 250;
	lastGeneration = GameTimer;
	numberOfPyroxenes = 10;
	generatedPyroxenes = 0;
	display = false;
}

PyroxeneField::PyroxeneField(Json::Value aConfig)
{
	generationRate = aConfig.get("generationRate", 250).asInt();
	numberOfPyroxenes = aConfig.get("numberOfPyroxenes", 25).asInt();

	generatedPyroxenes = 0;
	lastGeneration = GameTimer;
	display = false;
}


void PyroxeneField::animate()
{
	if(generatePyroxene())
	{
		pyroxenes.push_back(new Pyroxene(BIG_PYROXENE));
		generatedPyroxenes++;
	}

	CurrentLevel->checkEnemyCollision(pyroxenes);

	for(list<Enemy *>::iterator aPyroxene = pyroxenes.begin(); aPyroxene != pyroxenes.end();++aPyroxene)
	{
		if((*aPyroxene)->toRemove)
		{
			delete (*aPyroxene);
			aPyroxene =	pyroxenes.erase(aPyroxene);
		}
		else
		{
			(*aPyroxene)->animate();
			(*aPyroxene)->processDisplay();
		}
	}

	if(generatedPyroxenes == numberOfPyroxenes && pyroxenes.empty())
		toRemove = true;
}

bool PyroxeneField::generatePyroxene()
{
	unsigned int nextGenerationTime = lastGeneration + generationRate;
	if (generatedPyroxenes < numberOfPyroxenes && nextGenerationTime < GameTimer)
	{
		lastGeneration = GameTimer;
		return true;
	}

	return false;
}

/*
 * Pyroxene functions
 */

Pyroxene::Pyroxene()
{
	speed = 0;
	angle = 0;
	scoreValue = 500;
	life = 250;
	bonusProbability = 10;
}

//Pyroxene appears randomly and have random trajectory
//So we get two random points (four values in total) which set the depart point and the destination point of the pyroxene
Pyroxene::Pyroxene(int asteroidType)
{
	if (asteroidType == BIG_PYROXENE)
	{
		copyFrom(CurrentLevel->loadedObjects.at("bigPyroxene"));
		life = 250;
	}
	if (asteroidType == SMALL_PYROXENE)
	{
		copyFrom(CurrentLevel->loadedObjects.at("smallPyroxene"));
		life = 100 ;
	}
	setAngleAndSpeed();
	type = asteroidType;
	bonusProbability = 25;
	scoreValue = 500 - (200 * asteroidType);

	int temp = 1 +  rand() % 4;
	stringstream tmpStream;
	tmpStream << "type" << temp;
	setAnimation(tmpStream.str());

	setAnimX(0);
	setAnimY(0);
}

//Pyroxenes have a predefined trajectory
//typically used for smaller pyroxenes resulting of the explosion of bigger pyroxenes
Pyroxene::Pyroxene(int asteroidType, int sX, int sY, int aSpeed, float anAngle)
{
	if (asteroidType == BIG_PYROXENE)
	{
		copyFrom(CurrentLevel->loadedObjects.at("bigPyroxene"));
		life = 250;

	}
	if (asteroidType == SMALL_PYROXENE)
	{
		copyFrom(CurrentLevel->loadedObjects.at("smallPyroxene"));
		life = 100;
	}


	type = asteroidType;
	bonusProbability = 25;
	scoreValue = 500 - (200 * asteroidType);

	int temp = 1 +  rand() % 4;
	stringstream tmpStream;
	tmpStream << "type" << temp;
	setAnimation(tmpStream.str());

	setAnimX(0);
	setAnimY(0);
	speed = aSpeed;
	angle = anAngle;
	posX = sX;
	posY = sY;
}

//Randomly decide the characteristics of a pyroxene (speed and angle)
void Pyroxene::setAngleAndSpeed()
{
	//Set which side of the screen it will appear from

	int arrivalSide = (rand() % 3);
	int angleDegree;

	//Set the angle (in degrees)
	//We try too sharp angle
	if (arrivalSide == RIGHT)
	{
		posX = SCREEN_WIDTH + width/2;
		posY = width/2 + rand() % (GAMEZONE_HEIGHT - (int)width/2);

		angleDegree = (rand() % 90) + 135; // 45 and 315
	}
	if (arrivalSide == UP)
	{
		posY = -height/2;
		posX =  (rand() % (SCREEN_WIDTH - 100)) + 100;
		angleDegree = (rand() % 150) + 15; //values between 15 and 165
	}
	if (arrivalSide == DOWN || arrivalSide == LEFT) //Actually DOWN but since we don't want anything coming from the left
	{
		posY = GAMEZONE_HEIGHT + width/2;
		posX = (rand() % (SCREEN_WIDTH - 100)) + 100;
		angleDegree = (rand() % 150) + 195; // -165 and -15
	}

	//Convert into radians
	angle = angleDegree * (PI / 180.0);

	//Set the speed (between 1 and 3)
	speed = (rand() % 2) + 1;
}

void Pyroxene::fire()
{

}

void Pyroxene::animate()
{
	updateAnimationFrame();

	float vx, vy;
	vx = speed * cos(angle);
	vy = speed * sin(angle);
	posX = posX + vx;
	posY = posY + vy;

	if(!CurrentLevel->isOnScreen(this))
		toRemove = true;
}

void Pyroxene::processCollisionWith(Drawable* aDrawable)
{
	if(aDrawable->isHero())
	{
		if (type == BIG_PYROXENE)
		{
			CurrentLevel->soundEngine->playSound("xwing_explode");
			CurrentLevel->createExplosion(posX, posY);
		}
		else if(type == SMALL_PYROXENE)
		{
			CurrentLevel->soundEngine->playSound("xwing_explode");
			CurrentLevel->createExplosion(posX, posY);
		}

		dropBonus(posX, posY);
		toRemove = true;
		Score = Score + scoreValue;
		return;
	}

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
			if (type == BIG_PYROXENE)
			{
				CurrentLevel->soundEngine->playSound("xwing_explode");
				CurrentLevel->createExplosion(posX, posY);
			}
			else if(type == SMALL_PYROXENE)
			{
				CurrentLevel->soundEngine->playSound("xwing_explode");
				CurrentLevel->createExplosion(posX, posY);
			}
			Score = Score + scoreValue;
			//The pyroxene is not the smallest type
			if(type != SMALL_PYROXENE)
				{createSmallerPyroxene(this);}

			dropBonus(posX, posY);
			toRemove = true;
		}
		else
		{
			startBlinkingWhite(4);
		}
	}
	return;
}

//When a big pyroxene explodes, it creates two smaller pyroxenes that go in perpendicular directions
void Pyroxene::createSmallerPyroxene(Pyroxene * anAsteroid)
{
	CurrentLevel->activeElements.push_back(new Pyroxene(anAsteroid->type + 1, anAsteroid->posX, anAsteroid->posY, anAsteroid->speed + 1, anAsteroid->angle + (90.0 * (PI / 180.0))));
	CurrentLevel->activeElements.push_back(new Pyroxene(anAsteroid->type + 1, anAsteroid->posX, anAsteroid->posY, anAsteroid->speed + 1, anAsteroid->angle - (90.0 * (PI / 180.0))));
}
