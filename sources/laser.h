/*
 * Contains herodescription
 */
 
#ifndef LASER_H
#define LASER_H

#include <math.h>
#include "graphicEngine.h"
#include "level.h"

class Laser: public Drawable
{
   public:

	  int type;
	  int power;
	  int direction;

	  Laser();
	  Laser(int x, int y, int dir, int type);
	  virtual void animate();
	  virtual int isLaser() {return 1;}
	  virtual void processCollisionWith(Drawable* aDrawable);
};

#define RED_LASER 0
#define GREEN_LASER 1

class Bullet: public Drawable
{
   public:

	  int type;
	  int power;
	  int direction;
	  int angleDeg;
	  float angle;
	  int speed;

	  Bullet();
	  Bullet(int x, int y, int dir, int type);
	  Bullet(int x, int y, int dir, int aType, int anAngle, int speed);
	  virtual void animate();
	  virtual int isBullet() {return 1;}
	  virtual int isLaser() {return 1;}
	  virtual void processCollisionWith(Drawable* aDrawable);
};

#define BLUE_BULLET 0
#define GUIDED_BULLET 1

#endif

