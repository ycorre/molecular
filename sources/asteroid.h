/*
 * Asteroid.h
 */
#ifndef ASTEROID_H
#define ASTEROID_H
#include "graphicEngine.h"
#include "drawable.h"
#include "enemy.h"

enum pyroxeneType {BIG_PYROXENE, SMALL_PYROXENE};

class Pyroxene: public Enemy
{
	public:
		float angle;
		int speed;
		int life;
		int bonusProbability;
		int scoreValue;

		Pyroxene();
		Pyroxene(int type);
		//used to set small fragments of pyroxene
		Pyroxene(int type, int sX, int sY, int speed, float angle);
		void processCollisionWith(Drawable* aDrawable);
		void createSmallerPyroxene(Pyroxene * anElement);
		virtual void animate();
		virtual void fire();
		void setAngleAndSpeed();
};


#endif
