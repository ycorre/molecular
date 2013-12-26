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
class Hero;

class Weapon
{
   public:
	  string name;
	  float power;
	  unsigned int lastTimeFired;
	  int isFiring;
	  int canFire;
	  int fireRate;
	  int level;
	  int maxFireRate;
	  list<Shoot *> shoots;
	  int loadable;
	  int load;

	  Weapon();
	  virtual void fire(Hero * aHero);
	  void animateLasers();
	  void checkFire();
	  void upgrade();
	  void createImpact(float x, float y);
};

class Electron: public Weapon
{
   public:
	  Electron();
	  virtual void fire(Hero * aHero);
};

class Hadron: public Weapon
{
   public:
	 Hadron();
	 virtual void fire(Hero * aHero);
};

class Baryon: public Weapon
{
   public:
	 Baryon();
};


class Plasma: public Weapon
{
   public:
	 Plasma();
};

#endif

