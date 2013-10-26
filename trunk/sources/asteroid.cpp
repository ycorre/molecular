#include "enemy.h"

Asteroid::Asteroid()
{
	//	Enemy();
}

//Asteroids appears randomly and have random trajectory
//So we get two random points (four values in total) which set the depart point and the destination point of the asteroid
Asteroid::Asteroid(int asteroidType)
{
	Drawable();
	if (asteroidType == ASTER_NORMAL)
	{
		texture = ge->textures.at("asteroid");
		width = atoi(((lev->configurationElements.at("asteroid")).at(0)).c_str());
		height = atoi(((lev->configurationElements.at("asteroid")).at(1)).c_str());
		nbFrames = parseAnimationState((lev->configurationElements.at("asteroid")).at(2));
	}
	if (asteroidType == ASTER_DEMI)
	{
		texture = ge->textures.at("asteroid2");
		width = atoi(((lev->configurationElements.at("asteroid2")).at(0)).c_str());
		height = atoi(((lev->configurationElements.at("asteroid2")).at(1)).c_str());
		nbFrames = parseAnimationState((lev->configurationElements.at("asteroid2")).at(2));
	}
	if (asteroidType == ASTER_2THIRD)
	{
		texture = ge->textures.at("asteroid23");
		width = atoi(((lev->configurationElements.at("asteroid23")).at(0)).c_str());
		height = atoi(((lev->configurationElements.at("asteroid23")).at(1)).c_str());
		nbFrames = parseAnimationState((lev->configurationElements.at("asteroid23")).at(2));
	}
	if (asteroidType == ASTER_1THIRD)
	{
		texture = ge->textures.at("asteroid46");
		width = atoi(((lev->configurationElements.at("asteroid46")).at(0)).c_str());
		height = atoi(((lev->configurationElements.at("asteroid46")).at(1)).c_str());
		nbFrames = parseAnimationState((lev->configurationElements.at("asteroid46")).at(2));
	}

	state = 0;
	animX = 0;
	animY = 0;
	scoreValue = 500 - (200 * asteroidType);
	direction = UP;
	type = asteroidType;
	bonusProbability = 10;
	canFire = FALSE;
	fireRate = 2500;
	lastTimeFired = 0;
	life = 50;
	setAngleAndSpeed();
}

//Asteroids have a predefined trajectory
//typically used for smaller asteroids resulting of the explosion of bigger asteroids
Asteroid::Asteroid(int asteroidType, int sX, int sY, int aSpeed, float anAngle)
{
	Drawable();
	if (asteroidType == ASTER_NORMAL)
	{
		texture = ge->textures.at("asteroid");
		width = atoi(((lev->configurationElements.at("asteroid")).at(0)).c_str());
		height = atoi(((lev->configurationElements.at("asteroid")).at(1)).c_str());
		nbFrames = parseAnimationState((lev->configurationElements.at("asteroid")).at(2));
	}
	if (asteroidType == ASTER_DEMI)
	{
		texture = ge->textures.at("asteroid2");
		width = atoi(((lev->configurationElements.at("asteroid2")).at(0)).c_str());
		height = atoi(((lev->configurationElements.at("asteroid2")).at(1)).c_str());
		nbFrames = parseAnimationState((lev->configurationElements.at("asteroid2")).at(2));
	}
	if (asteroidType == ASTER_2THIRD)
	{
		texture = ge->textures.at("asteroid23");
		width = atoi(((lev->configurationElements.at("asteroid23")).at(0)).c_str());
		height = atoi(((lev->configurationElements.at("asteroid23")).at(1)).c_str());
		nbFrames = parseAnimationState((lev->configurationElements.at("asteroid23")).at(2));
	}
	if (asteroidType == ASTER_1THIRD)
	{
		texture = ge->textures.at("asteroid46");
		width = atoi(((lev->configurationElements.at("asteroid46")).at(0)).c_str());
		height = atoi(((lev->configurationElements.at("asteroid46")).at(1)).c_str());
		nbFrames = parseAnimationState((lev->configurationElements.at("asteroid46")).at(2));
	}

	state = 0;
	animX = 0;
	animY = 0;
	type = asteroidType;
	bonusProbability = 10;
	canFire = FALSE;
	scoreValue = 500 - (200 * asteroidType);
	fireRate = 2500;
	lastTimeFired = 0;
	life = 50;
	speed = aSpeed;
	angle = anAngle;
	posX = sX + width / 2;
	posY = sY + height / 2;
}

//Randomily decide the characteristics of an asteroid (speed and angle)
void Asteroid::setAngleAndSpeed()
{
	//Set which side of the screen it will appear from
	int arrivalSide = rand() % 4;
	int angleDegree;

	//Then set the starting point
	if(arrivalSide == LEFT)
	{
		posX = -width;
		posY = rand() % GAMEZONE_HEIGHT;
	}
	if (arrivalSide == RIGHT)
	{
		posX = SCREEN_WIDTH;
		posY = rand() % GAMEZONE_HEIGHT;
	}
	if (arrivalSide == UP)
	{
		posY = -height;
		posX = rand() % SCREEN_WIDTH;
	}
	if (arrivalSide == DOWN)
	{
		posY = GAMEZONE_HEIGHT;
		posX = rand() % SCREEN_WIDTH;
	}

	//Set the speed (between 2 and 7)
	speed = (rand() % 5) + 2;

	//Set the angle (in degrees)
	if (arrivalSide == LEFT)
	{
		angleDegree = (rand() % 90) + 315; // values between -45 and 45
	}
	if (arrivalSide == RIGHT)
	{
		angleDegree = (rand() % 90) + 135; // 45 and 315
	}
	if (arrivalSide == DOWN)
	{
		angleDegree = (rand() % 180) + 180; // -180 and 0
	}
	if (arrivalSide == UP)
	{
		angleDegree = rand() % 180; //values between 0 and 180
	}

	//Convert into radians
	angle = angleDegree * (PI / 180.0);
}


void Asteroid::animate()
{
	updateAnimationFrame();

	float vx, vy;
	vx = (speed) * cos(angle);
	vy = (speed) * sin(angle);
	posX = posX + vx;
	posY = posY + vy;
}


void Asteroid::processCollisionWith(Drawable* aDrawable)
{
	if(aDrawable->isHero())
	{
		lev->createExplosion(this->posX-this->width/2, this->posY - this->height/2, XWING_EXPL);
		dropBonus(this->posX, this->posY);
		this->toRemove = TRUE;
		Score = Score + scoreValue;
		return;
	}

	if (aDrawable->isLaser())
	{
		Laser * aLaser =  static_cast<Laser*>(aDrawable);
		life = life - aLaser->power;
		if (life<=0)
		{
			lev->createExplosion(this->posX - this->width/2, this->posY - this->height/2, XWING_EXPL);
			Score = Score + scoreValue;
			//The asteroid is not the smallest type
			if(type != ASTER_1THIRD)
				{createSmallerAsteroid(this);}

			dropBonus(this->posX, this->posY);
			this->toRemove = TRUE;
		}
		else
		{
			lev->createExplosion(aLaser->posX, aLaser->posY-this->height, SPARK);
		}
		return;
	}
	return;
}

//When a big asteroid explodes create two smaller asteroids that goes in perpendicular directions
void Asteroid::createSmallerAsteroid(Asteroid * anAsteroid)
{
	lev->activeElements.push_back(new Asteroid(anAsteroid->type + 1, anAsteroid->posX, anAsteroid->posY, anAsteroid->speed + 1, anAsteroid->angle + (90.0 * (PI / 180.0))));
	lev->activeElements.push_back(new Asteroid(anAsteroid->type + 1, anAsteroid->posX, anAsteroid->posY, anAsteroid->speed + 1, anAsteroid->angle - (90.0 * (PI / 180.0))));
}
