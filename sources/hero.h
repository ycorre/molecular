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
#include "weapon.h"
#include "effect.h"

class Laser;
class Weapon;
class Effect;

class Hero: public MaskedDrawable , public AnimatedDrawable
{
	public:
	  //Values kept from one level to the next
	  int health;
	  int nbLife;
	  float regenMassPo;
	  Weapon * currentWeapon;
	  vector<Weapon*> ownedWeapons;

	  //Should be reset from one level to the next
	  int heroMovingUpOrDown;
	  int canFire;
	  int invincible;
	  int isFiring;
	  int leftFlag, rightFlag, topFlag, bottomFlag, dontMove; //last one used during automatic animations (e.g. the entrance one)
	  float massPotential;
	  float radioactivePotential;
	  float backOffSpeed;

	  AnimatedDrawable * effect;
	  string currentEffect;
	  vector<Effect *> activeEffects;

	  //Should be constant throughout the game
	  int maxHealth;
	  Uint32 invincibilityTime, startInvincibility;
	  Uint32 startTeleporting;

	  float hitAngle;

	  Hero();
	  void setState(int aState);
	  void setTexture(Drawable * aModel);
	  void fire();
	  void move(int x, int y);
	  void moveUp();
	  void moveDown();
	  void moveLeft();
	  void moveRight();
	  void animateLasers();
	  void makeInvincible(int time);
	  void checkInvicibility();
	  void loseLife();
	  void resetHero();
	  float hitBackoff();
	  void teleport();
	  void playEffect();
	  void startEffect(string anEffect);
	  int endTeleport();
	  list<Laser*> * getLasers();
	  virtual void animate();
	  virtual int isHero() {return 1;}
	  virtual void processCollisionWith(Drawable * aDrawable);

	private:
	  AnimatedDrawable * firingEffect;
	  int heroChangedState;
};

#define STATIC 0
#define APPAR 4
#define DISPAR 5
#define HIT 1
#define ENTER 2
#define DEAD 3
#define EXITING 6
#define CURSOR 7

#endif

