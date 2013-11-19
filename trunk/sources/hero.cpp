/*
 * Contains herodescription
 */

#include "hero.h"

Hero::Hero()
{
	health = 4;
	maxHealth = 8;
	nbLife = 5;
	posX = 0;
	posY = 170;
	state = ENTER;
	invincible = FALSE;
	invincibilityTime = 1500;
	heroChangedState = TRUE;
	canFire = 1;
	collision =  ge->loadTexture("res/Joueur_Col.png");
	heroMovingUpOrDown = 0;
	topFlag = leftFlag = bottomFlag = rightFlag = dontMove = 0;
	isFiring = FALSE;
	regenMassPo = 0.1;
	massPotential = 0;
	radioactivePotential = 0;
	toBlend = TRUE;
	currentWeapon = new Weapon();
	currentEffect = "";
}

void Hero::setTexture()
{
	name = "atom";
	addTexture("atom");
	setWidth(atoi(((lev->configurationElements.at("atom")).at(0)).c_str()));
	height = atoi(((lev->configurationElements.at("atom")).at(1)).c_str());
	nbFrames = parseAnimationState((lev->configurationElements.at("atom")).at(2));
	addTexture("atomHit");
	addTexture("atomDead");
	addTexture("atomEnter");
	addTexture("atomAppar");
	addTexture("atomDispar");
	addTexture("atomCursor");
	setAnimX(0);

	effect = new MultiTextureDrawable();
	effect->name = "effect";
	effect->height =  atoi(((lev->configurationElements.at("effectJoueur")).at(1)).c_str());
	effect->setWidth(atoi(((lev->configurationElements.at("effectJoueur")).at(0)).c_str()));
	effect->nbFrames = parseAnimationState((lev->configurationElements.at("effectJoueur")).at(2));
	effect->toBlend = TRUE;
	effect->addTexture("effectHit");
	effect->addTexture("effectAppar");
	effect->addTexture("effectDispar");

	firingEffect = new Drawable();
	firingEffect->addTexture("muzzl");
	firingEffect->nbFrames = parseAnimationState((lev->configurationElements.at("muzzl")).at(2));
	firingEffect->width = atoi(((lev->configurationElements.at("muzzl")).at(0)).c_str());
	firingEffect->height = atoi(((lev->configurationElements.at("muzzl")).at(1)).c_str());
	firingEffect->posX = 0;
	firingEffect->posY = 0;
	firingEffect->setAnimX(0);
	firingEffect->setAnimY(0);
	firingEffect->toBlend = TRUE;
	firingEffect->display = FALSE;
}


void Hero::resetHero()
{
	if(health < 4)
		health = 4;

	posX = 0;
	posY = 170;
	state = ENTER;
	invincible = FALSE;
	invincibilityTime = 1250;
	heroChangedState = TRUE;
	canFire = 1;
	heroMovingUpOrDown = 0;
	topFlag = leftFlag = bottomFlag = rightFlag = dontMove = 0;
	massPotential = 0;
	radioactivePotential = 0;
	isFiring = FALSE;
	currentEffect = "";
}

void Hero::animate()
{
//TODO can we do something more generic ? states loaded through a textfile

	//If we have changed state between two calls to this function
	if (heroChangedState)
	{
		dontMove = FALSE;
		switch(state)
		{
			case STATIC:
				setTextureState("");
				setAnimX(0);
				setAnimY(0);
				break;

			case DISPAR:
				dontMove = TRUE;
				makeInvincible(4000);
				if(currentEffect.empty())
				{
					heroChangedState = FALSE;
					setTextureState("Dispar");
					setAnimX(0);
					setAnimY(0);
				}
				break;

			case CURSOR:
				setTextureState("Cursor");
				makeInvincible(3000);
				startTeleporting = GameTimer;
				dontMove = FALSE;
				setAnimX(0);
				setAnimY(0);
				break;

			case APPAR:

				setTextureState("Appar");
				dontMove = TRUE;
				makeInvincible(4000);
				setAnimX(0);
				setAnimY(0);
				break;

			case HIT:
				setTextureState("Hit");
				dontMove = TRUE;
				makeInvincible(2000);
				setAnimX(0);
				setAnimY(0);
				break;
		
			case ENTER:
				setAnimX(0);
				setAnimY(0);
				SDL_WarpMouse(0, 170);
				dontMove = TRUE;
				display = TRUE;
				isBlinking = FALSE;
				makeInvincible(1500);
				setTextureState("Enter");
				break;

			case DEAD:
				setAnimX(0);
				setAnimY(0);
				dontMove = TRUE;
				makeInvincible(4000);
				isBlinking = FALSE;
				isFiring = FALSE;
				setTextureState("Dead");
				break;

			case EXITING:
				setAnimX(0);
				setAnimY(0);
				dontMove = TRUE;
				makeInvincible(100000);
				display = TRUE;
				isFiring = FALSE;
				isBlinking = FALSE;
				break;
		
			default:
				break;
		}
		heroChangedState = FALSE;
	}
	else
	{
		if(updateAnimationFrame())
		{
			switch(state)
			{
				case STATIC:
					break;

				case DISPAR:
					if (currentEffect.empty() && textureState != "atomDispar")
						heroChangedState = TRUE;

					if (currentFrame == 0 && textureState == "atomDispar")
					{

						state = CURSOR;
						heroChangedState = TRUE;
					}
					break;

				case CURSOR:
					if (endTeleport())
					{
						state = APPAR;
						heroChangedState = TRUE;
					}
					break;

				case APPAR:
					if (currentFrame == 25)
					{
						dontMove = FALSE;
					}
					if (currentFrame == 0)
					{
						state = STATIC;
						heroChangedState = TRUE;
						startEffect("Appar");
					}
					break;

				case HIT:
					hitBackoff();
					if (currentFrame == 0)
					{
						state = STATIC;
						heroChangedState = TRUE;
						startEffect("Hit");
					}
					break;

				case ENTER:
					if (currentFrame == 0)
					{
						state = STATIC;
						heroChangedState = TRUE;
					}
					break;

				case DEAD:
					if (currentFrame == 0)
					{
						loseLife();
						setTextureState("");
					}
					break;


				case EXITING:
					//Make the ship exit to the right
					posX = posX + 12;

					//Make the ship go toward the middle of the screen
					if(posY > GAMEZONE_HEIGHT/2 - this->height/2 - 3)
					{
						posY = posY - 4;
					}
					if(posY < GAMEZONE_HEIGHT/2 - this->height/2 + 3)
					{
						posY = posY + 4;
					}
					break;

				default:
					break;
			}
		}
	}

	//We want the effect to stop on a weakly lighted frame
	//So we play between condition
	if(isFiring || firingEffect->display)
	{
		//We started firing
		if (isFiring && (!firingEffect->display))
		{
			firingEffect->display = TRUE;
			lev->soundEngine->playSound("mitAttack");
		}

		//Update position to match with the one of the ship
		firingEffect->posX = posX + 32;
		firingEffect->posY = posY - 32;
		
		lev->soundEngine->playSound("mitLoop");
		int updated = firingEffect->updateAnimationFrame();

		//if(firingEffect->getAnimX() == 0)
		//	lev->soundEngine->playSound("mitLoop");

		//We stopped firing and now we want to stop on a correct frame
		//We stop when we are back to the strongest lighted
		//but not if this the current frame (hence the updated condition)
		if ((!isFiring) && updated && (((int)(firingEffect->getAnimX()/firingEffect->width) % 3) == 0))
		{
			firingEffect->display = FALSE;
			lev->soundEngine->stopSound("mitLoop");
			lev->soundEngine->playSound("mitRelease");
		}

		firingEffect->processDisplay();
	}

	massPotential = min(64.0f, massPotential + regenMassPo);

	currentWeapon->animateLasers();
	if (!currentEffect.empty())
	{
		playEffect();
	}
	animateLasers();
	if(state!=DEAD)
		checkInvicibility();

	processDisplay();
}

void Hero::fire()
{
	if(state!=CURSOR)
	{
		currentWeapon->fire(this);
		isFiring = currentWeapon->isFiring;
	}
}

list<Laser*> * Hero::getLasers()
{
	return &currentWeapon->shoots;
}


void Hero::moveUp()
{
	if(topFlag)
	{
		posY = posY - 6;
	}
}

void Hero::moveDown()
{
	if(bottomFlag)
	{
		posY = posY + 6;
	}
}

void Hero::moveLeft()
{
	if(leftFlag)
	{
		posX = posX - 6;
	}
}

void Hero::moveRight()
{
	if(rightFlag)
	{
		posX = posX + 6;
	}
}

void Hero::move(int x, int y)
{
	x = min(x, 30);
	x = max(x, -30);

	y = min(y, 16);
	y = max(y, -16);

	posX = posX + x;
	posY = posY + y;

	//Make sure we stay inside the game zone
	posX = max((int)posX, -66);
	posY = max((int)posY, -66);
	posX = min((int)posX, SCREEN_WIDTH - this->width + 66);
	posY = min((int)posY, GAMEZONE_HEIGHT - this->height + 66);
}

void Hero::animateLasers()
{
	currentWeapon->animateLasers();

}

void Hero::loseLife()
{
	nbLife --;
	if (nbLife < 0)
	{
		cout<<"GAME OVER!!!\n";
		lev->levelState = LEVEL_LOST;
	}
	else
	{
		posX = 0;
		posY = 170;
		state = ENTER;
		heroChangedState = TRUE;
		display = FALSE;
		health = 4;
		canFire = 1;
		heroMovingUpOrDown = 0;
		topFlag = leftFlag = bottomFlag = rightFlag = dontMove = 0;
	}
}

void Hero::processCollisionWith(Drawable* aDrawable)
{
	if(!invincible)
	{
		if(aDrawable->isEnemy())
		{
			lev->soundEngine->playSound("tie_explode");
			//lev->createExplosion(this->posX-this->width/2, this->posY - this->height, TIE_EXPL);
			this->toRemove = TRUE;
			state = DEAD;
			heroChangedState = TRUE;
			return;
		}

		if (aDrawable->isLaser())
		{
			Bullet * aLaser = dynamic_cast<Bullet*>(aDrawable);
			health--;//= life - aLaser->power;
			if (health<=0)
			{
				//lev->createExplosion(this->posX-this->width/2, this->posY - this->height, TIE_EXPL);
				lev->soundEngine->playSound("tie_explode");
				state = DEAD;
				heroChangedState = TRUE;
			}
			else
			{
				lev->soundEngine->playSound("tie_hit");
				setState(HIT);
				hitAngle = aLaser->angle;
			}
			return;
		}
	}

	if (aDrawable->isBonus())
	{
		Bonus * aBonus = static_cast<Bonus*>(aDrawable);
		if (aBonus->type == BONUS_LIFE)
		{
			if(health < maxHealth)
			{
				health++;
			}
		}
		if (aBonus->type == BONUS_FIRERATE)
		{
			radioactivePotential = min(64.0f, radioactivePotential + 32);
		}
		return;
	}

	return;
}

//Perform a backing move when hit
void Hero::hitBackoff()
{
	float vx, vy;
	//Speed == 7
	vx = 7*cos(hitAngle);
	vy = 7*sin(hitAngle);

	posX = posX + vx;
	posY = posY + vy;

	//Make sure we stay inside the game zone
	posX = max((int)posX, 0);
	posY = max((int)posY, 0);
	posX = min((int)posX, SCREEN_WIDTH - this->width);
	posY = min((int)posY, GAMEZONE_HEIGHT - this->height);
}

void Hero::makeInvincible(int aTime)
{
	invincibilityTime = aTime;
	startInvincibility = GameTimer;
	invincible = TRUE;
	//isBlinking = TRUE;
}

void Hero::checkInvicibility()
{
	if (startInvincibility + invincibilityTime < GameTimer)
	{
		invincible = FALSE;
		display = TRUE;
		isBlinking = FALSE;
	}
}

void Hero::setState(int aState)
{
	state = aState;
	heroChangedState = TRUE;
}

void  Hero::teleport()
{
	if(massPotential == 64)
	{
		setState(DISPAR);
		startEffect("Dispar");
		massPotential = 0;
	}
}

int  Hero::endTeleport()
{
	unsigned int endTeleportTime = startTeleporting + 2000;
	if (endTeleportTime < GameTimer)
	{
		return TRUE;
	}
	return FALSE;
}

void Hero::startEffect(string anEffect)
{
	effect->setTextureState(anEffect);
	effect->posX = posX;
	effect->posY = posY;

	currentEffect = anEffect;
}

void Hero::playEffect()
{
	effect->posX = posX;
	effect->posY = posY;
	effect->updateAnimationFrame();

	if(effect->getAnimX() == 0)
	{
		currentEffect = "";
		return;
	}

	effect->processDisplay();
}
