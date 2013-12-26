/*
 * Contains herodescription
 */
 
#ifndef LASER_H
#define LASER_H

#include <math.h>
#include "graphicEngine.h"
#include "level.h"

class Weapon;

class Shoot: public Drawable
{
   public:
	  int power;
	  float angle;
	  Weapon * firingWeapon;

	  Shoot();
	  Shoot(int x, int y, Weapon * aWeapon);
	  virtual void animate();
	  virtual int isLaser() {return TRUE;}
	  virtual int isPhoton() {return FALSE;}
	  virtual void processCollisionWith(Drawable * aDrawable);
	  void createImpact(float x, float y);
};

class AnimatedShoot: public AnimatedDrawable
{
   public:
	  int power;
	  float angle;
	  Weapon * firingWeapon;

	  AnimatedShoot();
	 // virtual void animate();
	  virtual int isLaser() {return TRUE;}
	  virtual int isPhoton() {return FALSE;}
	  //virtual void processCollisionWith(Drawable * aDrawable);
	 // void createImpact(float x, float y);*/
};

class ElectronAmmo: public Shoot
{
   public:

	  ElectronAmmo(int x, int y, Weapon * aWeapon);
	  virtual int isLaser() {return TRUE;}
};

class Photon: public Shoot
{
   public:
	  AnimatedDrawable trail;

	  Photon();
	  Photon(int x, int y, Weapon * aWeapon);
	  void animate();
	  virtual int isLaser() {return TRUE;}
	  virtual int isPhoton() {return TRUE;}
	  virtual void processCollisionWith(Drawable * aDrawable);
	  void setParam(int x, int y, Weapon * aWeapon);
	  void removeEnergy(int anEnergyValue);
};

class Bullet: public Shoot
{
   public:
	  int power;
	  int speed;

	  Bullet();
	  Bullet(int x, int y, float anAngle, int speed);
	  virtual void animate();
	  virtual int isBullet() {return TRUE;}
	  virtual int isLaser() {return TRUE;}
	  virtual void processCollisionWith(Drawable * aDrawable);
};

class CadmiumAmmo: public Shoot
{
   public:
	  int power;
	  int speed;
	  Drawable halo;

	  CadmiumAmmo(int x, int y, float anAngle, float aSpeed);
	  virtual void animate();
	  virtual int isLaser() {return TRUE;}
	  virtual void processCollisionWith(Drawable * aDrawable);
};

#endif

