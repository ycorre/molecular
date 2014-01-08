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

class Shoot;
class Weapon;
class Effect;
class Shield;

class Hero: public AnimatedDrawable
{
	public:
	  //Values kept from one level to the next
	  int health;
	  int nbLife;
	  float regenMassPo;
	  Weapon * currentWeapon;
	  map<weaponName, Weapon *> ownedWeapons;
	  map<QuarkType, int> quarkLevels;
	  int shielded;
	  Shield * shield;

	  //Should be reset from one level to the next
	  int heroMovingUpOrDown;
	  int canFire;
	  int invincible;
	  int isFiring;
	  int leftFlag, rightFlag, topFlag, bottomFlag, dontMove; //last one used during automatic animations (e.g. the entrance one)
	  float massPotential;
	  float radioactivePotential;
	  float backOffSpeed;
	  int state;
	  list<Shoot *> shoots;

	  //Should be constant throughout the game
	  int maxHealth, maxLife;
	  Uint32 invincibilityTime, startInvincibility;
	  Uint32 startTeleporting;

	  float hitAngle;

	  Hero();
	  void setState(int aState);
	  void setTexture(Drawable * aModel);
	  void fire();
	  void fireWeapon(weaponName aWeaponName);
	  void upgradeWeaponTo(int aLevel);
	  void move(int x, int y);
	  void moveUp();
	  void moveDown();
	  void moveLeft();
	  void moveRight();
	  void animateLasers();
	  void makeInvincible(int time);
	  void checkInvicibility();
	  void setWeapon(weaponName aWeaponName);
	  void loseLife();
	  void resetHero();
	  float hitBackoff();
	  void teleport();
	  void startEffect(string anEffect);
	  int endTeleport();
	  list<Shoot *> * getLasers();
	  void spreadQuarks();
	  void checkQuarkLevels();
	  virtual void animate();
	  virtual int isHero() {return 1;}
	  virtual void processCollisionWith(Drawable * aDrawable);

	private:
	  int heroChangedState;
};

class Shield: public AnimatedDrawable
{
public:
	float energy;

	Shield();
	Shield(float x, float y);
};

#endif

