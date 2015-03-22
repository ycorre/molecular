/*
 * Contains bonus functions
 */

#include "include/bonus.h"


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
	isQuarkBonus = false;
	quantity = 0;
	quarkType = -1;
	speed = 0;
	angle = 0;
	drawn = false;
}


Bonus::Bonus(int x, int y, float aSpeed, float anAngle, bonusType bType)
{
	type = bType;
	name = "bonus " + type;
	isQuarkBonus = true;
	quantity = 0;
	quarkType = -1;
	speed = aSpeed;
	angle = anAngle;
	drawn = false;

	switch (type)
	{
		case BONUS_QUARKB_A:
			copyFrom(CurrentLevel->loadedObjects.at("qSp_ba"));
			quantity = 1;
			quarkType = QuarkB;
			break;

		case BONUS_QUARKB_G:
			copyFrom(CurrentLevel->loadedObjects.at("qSp_bg"));
			quantity = 5;
			quarkType = QuarkB;
			break;

		case BONUS_QUARKC_A:
			copyFrom(CurrentLevel->loadedObjects.at("qSp_ca"));
			quantity = 1;
			quarkType = QuarkC;
			break;

		case BONUS_QUARKC_G:
			copyFrom(CurrentLevel->loadedObjects.at("qSp_cg"));
			quantity = 5;
			quarkType = QuarkC;
			break;

		case BONUS_QUARKT_A:
			copyFrom(CurrentLevel->loadedObjects.at("qSp_ta"));
			quantity = 1;
			quarkType = QuarkT;
			break;

		case BONUS_QUARKT_G:
			copyFrom(CurrentLevel->loadedObjects.at("qSp_tg"));
			quantity = 5;
			quarkType = QuarkT;
			break;

		case BONUS_QUARKU_A:
			copyFrom(CurrentLevel->loadedObjects.at("qSp_ua"));
			quantity = 1;
			quarkType = QuarkU;
			break;

		case BONUS_QUARKU_G:
			copyFrom(CurrentLevel->loadedObjects.at("qSp_ug"));
			quantity = 5;
			quarkType = QuarkU;
			break;

		case BONUS_QUARKD_A:
			copyFrom(CurrentLevel->loadedObjects.at("qSp_da"));
			quantity = 1;
			quarkType = QuarkD;
			break;

		case BONUS_QUARKD_G:
			copyFrom(CurrentLevel->loadedObjects.at("qSp_dg"));
			quantity = 5;
			quarkType = QuarkD;
			break;

		case BONUS_QUARKS_A:
			copyFrom(CurrentLevel->loadedObjects.at("qSp_sa"));
			quantity = 1;
			quarkType = QuarkS;
			break;

		case BONUS_QUARKS_G:
			copyFrom(CurrentLevel->loadedObjects.at("qSp_sg"));
			quantity = 5;
			quarkType = QuarkS;
			break;

		case BONUS_SHIELD:
			copyFrom(CurrentLevel->loadedObjects.at("qSp_Capsule"));
			isQuarkBonus = false;
			break;

		case BONUS_LIFE_SMALL:
			copyFrom(CurrentLevel->loadedObjects.at("bSp_W"));
			quantity = 1;
			isQuarkBonus = false;
			break;

		case BONUS_LIFE_MEDIUM:
			copyFrom(CurrentLevel->loadedObjects.at("bSp_Z"));
			quantity = 3;
			isQuarkBonus = false;
			break;

		case BONUS_LIFE_BIG:
			copyFrom(CurrentLevel->loadedObjects.at("bSp_Y"));
			quantity = 5;
			isQuarkBonus = false;
			break;

		default:
			break;
	}

	halo.copyFrom(CurrentLevel->loadedObjects.at("qSp_Sign"));
	halo.posX = x;
	halo.posY = y;

	posX = x;
	posY = y;
	setAnimX(0);
	setAnimY(0);
	scoreValue = 200;
}

void Bonus::animate()
{
	updateAnimationFrame();

	if(!drawn)
		posX = posX - 2;

	if (speed != 0)
	{
		float vx, vy;
		vx = speed * cos(angle);
		vy = speed * sin(angle);

		posX = posX + vx;
		posY = posY + vy;

		halo.posX = posX;
		halo.posY = posY;

		//Make sure it does not disappear through the tob or the bottom of the screen
		posY = max(posY, GAMEZONE_LIMIT + height/2);
		posY = min(posY, (float) SCREEN_HEIGHT - height/2);

		if(!drawn)
			speed = speed * 0.92f;
	}

	if(!drawn)
		halo.posX = halo.posX - 2;

	halo.updateAnimationFrame();
	halo.processDisplay();

	if(!CurrentLevel->isOnScreen(this))
	{
		toRemove = true;
	}
}

void Bonus::processCollisionWith(Drawable* aDrawable)
{
	if (aDrawable->isHero())
	{
		Score = Score + scoreValue;
		toRemove = true;
	}
}

//"My Hero! <3"
void Bonus::drawnToHero()
{
	drawn = true;
	float xDiff = CurrentLevel->hero->posX - posX;
	float yDiff = CurrentLevel->hero->posY - posY;
	angle = atan2(yDiff, xDiff);

	speed = min(speed + 0.4f, 10.0f);
}
