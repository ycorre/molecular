#include "enemy.h"

Asteroid::Asteroid()
{
	//	Enemy();
	//slide = destinationX =	destinationY =	posX =posY = -1;
}

//Asteroids appears randomly and have random trajectory
//So we get two random points (four values in total) which set the depart point and the destination point of the asteroid
Asteroid::Asteroid(int typeXW)
{
	Drawable();
	if (typeXW == ASTER_NORMAL)
	{
		texture = ge->textures.at("asteroid");
		width = atoi(((lev->configurationElements.at("asteroid")).at(0)).c_str());
		height = atoi(((lev->configurationElements.at("asteroid")).at(1)).c_str());
		nbFrames = parseAnimationState((lev->configurationElements.at("asteroid")).at(2));
	}
	if (typeXW == ASTER_DEMI)
	{
		texture = ge->textures.at("asteroid2");
		width = atoi(((lev->configurationElements.at("asteroid2")).at(0)).c_str());
		height = atoi(((lev->configurationElements.at("asteroid2")).at(1)).c_str());
		nbFrames = parseAnimationState((lev->configurationElements.at("asteroid2")).at(2));
	}
	if (typeXW == ASTER_2THIRD)
	{
		texture = ge->textures.at("asteroid23");
		width = atoi(((lev->configurationElements.at("asteroid23")).at(0)).c_str());
		height = atoi(((lev->configurationElements.at("asteroid23")).at(1)).c_str());
		nbFrames = parseAnimationState((lev->configurationElements.at("asteroid23")).at(2));
	}
	if (typeXW == ASTER_1THIRD)
	{
		texture = ge->textures.at("asteroid46");
		width = atoi(((lev->configurationElements.at("asteroid46")).at(0)).c_str());
		height = atoi(((lev->configurationElements.at("asteroid46")).at(1)).c_str());
		nbFrames = parseAnimationState((lev->configurationElements.at("asteroid46")).at(2));
	}

	state = 0;
	animX = 0;
	animY = 0;
	maxSpeed = 3;
	direction = UP;
	type = typeXW;
	bonusProbability = 10;
	canFire = FALSE;
	fireRate = 2500;
	lastTimeFired = 0;
	life = 50;

	setTrajectory();
}

//Asteroids have a predefine trajectory
//typîcally used for smaller asteroids resulting of the explosion of smaller asteroids
Asteroid::Asteroid(int typeXW, int sX, int sY, int destX, int destY, int dir)
{
	Drawable();
	if (typeXW == ASTER_NORMAL)
	{
		texture = ge->textures.at("asteroid");
		width = atoi(((lev->configurationElements.at("asteroid")).at(0)).c_str());
		height = atoi(((lev->configurationElements.at("asteroid")).at(1)).c_str());
		nbFrames = parseAnimationState((lev->configurationElements.at("asteroid")).at(2));
	}
	if (typeXW == ASTER_DEMI)
	{
		texture = ge->textures.at("asteroid2");
		width = atoi(((lev->configurationElements.at("asteroid2")).at(0)).c_str());
		height = atoi(((lev->configurationElements.at("asteroid2")).at(1)).c_str());
		nbFrames = parseAnimationState((lev->configurationElements.at("asteroid2")).at(2));
	}
	if (typeXW == ASTER_2THIRD)
	{
		texture = ge->textures.at("asteroid23");
		width = atoi(((lev->configurationElements.at("asteroid23")).at(0)).c_str());
		height = atoi(((lev->configurationElements.at("asteroid23")).at(1)).c_str());
		nbFrames = parseAnimationState((lev->configurationElements.at("asteroid23")).at(2));
	}
	if (typeXW == ASTER_1THIRD)
	{
		texture = ge->textures.at("asteroid46");
		width = atoi(((lev->configurationElements.at("asteroid46")).at(0)).c_str());
		height = atoi(((lev->configurationElements.at("asteroid46")).at(1)).c_str());
		nbFrames = parseAnimationState((lev->configurationElements.at("asteroid46")).at(2));
	}

	state = 0;
	animX = 0;
	animY = 0;
	maxSpeed = 3;
	direction = dir;
	type = typeXW;
	bonusProbability = 10;
	canFire = FALSE;
	fireRate = 2500;
	lastTimeFired = 0;
	life = 50;
	destinationX = destX;
	destinationY = destY;
	posX = sX + width / 2;
	posY = sY + height / 2;
	slide = (posX - posY) / (destinationX - destinationY);
}

void Asteroid::setTrajectory()
{
	//Set which side of the screen it will appear from
	int arrivalSide = rand() % 4;

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

	//Set which side of the screen it will exit to
	//We want it different from the arrival side
	int destSide = arrivalSide;
	while(destSide == arrivalSide)
	{
		destSide = rand() % 4;
	}

	//Set the coordinates of the destination point
	if (destSide == LEFT)
	{
		destinationX = -width;
		destinationY = rand() % GAMEZONE_HEIGHT;
	}
	if (destSide == RIGHT)
	{
		destinationX = SCREEN_WIDTH;
		destinationY = rand() % GAMEZONE_HEIGHT;
	}
	if (destSide == UP)
	{
		destinationY = -height;
		destinationX = rand() % SCREEN_WIDTH;
	}
	if (destSide == DOWN)
	{
		destinationY = GAMEZONE_HEIGHT;
		destinationX = rand() % SCREEN_WIDTH;
	}
	direction = destSide;
	slide = (posX - posY) / (destinationX - destinationY);
}


void Asteroid::animate()
{
	updateAnimationFrame();

	//Trick to limit the speed of the asteroids
	int step;
	int finalStep;

	//y - y1 = m(x - x1)
	switch(direction)
	{
		case UP:
			posY = posY - 2;
			//posX = (slide * (posY - destinationY)) + destinationX;
			step = posX - (slide * (posY - destinationY)) + destinationX;
			finalStep = min(step, maxSpeed);
			finalStep = max(finalStep, -maxSpeed);
			posX = posX + finalStep;
			break;
		case DOWN:
			posY = posY + 2;
			step = posX - (slide * (posY - destinationY)) + destinationX;
			finalStep = min(step, maxSpeed);
			finalStep = max(finalStep, -maxSpeed);
			posX = posX + finalStep;
			break;
		case LEFT:
			posX = posX - 2;
			//posY = (slide * (posX - destinationX)) + destinationY;
			step = posY - (slide * (posX - destinationX)) + destinationY;
			finalStep = min(step, maxSpeed);
			finalStep = max(finalStep, -maxSpeed);
			posY = posY + finalStep;
			break;
		case RIGHT:
			posX = posX + 2;
			step = posY - (slide * (posX - destinationX)) + destinationY;
			finalStep = min(step, maxSpeed);
			finalStep = max(finalStep, -maxSpeed);
			posY = posY + finalStep;
			break;
		default:
			break;
	}


}

void Asteroid::checkFire()
{

}

void Asteroid::processCollisionWith(Drawable* aDrawable)
{
	if(aDrawable->isHero())
	{
		lev->createExplosion(this->posX-this->width/2, this->posY - this->height/2, XWING_EXPL);
		dropBonus(this->posX, this->posY);
		this->toRemove = TRUE;
		return;
	}
	if (aDrawable->isLaser())
	{
		Laser * aLaser =  static_cast<Laser*>(aDrawable);
		life = life - aLaser->power;
		if (life<=0)
		{
			lev->createExplosion(this->posX-this->width/2, this->posY - this->height/2, XWING_EXPL);
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
}

void Asteroid::createSmallerAsteroid(Asteroid * anAsteroid)
{
	lev->activeElements.push_back(new Asteroid(anAsteroid->type + 1, anAsteroid->posX, anAsteroid->posY, anAsteroid->destinationX, anAsteroid->destinationY, ((anAsteroid->direction+1)%4)));
	lev->activeElements.push_back(new Asteroid(anAsteroid->type + 1, anAsteroid->posX, anAsteroid->posY, anAsteroid->destinationY, anAsteroid->destinationX, ((anAsteroid->direction-1)%4)));
}
