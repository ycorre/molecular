/*
 * Contains hero description
 */
 
#ifndef HERO_H
#define HERO_H

#include "graphicEngine.h"
#include "bonus.h"
#include "explosion.h"
#include "level.h"
#include "laser.h"

class Laser;

class Hero: public Drawable
{
	public:
	  int life;
	  int maxHealth;
	  int nbLife;
	  int heroMovingUpOrDown;
	  list<Laser*> lasers;
	  int canFire;
	  int fireRate;
	  int maxFireRate;
	  int leftFlag, rightFlag, topFlag, bottomFlag, dontMove; //last one used during automatic animations (e.g. the entrance one)
	  int invincible;
	  Uint32 invincibilityTime, startInvincibility;
	  unsigned int lastTimeFired;

	  Hero();
	  void fire();
	  void moveUp();
	  void moveDown();
	  void moveLeft();
	  void moveRight();
	  void animateLasers();
	  void checkFire();
	  void checkInvicibility();
	  void loseLife();
	  virtual void animate();
	  virtual int isHero() {return 1;}
	  virtual void processCollisionWith(Drawable * aDrawable);
};

#define STATIC 0
#define GO_UP 1
#define GO_DOWN 2
#define HIT 3
#define ENTER 4
#define DEAD 5

#endif

