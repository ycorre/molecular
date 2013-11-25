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

class Enemy: public MaskedDrawable, public AnimatedDrawable
{
	public:
		int direction;
		int bonusProbability; //probability out of 100
		int type;
		int life;
		int canFire;
		int fireRate, minFireRate, maxFireRate;
		int scoreValue;
		float speed;
		unsigned int lastTimeFired;

		//Sinusoid movement stuff
		float originY;
		float sinusWidth;
		float sinusHeigth;

		Enemy();
		Enemy(int x, int y, int type, int dir);
		Enemy(int x, int y, float sinWidth, float sinHeigth, float speed);
		void dropBonus(int x, int y);
		virtual void animate();
		virtual void processCollisionWith(Drawable* aDrawable);
		virtual int isEnemy() {return 1;}
		virtual void fire();
		virtual void checkFire();
};


class Mitrailleur: public Enemy
{
	public:
		Mitrailleur();
		Mitrailleur(int x, int y);
		virtual void animate();
		virtual void checkFire();
};

#define ASTER_NORMAL 0
#define ASTER_DEMI 3
#define ASTER_QUARTER 4
#define ASTER_2THIRD 1
#define ASTER_1THIRD 2

class Bomb: public Enemy
{
	public:
		float angle;
		int speed;

		Bomb();
		Bomb(int sX, int sY, int speed, float angle);
		virtual void animate();
		void setAngleAndSpeed();
		virtual void fire();
};

#endif

