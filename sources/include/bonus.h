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
	  bool isQuarkBonus;
	  float speed;
	  float angle;
	  AnimatedDrawable halo;
	  bool drawn;

	  Bonus();
	  Bonus(int x, int y, float speed, float angle, bonusType type);
	  void animate();
	  bool isBonus() {return true;}
	  void processCollisionWith(Drawable * aDrawable);
	  void drawnToHero();
};

#endif

