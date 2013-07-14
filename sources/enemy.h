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
		unsigned int lastTimeFired;

		Enemy();
		Enemy(int x, int y, int type, int dir);
		void dropBonus(int x, int y);
		virtual void animate();
		virtual void processCollisionWith(Drawable* aDrawable);
		virtual int isEnemy() {return 1;}
		void fire();
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
		int destinationX;
		int destinationY;
		int maxSpeed;
		float slide;

		Asteroid();
		Asteroid(int type);
		//used to set small fragment of asteroid
		Asteroid(int typeXW, int sX, int sY, int destX, int destY, int dir);
		void processCollisionWith(Drawable* aDrawable);
		void createSmallerAsteroid(Asteroid * anElement);
		virtual void animate();
		void setTrajectory();
		virtual void checkFire();
};

#endif

