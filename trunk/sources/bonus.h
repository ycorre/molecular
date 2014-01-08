/*
 * Contains bonuses description
 */
 
#ifndef BONUS_H
#define BONUS_H

#include "graphicEngine.h"
#include "level.h"

class Bonus: public AnimatedDrawable
{
   public:
	  bonusType type;
	  int scoreValue;
	  int quarkType;
	  int quantity;
	  int isQuarkBonus;
	  float speed;
	  float angle;
	  AnimatedDrawable halo;

	  Bonus();
	  Bonus(int x, int y, float speed, float angle, bonusType type);
	  virtual void animate();
	  virtual int isBonus() {return TRUE;}
	  virtual void processCollisionWith(Drawable * aDrawable);
};

#endif

