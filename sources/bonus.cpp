/*
 * Contains bonus functions
 */

#include "bonus.h"


Bonus::Bonus()
{
	type = BONUS_SHIELD;
	name = "bonus";
	width = texture->w;
	height = texture->h;
	posX = 0;
	posY = 0;
	setAnimX(0);
	setAnimY(0);
	scoreValue = 200;
	isQuarkBonus = FALSE;
	quantity = 0;
	quarkType = -1;
	speed = 0;
	angle = 0;
}


Bonus::Bonus(int x, int y, float aSpeed, float anAngle, bonusType bType)
{
	type = bType;
	name = "bonus " + type;
	isQuarkBonus = TRUE;
	quantity = 0;
	quarkType = -1;
	speed = aSpeed;
	angle = anAngle;

	switch (type)
	{
		case BONUS_QUARKB_A:
			copyFrom(lev->loadedObjects.at("qSp_ba"));
			quantity = 1;
			quarkType = QuarkB;
			break;

		case BONUS_QUARKB_G:
			copyFrom(lev->loadedObjects.at("qSp_bg"));
			quantity = 5;
			quarkType = QuarkB;
			break;

		case BONUS_QUARKC_A:
			copyFrom(lev->loadedObjects.at("qSp_ca"));
			quantity = 1;
			quarkType = QuarkC;
			break;

		case BONUS_QUARKC_G:
			copyFrom(lev->loadedObjects.at("qSp_cg"));
			quantity = 5;
			quarkType = QuarkC;
			break;

		case BONUS_QUARKT_A:
			copyFrom(lev->loadedObjects.at("qSp_ta"));
			quantity = 1;
			quarkType = QuarkT;
			break;

		case BONUS_QUARKT_G:
			copyFrom(lev->loadedObjects.at("qSp_tg"));
			quantity = 5;
			quarkType = QuarkT;
			break;

		case BONUS_QUARKU_A:
			copyFrom(lev->loadedObjects.at("qSp_ua"));
			quantity = 1;
			quarkType = QuarkU;
			break;

		case BONUS_QUARKU_G:
			copyFrom(lev->loadedObjects.at("qSp_ug"));
			quantity = 5;
			quarkType = QuarkU;
			break;

		case BONUS_QUARKD_A:
			copyFrom(lev->loadedObjects.at("qSp_da"));
			quantity = 1;
			quarkType = QuarkD;
			break;

		case BONUS_QUARKD_G:
			copyFrom(lev->loadedObjects.at("qSp_dg"));
			quantity = 5;
			quarkType = QuarkD;
			break;

		case BONUS_QUARKS_A:
			copyFrom(lev->loadedObjects.at("qSp_sa"));
			quantity = 1;
			quarkType = QuarkS;
			break;

		case BONUS_QUARKS_G:
			copyFrom(lev->loadedObjects.at("qSp_sg"));
			quantity = 5;
			quarkType = QuarkS;
			break;

		case BONUS_SHIELD:
			copyFrom(lev->loadedObjects.at("qSp_Capsule"));
			isQuarkBonus = FALSE;
			break;

		case BONUS_LIFE_SMALL:
			copyFrom(lev->loadedObjects.at("bSp_W"));
			quantity = 1;
			isQuarkBonus = FALSE;
			break;

		case BONUS_LIFE_MEDIUM:
			copyFrom(lev->loadedObjects.at("bSp_Z"));
			quantity = 3;
			isQuarkBonus = FALSE;
			break;

		case BONUS_LIFE_BIG:
			copyFrom(lev->loadedObjects.at("bSp_Y"));
			quantity = 5;
			isQuarkBonus = FALSE;
			break;

		default:
			break;
	}

	halo.copyFrom(lev->loadedObjects.at("qSp_Sign"));
	halo.posX = x + (width/2 - halo.width/2);
	halo.posY = y + (height/2 - halo.height/2);

	posX = x;
	posY = y;
	setAnimX(0);
	setAnimY(0);
	scoreValue = 200;
}

void Bonus::animate()
{
	updateAnimationFrame();

	posX = posX - 2;

	if (speed != 0)
	{
		float vx, vy;
		vx = speed * cos(angle);
		vy = speed * sin(angle);

		posX = posX + vx;
		posY = posY + vy;

		halo.posX = halo.posX + vx;
		halo.posY = halo.posY + vy;

		speed = speed * 0.92f;
	}

	halo.posX = halo.posX - 2;
	halo.updateAnimationFrame();
	halo.processDisplay();

	if(!lev->isOnScreen(this))
	{
		toRemove = TRUE;
	}
}

void Bonus::processCollisionWith(Drawable* aDrawable)
{
	if (aDrawable->isHero())
	{
		Score = Score + scoreValue;
		toRemove = TRUE;
	}
}
