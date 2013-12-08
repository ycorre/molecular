/*
 * Contains Weapon class definition
 */
 
#ifndef WEAPON_H
#define WEAPON_H

#include <math.h>
#include "graphicEngine.h"
#include "level.h"
#include "laser.h"

class Laser;
class Hero;

class Weapon: public Drawable
{
   public:
	  string name;
	  float power;
	  unsigned int lastTimeFired;
	  int isFiring;
	  int canFire;
	  int fireRate;
	  int type;
	  int maxFireRate;
	  list<Laser> shoots;
	  list<AnimatedDrawable *> impacts;

	  Weapon();
	  Weapon(string name, int power, int fireRate, int type);
	  void fire(Hero * aHero);
	  void animateLasers();
	  void checkFire();
	  void upgrade();
	  void createImpact(float x, float y);
};

#define WEAPON_ELECTRON 0
#define WEAPON_PHOTON 1

#endif

