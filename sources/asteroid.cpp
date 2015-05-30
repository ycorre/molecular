#include "include/asteroid.h"

PyroxeneField::PyroxeneField()
{
	generationRate = 250;
	lastGeneration = GameTimer;
	numberOfPyroxenes = 10;
	generatedPyroxenes = 0;
	display = false;
	enemyWave = NULL;
	posX = posY = -500;
}

PyroxeneField::PyroxeneField(Json::Value aConfig, EnemyWave * anEnemyWave)
{
	generationRate = aConfig.get("generationRate", 250).asInt();
	numberOfPyroxenes = aConfig.get("numberOfPyroxenes", 25).asInt();

	generatedPyroxenes = 0;
	lastGeneration = GameTimer;
	display = false;
	enemyWave = anEnemyWave;
	posX = posY = -500;
}


void PyroxeneField::animate()
{
	if(generatePyroxene())
	{
		enemyWave->enemies.push_back(new Pyroxene(BIG_ROCK));
		generatedPyroxenes++;
	}

	if(generatedPyroxenes == numberOfPyroxenes && enemyWave->enemies.size() == 1)
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
 * Rock functions
 */
Rock::Rock()
{
	speed = 0;
	angle = 0;
	scoreValue = 500;
	life = 250;
	bonusProbability = 10;
}

//Rock appears randomly and have random trajectory
//So we get two random points (four values in total) which set the starting point and the destination point of the rock
Rock::Rock(int rockType)
{
	if (rockType == BIG_ROCK)
	{
		copyFrom(CurrentLevel->loadedObjects.at("bigRock"));
		life = 250;
	}
	if (rockType == SMALL_ROCK)
	{
		copyFrom(CurrentLevel->loadedObjects.at("smallRock"));
		life = 100 ;
	}
	setAngleAndSpeed();
	type = rockType;
	bonusProbability = 25;
	scoreValue = 500 - (200 * rockType);

	int temp = 1 +  rand() % 2;
	stringstream tmpStream;
	tmpStream << "type" << temp;
	setAnimation(tmpStream.str());

	currentAnimation->reverse = rand() % 2;
}

//Rocks have a predefined trajectory
//typically used for smaller rocks resulting of the explosion of bigger rocks
Rock::Rock(int rockType, int sX, int sY, int aSpeed, float anAngle)
{
	if (rockType == BIG_ROCK)
	{
		copyFrom(CurrentLevel->loadedObjects.at("bigRock"));
		life = 250;
	}
	if (rockType == SMALL_ROCK)
	{
		copyFrom(CurrentLevel->loadedObjects.at("smallRock"));
		life = 100;
	}

	type = rockType;
	bonusProbability = 25;
	scoreValue = 500 - (200 * rockType);

	int temp = 1 +  rand() % 2;
	stringstream tmpStream;
	tmpStream << "type" << temp;
	setAnimation(tmpStream.str());

	currentAnimation->reverse = rand() % 2;

	speed = aSpeed;
	angle = anAngle;
	posX = sX;
	posY = sY;
}

//Randomly decide the characteristics of a rock (speed and angle)
void Rock::setAngleAndSpeed()
{
	//Set which side of the screen it will appear from
	int arrivalSide = (rand() % 3);
	int destPoint;

	//Set the angle
	//We make sure the rocks exit through the opposite side from the one it appeared
	if (arrivalSide == RIGHT)
	{
		posX = SCREEN_WIDTH + width/2;
		posY = GAMEZONE_LIMIT + height/2 + rand() % (GAMEZONE_HEIGHT - (int)height/2);

		destPoint = GAMEZONE_LIMIT + height/2 + rand() % (GAMEZONE_HEIGHT - (int)height/2);
		float yDiff = destPoint - posY;
		float xDiff = width/2 - posX;
		angle = atan2(yDiff, xDiff);
	}
	if (arrivalSide == UP)
	{
		posY = SCREEN_HEIGHT + height/2;
		posX =  (rand() % (SCREEN_WIDTH - 100)) + 100;

		destPoint = (rand() % (SCREEN_WIDTH - 100)) + 100;

		float xDiff = destPoint - posX;
		float yDiff = GAMEZONE_LIMIT - height/2 - posY;
		angle = atan2(yDiff, xDiff);
	}
	if (arrivalSide == DOWN || arrivalSide == LEFT) //Actually DOWN but since we don't want anything coming from the left
	{
		posY = GAMEZONE_LIMIT - height/2;
		posX = (rand() % (SCREEN_WIDTH - 100)) + 100;

		destPoint = (rand() % (SCREEN_WIDTH - 100)) + 100;

		float xDiff = destPoint - posX;
		float yDiff = SCREEN_HEIGHT + height/2 - posY;
		angle = atan2(yDiff, xDiff);
	}

	//Set the speed (between 2 and 4)
	speed = (rand() % 2) + 2;
}

void Rock::animate()
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

void Rock::processCollisionWith(Drawable* aDrawable)
{
	if(aDrawable->isHero())
	{
		if(type == SMALL_ROCK)
		{
			CurrentLevel->soundEngine->playSound("explRocSmall", posX);
		}
		else
		{
			CurrentLevel->soundEngine->playSound("explRocBig", posX);
		}
		CurrentLevel->createExplosion(posX, posY);
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
			HadronParticle * aPhoton =  dynamic_cast<HadronParticle *>(aDrawable);
			aPhoton->removeEnergy(lifeValue - life);
		}
		if (life<=0)
		{
			if(type == SMALL_ROCK)
			{
				CurrentLevel->soundEngine->playSound("explRocSmall", posX);
			}
			else
			{
				CurrentLevel->soundEngine->playSound("explRocBig", posX);
			}

			CurrentLevel->createExplosion(posX, posY);
			Score = Score + scoreValue;
			//The rock is not the smallest type
			if(type != SMALL_ROCK)
				{this->createSmallerRock(this);}

			dropBonus(posX, posY);
			toRemove = true;
		}
		else
		{
			startBlinkingWhite(4);
		}
	}
}

//When a big rock explodes, it creates two smaller rocks that go in perpendicular directions
void Rock::createSmallerRock(Rock * aRock)
{
	CurrentLevel->activeElements.push_back(new Rock(type + 1, posX, posY, speed + 1, angle + (PI / 2)));
	CurrentLevel->activeElements.push_back(new Rock(type + 1, posX, posY, speed + 1, angle - (PI / 2)));
}

/*
 * Pyroxene functions
 */
Pyroxene::Pyroxene(int rockType)
{
	if (rockType == BIG_ROCK)
	{
		copyFrom(CurrentLevel->loadedObjects.at("bigPyroxene"));
		life = 250;
	}
	if (rockType == SMALL_ROCK)
	{
		copyFrom(CurrentLevel->loadedObjects.at("smallPyroxene"));
		life = 100 ;
	}
	setAngleAndSpeed();
	type = rockType;
	bonusProbability = 25;
	scoreValue = 500 - (200 * rockType);

	int temp = 1 +  rand() % 2;
	stringstream tmpStream;
	tmpStream << "type" << temp;
	setAnimation(tmpStream.str());

	currentAnimation->reverse = rand() % 2;
}

//Rocks have a predefined trajectory
//typically used for smaller rocks resulting of the explosion of bigger rocks
Pyroxene::Pyroxene(int rockType, int sX, int sY, int aSpeed, float anAngle) {
	if (rockType == BIG_ROCK) {
		copyFrom(CurrentLevel->loadedObjects.at("bigPyroxene"));
		life = 250;
	}
	if (rockType == SMALL_ROCK) {
		copyFrom(CurrentLevel->loadedObjects.at("smallPyroxene"));
		life = 100;
	}

	type = rockType;
	bonusProbability = 25;
	scoreValue = 500 - (200 * rockType);

	int temp = 1 +  rand() % 2;
	stringstream tmpStream;
	tmpStream << "type" << temp;
	setAnimation(tmpStream.str());

	currentAnimation->reverse = rand() % 2;

	speed = aSpeed;
	angle = anAngle;
	posX = sX;
	posY = sY;
}

//When a big rock explodes, it creates two smaller rocks that go in perpendicular directions
void Pyroxene::createSmallerRock(Rock * aRock) {
	CurrentLevel->activeElements.push_back(
			new Pyroxene(type + 1, posX, posY, speed + 1, angle + PI / 2));
	CurrentLevel->activeElements.push_back(
			new Pyroxene(type + 1, posX, posY, speed + 1, angle - PI / 2));
}

