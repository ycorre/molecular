/*
 * Contains herodescription
 */
 
#ifndef LASER_H
#define LASER_H

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

#endif

