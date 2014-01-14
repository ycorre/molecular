#include "asteroid.h"

Asteroid::Asteroid()
{
	speed = 0;
	angle = 0;
	scoreValue = 500;
	life = 250;
	bonusProbability = 10;
}

//Asteroids appears randomly and have random trajectory
//So we get two random points (four values in total) which set the depart point and the destination point of the asteroid
Asteroid::Asteroid(int asteroidType)
{
	if (asteroidType == BIG_ASTEROID)
	{
		copyFrom(lev->loadedObjects.at("bigAsteroid"));
		life = 250;
	}
	if (asteroidType == SMALL_ASTEROID)
	{
		copyFrom(lev->loadedObjects.at("smallAsteroid"));
		life = 100 ;
	}
	setAngleAndSpeed();
	type = asteroidType;
	bonusProbability = 25;
	scoreValue = 500 - (200 * asteroidType);

	int temp = rand() % 2;
	if(temp == 0)
	{
		setAnimation("type1");
	}
	else
	{
		setAnimation("type2");
	}
	setAnimX(0);
	setAnimY(0);
}

//Asteroids have a predefined trajectory
//typically used for smaller asteroids resulting of the explosion of bigger asteroids
Asteroid::Asteroid(int asteroidType, int sX, int sY, int aSpeed, float anAngle)
{
	if (asteroidType == BIG_ASTEROID)
	{
		copyFrom(lev->loadedObjects.at("bigAsteroid"));
		life = 250;

	}
	if (asteroidType == SMALL_ASTEROID)
	{
		copyFrom(lev->loadedObjects.at("smallAsteroid"));
		life = 100;
	}


	type = asteroidType;
	bonusProbability = 25;
	scoreValue = 500 - (200 * asteroidType);

	int temp = rand() % 2;
	if(temp == 0)
	{
		setAnimation("type1");
	}
	else
	{
		setAnimation("type2");
	}
	setAnimX(0);
	setAnimY(0);
	speed = aSpeed;
	angle = anAngle;
	posX = sX + width / 2;
	posY = sY + height / 2;
}

//Randomily decide the characteristics of an asteroid (speed and angle)
void Asteroid::setAngleAndSpeed()
{
	//Set which side of the screen it will appear from

	int arrivalSide = (rand() % 3);
	int angleDegree;

	//Set the angle (in degrees)
	//We try too sharp angle
	if (arrivalSide == RIGHT)
	{
		posX = SCREEN_WIDTH;
		posY = rand() % GAMEZONE_HEIGHT;

		angleDegree = (rand() % 90) + 135; // 45 and 315
	}
	if (arrivalSide == UP)
	{
		posY = -height;
		posX =  (rand() % (SCREEN_WIDTH - 100)) + 100;
		angleDegree = (rand() % 150) + 15; //values between 15 and 165
	}
	if (arrivalSide == DOWN || arrivalSide == LEFT) //Actually DOWN but since we don't want anything coming from the left
	{
		posY = GAMEZONE_HEIGHT;
		posX = (rand() % (SCREEN_WIDTH - 100)) + 100;
		angleDegree = (rand() % 150) + 195; // -165 and -15
	}

	//Convert into radians
	angle = angleDegree * (PI / 180.0);

	//Set the speed (between 1 and 3)
	speed = (rand() % 2) + 1;
}

void Asteroid::fire()
{

}

void Asteroid::animate()
{
	updateAnimationFrame();

	float vx, vy;
	vx = speed * cos(angle);
	vy = speed * sin(angle);
	posX = posX + vx;
	posY = posY + vy;

	if(!lev->isOnScreen(this))
		toRemove = true;
}

void Asteroid::processCollisionWith(Drawable* aDrawable)
{
	if(aDrawable->isHero())
	{
		if (type == BIG_ASTEROID)
		{
			lev->soundEngine->playSound("xwing_explode");
			lev->createExplosion(posX + width/2, posY + height/2);
		}
		else if(type == SMALL_ASTEROID)
		{
			lev->soundEngine->playSound("xwing_explode");
			lev->createExplosion(posX + width/2, posY + height/2);
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
			if (type == BIG_ASTEROID)
			{
				lev->soundEngine->playSound("xwing_explode");
				lev->createExplosion(posX + width/2, posY + height/2);
			}
			else if(type == SMALL_ASTEROID)
			{
				lev->soundEngine->playSound("xwing_explode");
				lev->createExplosion(posX + width/2, posY + height/2);
			}
			Score = Score + scoreValue;
			//The asteroid is not the smallest type
			if(type != SMALL_ASTEROID)
				{createSmallerAsteroid(this);}

			dropBonus(posX, posY);
			toRemove = true;
		}
	}
	return;
}

//When a big asteroid explodes create two smaller asteroids that goes in perpendicular directions
void Asteroid::createSmallerAsteroid(Asteroid * anAsteroid)
{
	lev->activeElements.push_back(new Asteroid(anAsteroid->type + 1, anAsteroid->posX, anAsteroid->posY, anAsteroid->speed + 1, anAsteroid->angle + (90.0 * (PI / 180.0))));
	lev->activeElements.push_back(new Asteroid(anAsteroid->type + 1, anAsteroid->posX, anAsteroid->posY, anAsteroid->speed + 1, anAsteroid->angle - (90.0 * (PI / 180.0))));
}
