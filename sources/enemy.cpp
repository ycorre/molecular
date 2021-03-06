#include "include/enemy.h"

Enemy::Enemy()
{
	name = "Enemy";
	posX = 0;
	posY = 0;
	setAnimX(0);
	setAnimY(0);
	scoreValue = 0;
	type = 0;
	bonusProbability = 90;
	speed = 2;
	originY = posY;
	sinusWidth = 400;
	sinusHeigth = 80;
	life = 50;
	canFire = false;
	lastTimeFired = 0;
	minFireRate = 2000;
	maxFireRate = 1000;
	fireRate = minFireRate + (rand() % maxFireRate);
	currentPosition = 0;
	damage = 1000;
	angleModifyingFactor = 0;

	speed = 2;
	angle = 180.0 * (PI/180);
	nextAngle = angle;
}

Enemy::Enemy(int x, int y, int typeXW)
{
	name = "Enemy";
	width = atoi(((CurrentLevel->configurationElements.at("enemy")).at(0)).c_str());
	height = atoi(((CurrentLevel->configurationElements.at("enemy")).at(1)).c_str());
	posX = x;
	posY = y;
	setAnimX(0);
	setAnimY(typeXW * height);
	type = typeXW;
	scoreValue = 200;
	bonusProbability = 50;
	canFire = false;
	minFireRate = 2000;
	maxFireRate = 1000;
	fireRate = minFireRate + (rand() % maxFireRate);
	lastTimeFired = 0;
	life = 50 * (typeXW + 1);
	speed = 2;
	angle = 180.0 * (PI/180);
	nextAngle = angle;
	currentPosition = 0;
	damage = 1000;
	angleModifyingFactor = 0;

	originY = y;
	sinusWidth = 400;
	sinusHeigth = 80;
}

Enemy::Enemy(int x, int y, float sinWidth, float sinHeigth, float aSpeed)
{
	copyFrom(CurrentLevel->loadedObjects.at("enemy"));

	posX = x;
	posY = y;
	setAnimX(0);
	setAnimY(0);
	type = 0;
	scoreValue = 200;
	bonusProbability = 50;
	canFire = false;
	minFireRate = 2000;
	maxFireRate = 1000;
	fireRate = minFireRate + (rand() % maxFireRate);
	lastTimeFired = 0;
	life = 50;
	speed = aSpeed;
	angle = 180.0 * (PI/180);
	nextAngle = angle;
	currentPosition = 0;
	damage = 1000;
	angleModifyingFactor = 0;

	originY = y;
	sinusWidth = sinWidth;
	sinusHeigth = sinHeigth;
}

void Enemy::animate()
{
	updateAnimationFrame();
	checkPositions();

	//posX = posX - speed;

	float vx, vy;
	vx = speed * cos(angle);
	vy = speed * sin(angle);

	posX = posX + vx;
	posY = posY + vy;

/*
	//Normalize posX on [0, 2PI]
	//The width of the sinusoid is given by sinusWidth,
	float vx = ((int)posX % (int)sinusWidth)*((2*PI)/sinusWidth);

	//Compute the movement on the Y axis
	sfloat vy = sin(vx);
	posY = originY + vy * sinusHeigth;
	*/
}

void Enemy::checkPositions()
{
	//Check if we are close to the next position
	if (sqrt(((posX - eventPosition.at(currentPosition).first)*(posX - eventPosition.at(currentPosition).first) +
		(posY - eventPosition.at(currentPosition).second)*(posY - eventPosition.at(currentPosition).second))) < speed*speed)
	{
		//Move to the next position
		currentPosition++;

		//If we are at the last position given then delete the object
		if (currentPosition >= eventPosition.size())
		{
			toRemove = true;
			return;
		}

		//Compute the new angle
		float xDiff = eventPosition.at(currentPosition).first - posX;
		float yDiff = eventPosition.at(currentPosition).second - posY;
		//We want curvy turns so we slowly change the angle
		nextAngle = fmodf(atan2(yDiff, xDiff) + 2*PI, 2*PI);
		angleModifyingFactor = (nextAngle - angle) / speed;
	}
}

void Enemy::processCollisionWith(Drawable * aDrawable)
{
	if(aDrawable->isHero())
	{
		die();
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
			die();
		}
		else
		{
			startBlinkingWhite(4);
		}
		return;
	}
}

void Enemy::dropBonus(int x, int y)
{
	int aNumber = rand() % 100;
	if (aNumber < bonusProbability)
	{
		bonusType aBonus = (bonusType) (rand () % BONUS_COUNT);
		CurrentLevel->createBonus(x, y, aBonus);
	}
}

void Enemy::die()
{
	CurrentLevel->soundEngine->playSound("Explode1", posX);
	CurrentLevel->createExplosion(posX, posY);
	dropBonus(posX, posY);
	Score = Score + scoreValue * (type + 1);
	toRemove = true;
}

void Enemy::fire()
{
	checkFire();
	if (canFire)
	{
		CurrentLevel->soundEngine->playSound("enemyGun");

		//Shoot toward the hero
		//Compute the angle
		float xDiff = (CurrentLevel->hero->posX + CurrentLevel->hero->width/2) - (posX + width/2);
		float yDiff = (CurrentLevel->hero->posY + CurrentLevel->hero->height/2) - (posY + height/2);
		float angle = atan2(yDiff, xDiff);

		CurrentLevel->activeElements.push_back(new Bullet(posX + 30, posY + 30, angle, 3));
		canFire = false;
	}
}

void Enemy::checkFire()
{
	if (CurrentLevel->isOnScreen(this))
	{
		unsigned int nextFireTime = lastTimeFired + fireRate;
		if (nextFireTime < GameTimer)
		{
			canFire = true;
			lastTimeFired = GameTimer;
			fireRate = minFireRate + (rand() % maxFireRate);
		}
	}
}

void Enemy::addSubpart(EnemyWave * aWave)
{

}

/*
 * Cadmium functions
 */
Cadmium::Cadmium(int x, int y, float aSpeed, vector<int> moves)
{
	copyFrom(CurrentLevel->loadedObjects.at("eSp_Cadmium"));

	posX = x;
	posY = y;
	setAnimX(0);
	setAnimY(0);
	scoreValue = 200;
	bonusProbability = 80;
	canFire = false;
	fireRate = 450;
	lastTimeFired = 0;
	life = 50;
	speed = aSpeed;
	originY = y;
	angle = 180.0 * (PI/180);
	shootingAngle = 0;
	//currentPosition = 0;

	if(!moves.empty())
	{
		unsigned int i;
		for(i=0; i<moves.size(); i = i + 2)
		{
			eventPosition.push_back(make_pair(moves.at(i), moves.at(i+1)));
		}
	}

	posRafale = 0;
	rafaleRate = 4000;
}

Cadmium::Cadmium(Json::Value aConfig)
{
	copyFrom(CurrentLevel->loadedObjects.at("eSp_Cadmium"));

	posX = aConfig.get("posX", 0.0).asFloat();
	posX = posX + SCREEN_WIDTH;
	posY = aConfig.get("posY", 100.0).asFloat();
	scoreValue = aConfig.get("scoreValue", 200).asInt();
	bonusProbability = aConfig.get("bonusProbability", 0).asInt();
	fireRate = aConfig.get("fireRate", 350).asInt();
	life = aConfig.get("life", 50).asInt();
	speed = aConfig.get("speed", 4.0).asFloat();
	rafaleRate = aConfig.get("rafaleRate", 4000).asInt();

	setAnimX(0);
	setAnimY(0);

	unsigned int i;
	const Json::Value movements = aConfig["movement"];
	vector<int> moves;
	for (i = 0; i < movements.size(); i++)
	{
		moves.push_back(movements[i].asInt());
	}

	if(!moves.empty())
	{
		unsigned int i;
		for(i=0; i<moves.size(); i = i + 2)
		{
			eventPosition.push_back(make_pair(moves.at(i), moves.at(i+1)));
		}
	}

	canFire = false;
	lastTimeFired = GameTimer;
	angle = 180.0 * (PI/180);
	nextAngle = angle;
	shootingAngle = 0;
	posRafale = 0;
	currentAnimation->currentFrame = rand() % currentAnimation->numberOfFrames;
}

void Cadmium::checkFire()
{
	if (CurrentLevel->isOnScreen(this))
	{
		unsigned int nextFireTime;
		if (posRafale == 0)
		{
			nextFireTime = lastTimeFired + rafaleRate;
		}
		else
		{
			nextFireTime = lastTimeFired + fireRate;
		}

		if (posRafale == 0)
		{
			//Shoot toward the hero
			//Compute the angle
			float xDiff = CurrentLevel->hero->posX - posX;
			float yDiff = CurrentLevel->hero->posY - posY;
			shootingAngle = atan2(yDiff, xDiff);
		}

		if (nextFireTime < GameTimer)
		{
			canFire = true;
			posRafale = (posRafale + 1) % 3;
			lastTimeFired = GameTimer;
		}
	}
	else
	{
		lastTimeFired = GameTimer;
	}
}

void Cadmium::fire()
{
	checkFire();
	if (canFire && currentAnimation->currentFrame == 10)
	{
		CurrentLevel->soundEngine->playSound("enemyGun");

		CurrentLevel->activeElements.push_back(new CadmiumAmmo(posX, posY, shootingAngle, 4));
		canFire = false;
	}
}

//Make a movement engine that would compute all sort of trajectory (curve, line, other...)
//and return the series of corresponding points ?
void Cadmium::animate()
{
	updateAnimationFrame();
	checkPositions();
	fire();

	//If we are still turning
	if (fabs(angle - nextAngle) > fabs(angleModifyingFactor))
	{
		angle = fmodf(angle + angleModifyingFactor + 2.0*PI,  2.0f*PI);
	}
	else
	{
		//Make sure we have the right angle
		//otherwise we might miss the next checkpoint
		angle = nextAngle;
	}

	float vx, vy;
	vx = speed * cos(angle);
	vy = speed * sin(angle);

	posX = posX + vx;
	posY = posY + vy;
}

void Cadmium::die()
{
	CurrentLevel->soundEngine->playSound("Explode1", posX);
	CurrentLevel->createExplosion(posX, posY);
	CurrentLevel->createEffect(posX, posY, "cadmiumExplosion");
	dropBonus(posX, posY);
	Score = Score + scoreValue;
	toRemove = true;
}

/*
 * Iron functions
 */
Iron::Iron(Json::Value aConfig)
{
	copyFrom(CurrentLevel->loadedObjects.at("eSp_Iron"));
	int maxX = aConfig.get("spreadWidth", 500).asInt();

	posX = aConfig.get("posX", -1.0).asFloat();
	if(posX == -1.0)
		posX = rand() % maxX;
	posX = posX + SCREEN_WIDTH;

	posY = aConfig.get("posY", -1.0).asFloat();
	if(posY == -1)
		posY = GAMEZONE_LIMIT + width/2 + rand() % (GAMEZONE_HEIGHT - (int)width);

	scoreValue = aConfig.get("scoreValue", 100).asInt();
	bonusProbability = aConfig.get("bonusProbability", 100).asInt();
	fireRate = aConfig.get("fireRate", 450).asInt();
	life = aConfig.get("life", 750).asInt();
	speed = aConfig.get("speed", 1.5).asFloat();
	minNextShift = 1000;
	maxNextShift = 1500;
	lastShift = GameTimer;
	nextDirectionShift = minNextShift + rand() % maxNextShift;

	const Json::Value bonusReleased = aConfig["possibleBonus"];
	for (unsigned int j = 0; j < bonusReleased.size(); j++)
	{
		possibleBonus.push_back((bonusType)(bonusReleased[j].asInt()));
	}

	setAnimX(0);
	setAnimY(0);

	canFire = false;
	angle = 180.0 * (PI/180);
}

void Iron::animate()
{
	updateAnimationFrame();

	/*if (lev->isOnScreen(this))
	{
		unsigned int nextShift = lastShift + nextDirectionShift;
		if (nextShift < GameTimer)
		{
			angle = (90.0 + (float) (rand() % 180)) * (PI/180.0);
			lastShift = GameTimer;
			nextDirectionShift = minNextShift + (rand() % maxNextShift);
		}
	}*/

	float vx, vy;
	vx = speed * cos(angle);
	vy = speed * sin(angle);

	posX = posX + vx;
	posY = posY + vy;
}

void Iron::dropBonus(int x, int y)
{
	int i;
	int numberOfBonus = 2 + rand() % 3;

	for(i = 0; i < numberOfBonus; i++)
	{
		float aSpeed = 5.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/10.0f));
		float anAngle = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/360.0f)) * (PI/180);
		int aBonusType = rand() % possibleBonus.size();
		CurrentLevel->createBonus(posX, posY, aSpeed, anAngle, possibleBonus.at(aBonusType));
	}
}

/*
 * Silicon Functions
 */
SiliconField::SiliconField(Json::Value aConfig, EnemyWave * aWave)
{
	enemyWave = aWave;
	int generatedNumber = aConfig.get("generatedNumber", 50).asInt();
	int maxX = aConfig.get("spreadWidth", 500).asInt();
	int minValue, i;

	vector<float> xPositions = getNormalDistributionNumbers(generatedNumber, maxX/2, maxX/4, &minValue);

	//We want the closest one to be at most zero + 50
	if (minValue < 0)
	{
		float correction = -minValue + 50;
		for(i = 0; i < xPositions.size(); i++)
		{
			xPositions.at(i) = xPositions.at(i) + correction ;
		}
	}

	for(i = 0; i < generatedNumber; i++)
	{
		aWave->enemies.push_back(new Silicon(aConfig, xPositions.at(i)));
	}

	toRemove = true;
}

Silicon::Silicon(Json::Value aConfig, float aPosX)
{
	copyFrom(CurrentLevel->loadedObjects.at("e004Sp"));
	int maxX = aConfig.get("spreadWidth", 500).asInt();

	posX = aConfig.get("posX", -1.0).asFloat();
	posX = aPosX;
	//If no position X was given then pick one randomly
	if(posX == -1.0)
		posX = rand() % maxX;

	posX = posX + SCREEN_WIDTH;

	posY = aConfig.get("posY", -1).asFloat();
	if(posY == -1)
		posY = GAMEZONE_LIMIT + width/2 + rand() % (GAMEZONE_HEIGHT - (int)width);

	scoreValue = aConfig.get("scoreValue", 10).asInt();
	bonusProbability = aConfig.get("bonusProbability", 0).asInt();
	life = aConfig.get("life", 20).asInt();
	//speed = aConfig.get("speed", 2.0).asFloat();
	speed = 1.5f +  (static_cast <float> (rand()) / static_cast <float> (RAND_MAX/1.0f));

	damage = aConfig.get("damage", 1).asInt();

	currentAnimation->currentFrame = rand() % currentAnimation->numberOfFrames;

	amplitude = 160;

	canFire = false;
	angle = PI;
	destY = -1;
	destX = -1;
	ySpeed = xSpeed = 0;
	moving = false;
}

void Silicon::animate()
{
	updateAnimationFrame();

	posX = posX - speed;

	if(!moving && CurrentLevel->isOnScreen(this))
	{
		//Should we make an expected move ?
	//	int move = rand() % 200;
		//if(move <= 1)
		if(!moving)
		{
			moving = true;

			//Which length ?
			destY = 10 + rand() % 30;

			//Up or down ?
			if(rand() % 2)
			{
				destY = posY - destY;
				ySpeed = -0.3;
			}
			else
			{
				destY = destY + posY;
				ySpeed = 0.3;
			}

			//Don't get out of the screen
			if(destY <= GAMEZONE_LIMIT + width/2 || destY >= SCREEN_HEIGHT - width/2)
				moving = false;

			if(rand() % 2)
			{
				destX = posX - destY;
				xSpeed = 0.2;
			}
		}
	}
	else
	{
		posY = posY + ySpeed;
		posX = posX - xSpeed;

		if(destY != -1 && std::abs((int)(posY - destY)) <= 4)
		{
			destY = -1;
			moving = false;
		}

	/*	if(destX != -1 && std::abs((int)(posX - destX)) <= speed + xSpeed + 1)
		{
			destX = -1;
			moving = false;
		}*/
	}

	if(posX < -width)
		toRemove = true;
}

void Silicon::die()
{
	CurrentLevel->soundEngine->playSound("Explode1", posX);
	CurrentLevel->createEffect(posX, posY, "explosionSilicon");
	Score = Score + scoreValue;
	toRemove = true;
}

