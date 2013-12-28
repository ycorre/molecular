#ifndef ENEMY_H
#define ENEMY_H

#include <algorithm>
#include "graphicEngine.h"
#include "explosion.h"
#include "level.h"
#include "laser.h"

class Cadmium;

class Enemy: public AnimatedDrawable
{
	public:
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
		Enemy(int x, int y, int type);
		Enemy(int x, int y, float sinWidth, float sinHeigth, float speed);
		void dropBonus(int x, int y);
		virtual void animate();
		virtual void processCollisionWith(Drawable* aDrawable);
		virtual int isEnemy() {return 1;}
		virtual void fire();
		virtual void checkFire();
};

class Cadmium: public Enemy
{
	public:
		int posRafale;
		int rafaleRate;
		float angle;

		Cadmium(int x, int y, float sinWidth, float sinHeigth, float aSpeed);
		virtual void checkFire();
		virtual void fire();
};


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

class Mitraille: virtual public Bomb
{
	public:
		int posRafale;
		int rafaleRate;

		Mitraille();
		Mitraille(int x, int y);
		virtual void checkFire();
		virtual void fire();
};

#endif

