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
	  bool isFiring;
	  bool canFire;
	  int fireRate;
	  weaponState powerLevel;
	  int maxFireRate;
	  list<Shoot *> shoots;
	  bool loadable;
	  float load;
	  float maxPower;
	  bool activated;

	  Weapon();
	  virtual ~Weapon();
	  virtual void fire(Hero * aHero);
	  virtual void releaseFire(Hero * aHero);
	  virtual void checkActivation(Hero * aHero);
	  virtual void upgradeTo(int aLevel);
	  void animateLasers();
	  void checkFire();
	  void createImpact(float x, float y);
};

class Electron: public Weapon
{
   public:
	  AnimatedDrawable muzzle;

	  Electron();
	  void fire(Hero * aHero);
	  void upgradeTo(int aLevel);
};

class Hadron: public Weapon
{
   public:
 	  float loadFactor;

	  Hadron();
	  void fire(Hero * aHero);
	  void releaseFire(Hero * aHero);
	  void checkActivation(Hero * aHero);
	  void upgradeTo(int aLevel);
};

class Baryon: public Weapon
{
   public:
	 Lazer * mahLazor;
	 AnimatedDrawable haloA, haloB;
	 Enemy * hitEnnemy;
	 float xImpactPos, yImpactPos;

	 Baryon();
	 ~Baryon();
	 void fire(Hero * aHero);
	 void upgradeTo(int aLevel);
	 void checkActivation(Hero * aHero);
	 void checkForCollision();
};


class Plasma: public Weapon
{
   public:
	 Plasma();
	 void fire(Hero * aHero);
	 void checkActivation(Hero * aHero);
};

#endif

