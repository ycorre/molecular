#include "enemy.h"

Bomb::Bomb()
{
	Drawable();

	width = atoi(((lev->configurationElements.at("bomb")).at(0)).c_str());
	height = atoi(((lev->configurationElements.at("bomb")).at(1)).c_str());
	this->addTexture("bomb");
	nbFrames = parseAnimationState((lev->configurationElements.at("bomb")).at(2));
	collision =  ge->loadTexture("res/Joueur_Col.png");

	state = 0;
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
	Drawable();
	this->addTexture("bomb");
	width = atoi(((lev->configurationElements.at("bomb")).at(0)).c_str());
	height = atoi(((lev->configurationElements.at("bomb")).at(1)).c_str());
	posX = x;
	posY = y;
	state = 0;
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
	collision =  ge->loadTexture("res/Ennemi_mask.png");
	nbFrames = parseAnimationState((lev->configurationElements.at("bomb")).at(2));
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
		//lev->soundEngine->playSound("enemyGun");

		//Shoot toward the hero
		//Compute the angle

		lev->activeElements.push_back(new Bullet(posX + 14, posY + 16, 0, 3));
		lev->activeElements.push_back(new Bullet(posX + 14, posY + 16, 45* (PI / 180.0), 3));
		lev->activeElements.push_back(new Bullet(posX + 14, posY + 16, 90* (PI / 180.0), 3));
		lev->activeElements.push_back(new Bullet(posX + 14, posY + 16, 135* (PI / 180.0), 3));
		lev->activeElements.push_back(new Bullet(posX + 14, posY + 16, 180* (PI / 180.0), 3));
		lev->activeElements.push_back(new Bullet(posX + 14, posY + 16, 225* (PI / 180.0), 3));
		lev->activeElements.push_back(new Bullet(posX + 14, posY + 16, 270* (PI / 180.0), 3));
		lev->activeElements.push_back(new Bullet(posX + 14 , posY + 16, 315* (PI / 180.0), 3));

		canFire = 0;
	}
}

//Randomily decide the characteristics of an asteroid (speed and angle)
void Bomb::setAngleAndSpeed()
{
	//Set which side of the screen it will appear from
	int arrivalSide = (rand() % 3);
	int angleDegree;

	//Set the angle (in degrees)
	if (arrivalSide == RIGHT)
	{
		posX = SCREEN_WIDTH;
		posY = rand() % GAMEZONE_HEIGHT;

		angleDegree = (rand() % 90) + 135; // 45 and 315
	}
	if (arrivalSide == UP)
	{
		posY = -height;
		posX = rand() % SCREEN_WIDTH;
		angleDegree = rand() % 180; //values between 0 and 180
	}
	if (arrivalSide == DOWN || arrivalSide == LEFT)
	{
		posY = GAMEZONE_HEIGHT;
		posX = rand() % SCREEN_WIDTH;
		angleDegree = (rand() % 180) + 180; // -180 and 0
	}

	//Convert into radians
	angle = angleDegree * (PI / 180.0);

	//Set the speed (between 2 and 7)
	speed = (rand() % 3) + 1;
}
