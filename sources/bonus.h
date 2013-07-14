/*
 * Contains bonuses description
 */
 
#ifndef BONUS_H
#define BONUS_H

#include "graphicEngine.h"
#include "level.h"

class Bonus: public Drawable
{
   public:

	  int type;

	  Bonus();
	  Bonus(int x, int y, int type);
	  virtual void animate();
	  virtual int isBonus() {return 1;}
	  virtual void processCollisionWith(Drawable* aDrawable);
};

#define BLIFE 0
#define BFIRERATE 1

#endif

