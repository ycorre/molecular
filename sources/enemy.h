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
		bool canFire;
		int fireRate, minFireRate, maxFireRate;
		int scoreValue;
		float speed;
		unsigned int lastTimeFired;
		unsigned int currentPosition;
		int damage;
		vector<bonusType> possibleBonus;

		//Sinusoid movement stuff
		float originY;
		float sinusWidth;
		float sinusHeigth;
		float angle;

		vector<pair <float, float> > eventPosition;

		Enemy();
		Enemy(int x, int y, int type);
		Enemy(int x, int y, float sinWidth, float sinHeigth, float speed);
		virtual void dropBonus(int x, int y);
		virtual void animate();
		virtual void checkPositions();
		virtual void processCollisionWith(Drawable* aDrawable);
		virtual bool isEnemy() {return true;}
		virtual void fire();
		virtual void die();
		virtual void checkFire();
};

class Cadmium: public Enemy
{
	public:
		int posRafale;
		int rafaleRate;
		float shootingAngle;

		Cadmium(int x, int y, float aSpeed, vector<int> moves);
		Cadmium(Json::Value aConfig);
		virtual void checkFire();
		virtual void fire();
		virtual void animate();
};

class Iron: public Enemy
{
	public:
		int nextDirectionShift, lastShift;
		int minNextShift, maxNextShift;

		Iron(Json::Value aConfig);
		virtual void dropBonus(int x, int y);
		virtual void animate();
};

class Silicon: public Enemy
{
	public:
		Silicon(Json::Value aConfig);
		virtual void animate();
		virtual void die();
};

class CopperCannon;
class Copper: public Enemy
{
	public:
		vector<CopperCannon *> cannons;
		int lifeTime;
		int currentLifeTime;
		float xLimit;
		float yDestination;
		int lifePhase;
		float ySpeed;
		bool invincible;
		int spinningAngle;
		int spinningCounter;
		int activeCannons;

		Copper(Json::Value aConfig);
		virtual void animate();
		void nextPhase();
		virtual void move();
		virtual void processCollisionWith(Drawable * aDrawable);
};

//Copper phase
enum {COPPER_FIRE, COPPER_SPIN, COPPER_STEADY, COPPER_ARRIVING, COPPER_LEAVING};

class CopperCannon: public Enemy
{
	public:
		int shootingAngle;
		int startShootingAngle;
		bool activated;
		bool destroyed;
		Copper * copper;
		float shiftX, shiftY;
		float copperAngle;
		vector<int> cannonAngles;

		CopperCannon(Copper * aCopper, float x, float y);
		virtual void animate();
		virtual void fire();
		virtual void processCollisionWith(Drawable * aDrawable);
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

