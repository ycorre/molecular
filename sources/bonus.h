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
	  int type;
	  int scoreValue;
	  int quarkType;
	  int quantity;
	  int isQuarkBonus;
	  float speed;
	  float angle;
	  AnimatedDrawable halo;

	  Bonus();
	  Bonus(int x, int y, float speed, float angle, int type);
	  virtual void animate();
	  virtual int isBonus() {return TRUE;}
	  virtual void processCollisionWith(Drawable * aDrawable);
};


enum bonusType {BONUS_SHIELD, BONUS_QUARKB_A, BONUS_QUARKB_G, BONUS_QUARKC_A, BONUS_QUARKC_G,
	BONUS_QUARKT_A, BONUS_QUARKT_G, BONUS_QUARKD_A, BONUS_QUARKD_G, BONUS_QUARKU_A,
	BONUS_QUARKU_G, BONUS_QUARKS_A, BONUS_QUARKS_G, BONUS_LIFE_SMALL, BONUS_LIFE_MEDIUM, BONUS_LIFE_BIG};

#endif

