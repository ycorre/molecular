#include "enemy.h"

Bomb::Bomb()
{
	copyFrom(lev->loadedObjects.at("bomb"));
	collision = ge->loadTexture("res/Bomb_Col.png");
	setAnimX(0);
	setAnimY(0);
	scoreValue = 500;
	direction = UP;
	type = 0;
	bonusProbability = 10;
	canFire = FALSE;
	fireRate = 2000;
	lastTimeFired = GameTimer;
	life = 250;
	setAngleAndSpeed();

	minFireRate = 2000;
	maxFireRate = 1000;
	fireRate = minFireRate + (rand() % maxFireRate);
}

Bomb::Bomb(int x, int y, int aSpeed, float anAngle)
{
	copyFrom(lev->loadedObjects.at("bomb"));
	posX = x;
	posY = y;
	setAnimX(0);
	setAnimY(0);
	direction = RIGHT;
	type = 0;
	scoreValue = 200;
	bonusProbability = 10;
	canFire = FALSE;
	minFireRate = 2000;
	maxFireRate = 1000;
	fireRate = minFireRate + (rand() % maxFireRate);
	lastTimeFired = 0;
	life = 250;
	collision = ge->loadTexture("res/Ennemi_mask.png");

	speed = aSpeed;
	angle = anAngle * (PI / 180.0);
}

void Bomb::animate()
{
	updateAnimationFrame();

	float vx, vy;
	vx = (speed) * cos(angle);
	vy = (speed) * sin(angle);
	posX = posX + vx;
	posY = posY + vy;
}


void Bomb::fire()
{
	checkFire();
	if (canFire)
	{
		lev->activeElements.push_back(new Bullet(posX + 14, posY + 16, 0, 3));
		lev->activeElements.push_back(new Bullet(posX + 14, posY + 16, 45* (PI / 180.0), 3));
		lev->activeElements.push_back(new Bullet(posX + 14, posY + 16, 90* (PI / 180.0), 3));
		lev->activeElements.push_back(new Bullet(posX + 14, posY + 16, 135* (PI / 180.0), 3));
		lev->activeElements.push_back(new Bullet(posX + 14, posY + 16, 180* (PI / 180.0), 3));
		lev->activeElements.push_back(new Bullet(posX + 14, posY + 16, 225* (PI / 180.0), 3));
		lev->activeElements.push_back(new Bullet(posX + 14, posY + 16, 270* (PI / 180.0), 3));
		lev->activeElements.push_back(new Bullet(posX + 14, posY + 16, 315* (PI / 180.0), 3));

		lev->activeElements.push_back(new Bullet(posX + 14, posY + 16, 22.5 * (PI / 180.0), 3));
		lev->activeElements.push_back(new Bullet(posX + 14, posY + 16, 67.5 * (PI / 180.0), 3));
		lev->activeElements.push_back(new Bullet(posX + 14, posY + 16, 112.5* (PI / 180.0), 3));
		lev->activeElements.push_back(new Bullet(posX + 14, posY + 16, 157.5* (PI / 180.0), 3));
		lev->activeElements.push_back(new Bullet(posX + 14, posY + 16, 202.5* (PI / 180.0), 3));
		lev->activeElements.push_back(new Bullet(posX + 14, posY + 16, 247.5* (PI / 180.0), 3));
		lev->activeElements.push_back(new Bullet(posX + 14, posY + 16, 292.5* (PI / 180.0), 3));
		lev->activeElements.push_back(new Bullet(posX + 14, posY + 16, 337.5* (PI / 180.0), 3));

		canFire = FALSE;
	}
}

//Randomily decide the characteristics of an asteroid (speed and angle)
void Bomb::setAngleAndSpeed()
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

	//Set the speed (between 2 and 7)
	speed = (rand() % 3) + 1;
}
