/*
 * Asteroid.h
 */
#ifndef ASTEROID_H
#define ASTEROID_H
#include "graphicEngine.h"
#include "drawable.h"
#include "enemy.h"

enum asteroidType {BIG_ASTEROID, SMALL_ASTEROID};

class Asteroid: public Enemy
{
	public:
		float angle;
		int speed;
		int life;
		int bonusProbability;
		int scoreValue;

		Asteroid();
		Asteroid(int type);
		//used to set small fragments of asteroid
		Asteroid(int type, int sX, int sY, int speed, float angle);
		void processCollisionWith(Drawable* aDrawable);
		void createSmallerAsteroid(Asteroid * anElement);
		virtual void animate();
		virtual void fire();
		void setAngleAndSpeed();
};


#endif
