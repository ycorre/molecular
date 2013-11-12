#ifndef ENEMY_H
#define ENEMY_H

#include <algorithm>
#include "graphicEngine.h"
#include "explosion.h"
#include "level.h"
#include "laser.h"

#define XRED 0
#define XBLUE 1
#define XYELLOW 2

class Enemy: public Drawable
{
	public:
		int direction;
		int bonusProbability; //probability out of 100
		int type;
		int life;
		int canFire;
		int fireRate;
		int scoreValue;
		unsigned int lastTimeFired;

		Enemy();
		Enemy(int x, int y, int type, int dir);
		void dropBonus(int x, int y);
		virtual void animate();
		virtual void processCollisionWith(Drawable* aDrawable);
		virtual int isEnemy() {return 1;}
		virtual void fire();
		virtual void checkFire();
};


class Patrol: public Enemy
{
	public:
		Patrol();
		Patrol(int x, int y, int type, int dir);
		virtual void animate();
};

#define ASTER_NORMAL 0
#define ASTER_DEMI 3
#define ASTER_QUARTER 4
#define ASTER_2THIRD 1
#define ASTER_1THIRD 2

class Asteroid: public Enemy
{
	public:
		float angle;
		int speed;

		Asteroid();
		Asteroid(int type);
		//used to set small fragments of asteroid
		Asteroid(int typeXW, int sX, int sY, int speed, float angle);
		void processCollisionWith(Drawable* aDrawable);
		void createSmallerAsteroid(Asteroid * anElement);
		virtual void animate();
		void setAngleAndSpeed();
};

class Falcon: public Enemy
{
	public:
		int destinationX;
		int destinationY;
		int speed;
		int initialLife;
		int initialHealthDisplayWidth;
		int currentPhase;
		int fireCounter;
		Drawable * healthDisplay;

		Falcon();
		//Falcon(int type);
		void processCollisionWith(Drawable* aDrawable);
		virtual void fire();
		virtual void animate();
		virtual void checkFire();
		void firePattern();
		void displayHealth();
};

#define FALCON_PHASE_1 0
#define FALCON_PHASE_2 1
#define FALCON_PHASE_3 2

#endif

