/*
 * Contains Weapon class definition
 */
 
#ifndef WEAPON_H
#define WEAPON_H

#include <math.h>
#include "graphicEngine.h"
#include "level.h"
#include "laser.h"

class Shoot;
class Lazer;
class Hero;

enum weaponState{WEAPON_STANDARD, WEAPON_SUPERIOR, WEAPON_FURIOUS, WEAPON_DEACTIVATE};

class Weapon
{
   public:
	  string name;
	  weaponName type;
	  float power;
	  unsigned int lastTimeFired;
	  int isFiring;
	  int canFire;
	  int fireRate;
	  weaponState powerLevel;
	  int maxFireRate;
	  list<Shoot *> shoots;
	  int loadable;
	  int load;
	  int activated;

	  Weapon();
	  virtual void fire(Hero * aHero);
	  virtual void checkActivation(Hero * aHero);
	  void animateLasers();
	  void checkFire();
	  virtual void upgradeTo(int aLevel);
	  void createImpact(float x, float y);
};

class Electron: public Weapon
{
   public:
		AnimatedDrawable muzzle;

	  Electron();
	  virtual void fire(Hero * aHero);
	  virtual void upgradeTo(int aLevel);
};

class Hadron: public Weapon
{
   public:
	 Hadron();
	 virtual void fire(Hero * aHero);
	 virtual void checkActivation(Hero * aHero);
};

class Baryon: public Weapon
{
   public:
	 Lazer * mahLazor;
	 AnimatedDrawable haloA, haloB;
	 Enemy * hitEnnemy;
	 float xImpactPos, yImpactPos;

	 Baryon();
	 virtual void fire(Hero * aHero);
	 virtual void upgradeTo(int aLevel);
	 virtual void checkActivation(Hero * aHero);
	 void checkForCollision();
};


class Plasma: public Weapon
{
   public:
	 Plasma();
	 virtual void fire(Hero * aHero);
	 virtual void checkActivation(Hero * aHero);
};

#endif

