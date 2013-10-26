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
	  //Values kept from one level to the next
	  int health;
	  int nbLife;
	  int fireRate;

	  //Should be reset from one level to the next
	  int heroMovingUpOrDown;
	  int canFire;
	  int invincible;
	  list<Laser*> lasers;
	  unsigned int lastTimeFired;
	  int leftFlag, rightFlag, topFlag, bottomFlag, dontMove; //last one used during automatic animations (e.g. the entrance one)

	  //Should be constant throughout the game
	  int maxHealth;
	  int maxFireRate;
	  Uint32 invincibilityTime, startInvincibility;

	  Hero();
	  void setState(int aState);
	  void fire();
	  void moveUp();
	  void moveDown();
	  void moveLeft();
	  void moveRight();
	  void animateLasers();
	  void checkFire();
	  void makeInvincible();
	  void checkInvicibility();
	  void loseLife();
	  void resetHero();
	  virtual void animate();
	  virtual int isHero() {return 1;}
	  virtual void processCollisionWith(Drawable * aDrawable);

	private:
	  int heroChangedState;
};

#define STATIC 0
#define GO_UP 1
#define GO_DOWN 2
#define HIT 3
#define ENTER 4
#define DEAD 5
#define EXITING 6

#endif

