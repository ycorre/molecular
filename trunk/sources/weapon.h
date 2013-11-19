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
	  int maxFireRate;
	  list<Laser *> shoots;
	  list<Drawable *> impacts;

	  Weapon();
	  void fire(Hero * aHero);
	  void animateLasers();
	  void checkFire();
	  void upgrade();
	  void createImpact(float x, float y);
};

#endif

