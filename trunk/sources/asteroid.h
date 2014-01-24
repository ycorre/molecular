/*
 * Asteroid.h
 */
#ifndef ASTEROID_H
#define ASTEROID_H
#include "graphicEngine.h"
#include "drawable.h"
#include "enemy.h"

enum rockType {BIG_ROCK, SMALL_ROCK};

class Rock: public Enemy
{
	public:
		float angle;
		int speed;
		int life;
		int bonusProbability;
		int scoreValue;

		Rock();
		Rock(int type);
		//used to set small fragments of rock
		Rock(int type, int sX, int sY, int speed, float angle);
		void processCollisionWith(Drawable* aDrawable);
		virtual void createSmallerRock(Rock * anElement);
		virtual void animate();
		void setAngleAndSpeed();
};

class Pyroxene: public Rock
{
	public:

		Pyroxene(int type);
		//used to set small fragments of rock
		Pyroxene(int type, int sX, int sY, int speed, float angle);
		virtual void createSmallerRock(Rock * anElement);
};


#endif
