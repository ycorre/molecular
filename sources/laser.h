/*
 * Contains herodescription
 */
 
#ifndef LASER_H
#define LASER_H

#include <math.h>
#include "graphicEngine.h"
#include "level.h"

class Laser: public MaskedDrawable
{
   public:

	  int type;
	  int power;
	  int direction;

	  Laser();
	  Laser(int x, int y, int dir, int type);
	  virtual void animate();
	  virtual int isLaser() {return TRUE;}
	  virtual void processCollisionWith(Drawable* aDrawable);
};

#define RED_LASER 0
#define GREEN_LASER 1

class Bullet: public MaskedDrawable
{
   public:

	  int type;
	  int power;
	  int direction;
	  int angleDeg;
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

