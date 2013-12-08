/*
 * Contains herodescription
 */
 
#ifndef LASER_H
#define LASER_H

#include <math.h>
#include "graphicEngine.h"
#include "level.h"

class Weapon;

class Laser: public MaskedDrawable, public AnimatedDrawable
{
   public:
	  int power;
	  float angle;
	  Weapon * firingWeapon;
	  AnimatedDrawable * trail;

	  Laser();
	  Laser(int x, int y, Weapon * aWeapon);
	  virtual void animate();
	  virtual int isLaser() {return TRUE;}
	  virtual int isPhoton() {return FALSE;}
	  virtual void processCollisionWith(Drawable * aDrawable);
	  void createImpact(float x, float y);
};

class Photon: public Laser
{
   public:
	  Photon();
	  Photon(int x, int y, Weapon * aWeapon);
	  virtual void animate();
	  virtual int isLaser() {return TRUE;}
	  virtual int isPhoton() {return TRUE;}
	  virtual void processCollisionWith(Drawable * aDrawable);
	  void setParam(int x, int y, Weapon * aWeapon);
	  void removeEnergy(int anEnergyValue);
};

class Bullet: public MaskedDrawable,  public AnimatedDrawable
{
   public:
	  int power;
	  float angle;
	  int speed;

	  Bullet();
	  Bullet(int x, int y, float anAngle, int speed);
	  virtual void animate();
	  virtual int isBullet() {return TRUE;}
	  virtual int isLaser() {return TRUE;}
	  virtual void processCollisionWith(Drawable* aDrawable);
};

#endif

