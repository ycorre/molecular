#ifndef ENEMY_H
#define ENEMY_H

#include <algorithm>
#include "graphicEngine.h"
#include "explosion.h"
#include "level.h"
#include "laser.h"

class Cadmium;
class EnemyWave;

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
		float angle, nextAngle;
		float angleModifyingFactor;

		vector<pair <float, float> > eventPosition;

		Enemy();
		Enemy(int x, int y, int type);
		Enemy(int x, int y, float sinWidth, float sinHeigth, float speed);
		virtual void dropBonus(int x, int y);
		virtual void animate();
		virtual void checkPositions();
		virtual void processCollisionWith(Drawable* aDrawable);
		virtual bool isEnemy() {return true;}
		virtual bool hasSubpart() {return false;}
		virtual void fire();
		virtual void die();
		virtual void checkFire();
		virtual void addSubpart(EnemyWave * aWave);
};

class Cadmium: public Enemy
{
	public:
		int posRafale;
		int rafaleRate;
		float shootingAngle;

		Cadmium(int x, int y, float aSpeed, vector<int> moves);
		Cadmium(Json::Value aConfig);
		void checkFire();
		void fire();
		void die();
		void animate();
};

class Iron: public Enemy
{
	public:
		int nextDirectionShift, lastShift;
		int minNextShift, maxNextShift;

		Iron(Json::Value aConfig);
		void dropBonus(int x, int y);
		void animate();
};

class SiliconField: public Enemy
{
	public:
		EnemyWave * enemyWave;

		SiliconField(Json::Value aConfig, EnemyWave * anEnemyWave);
};

class Silicon: public Enemy
{
	public:
		int amplitude;
		float destY, destX;
		float ySpeed;
		float xSpeed;
		bool moving;

		//Silicon(Json::Value aConfig);
		Silicon(Json::Value aConfig, float aPosX);
		void animate();
		void die();
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
		vector<pair<float, float> > moves;

		Copper(Json::Value aConfig);
		bool hasSubpart() {return true;}
		void animate();
		void nextPhase();
		void move();
		void processCollisionWith(Drawable * aDrawable);
		void addSubpart(EnemyWave * aWave);
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
		float copperAngle;
		vector<int> cannonAngles;

		CopperCannon(Copper * aCopper, float x, float y);
		void animate();
		void fire(int anAngle);
		void processCollisionWith(Drawable * aDrawable);
};

class Rock;

class PyroxeneField: public Enemy
{
	public:
		int generationRate;
		int lastGeneration;
		int numberOfPyroxenes;
		int generatedPyroxenes;
		EnemyWave * enemyWave;

		PyroxeneField();
		PyroxeneField(Json::Value aConf, EnemyWave * anEnemyWave);
		bool generatePyroxene();
		void animate();
};

class Bomb: public Enemy
{
	public:
		float angle;
		int speed;

		Bomb();
		Bomb(int sX, int sY, int speed, float angle);
		void animate();
		void setAngleAndSpeed();
		void fire();
};

#endif

