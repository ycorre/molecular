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

	  int type;
	  int power;
	  int direction;
	  float angle;
	  Weapon * firingWeapon;
	  AnimatedDrawable * trail;

	  Laser();
	  Laser(int x, int y, int dir, int type, Weapon * aWeapon);
	  virtual void animate();
	  virtual int isLaser() {return TRUE;}
	  virtual void processCollisionWith(Drawable* aDrawable);
	  void createImpact(float x, float y);
};

class Photon: public Laser
{
   public:

	Photon();
	Photon(int x, int y, int dir, int type, Weapon * aWeapon);
	  virtual void animate();
	  virtual int isLaser() {return TRUE;}
	  virtual void processCollisionWith(Drawable* aDrawable);
};

#define RED_LASER 0
#define GREEN_LASER 1

class Bullet: public MaskedDrawable,  public AnimatedDrawable
{
   public:

	  int type;
	  int power;
	  int direction;
	  float angle;
	  int speed;

	  Bullet();
	  Bullet(int x, int y, float anAngle, int speed);
	  virtual void animate();
	  virtual int isBullet() {return TRUE;}
	  virtual int isLaser() {return TRUE;}
	  virtual void processCollisionWith(Drawable* aDrawable);
};

#define BLUE_BULLET 0
#define GUIDED_BULLET 1

#endif

