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
	animX = 0;
	invincible = FALSE;
	invincibilityTime = 1250;
	heroChangedState = TRUE;
	canFire = 1;
	maxFireRate = 200;
	fireRate = 500;
	lastTimeFired = 0;
	heroMovingUpOrDown = 0;
	topFlag = leftFlag = bottomFlag = rightFlag = dontMove = 0;
}

void Hero::resetHero()
{
	if(health < 4)
		health = 4;

	posX = 0;
	posY = 170;
	state = ENTER;
	animX = 0;
	invincible = FALSE;
	invincibilityTime = 1250;
	heroChangedState = TRUE;
	canFire = 1;
	lastTimeFired = 0;
	heroMovingUpOrDown = 0;
	topFlag = leftFlag = bottomFlag = rightFlag = dontMove = 0;
	lasers.clear();
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
				animX = 0;
				animY = STATIC * height;
				break;
		
			case GO_UP:
				animX = 0;
				animY = GO_UP * height;
				break;
		
			case GO_DOWN:
				animX = 0;
				animY = GO_DOWN * height;
				break;
		
			case HIT:
				animX = 0;
				animY = HIT * height;
				makeInvincible();
				break;
		
			case ENTER:
				animX = 0;
				animY = ENTER * height;
				dontMove = TRUE;
				makeInvincible();
				break;

			case DEAD:
				animX = width - 1;
				animY = STATIC * height;
				dontMove = TRUE;
				makeInvincible();
				break;

			case EXITING:
				animX = 0;
				animY = STATIC * height;
				dontMove = TRUE;
				invincibilityTime = 100000;
				makeInvincible();
				display = TRUE;
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

				case GO_UP:
					if (animX == 0)
						{state = STATIC;
						heroChangedState = TRUE;}
					break;

				case GO_DOWN:
					if (animX == 0)
						{state = STATIC;
						heroChangedState = TRUE;}
					break;

				case HIT:
					if (animX == 0)
					{
						state = STATIC;
						heroChangedState = TRUE;
					}
					break;

				case ENTER:
					posX = posX + 6;
					if (animX == 0)
					{
						state = STATIC;
						heroChangedState = TRUE;
					}
					break;

				case EXITING:
					//Make the ship exit to the right
					posX = posX + 12;

					//make the ship go toward the middle of the screen
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
	animateLasers();
	checkInvicibility();
	processDisplay();
}

void Hero::fire()
{
	checkFire();
	if (canFire)
	{
		lev->soundEngine->playSound("tie_fire");
		cout<<"piou piou!\n";
		lasers.push_back(new Laser(posX + width/2, posY + height/4, RIGHT, GREEN_LASER));
		canFire = 0;
	}
}

void Hero::moveUp()
{
	if(topFlag)
	{
		if(!heroMovingUpOrDown)
		{
			state = GO_UP;
			heroChangedState = TRUE;
			heroMovingUpOrDown = 1;
		}
		posY = posY - 6;
	}
}

void Hero::moveDown()
{
	if(bottomFlag)
	{
		if(!heroMovingUpOrDown)
		{
			state = GO_DOWN;
			heroChangedState = TRUE;
			heroMovingUpOrDown = 1;
		}
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

void Hero::animateLasers()
{
	for (std::list<Laser *>::iterator aLaser = lasers.begin(); aLaser != lasers.end(); ++aLaser)
	{
		(*aLaser)->animate();
		if ((*aLaser)->posX > SCREEN_WIDTH + (*aLaser)->width || (*aLaser)->toRemove)
		{
			lasers.erase(aLaser++);
		}
		else
		{
			ge->toDisplay.push_back(*aLaser);
		}
	}
}

void Hero::checkFire()
{
	unsigned int nextFireTime = lastTimeFired + fireRate;
	if (nextFireTime < GameTimer && state !=DEAD)
	{
		canFire = 1;
		lastTimeFired = GameTimer;
	}
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
		posX = -width;
		posY = 170;
		state = ENTER;
		heroChangedState = TRUE;
		health = 4;
		fireRate = 500;
		canFire = 1;
		lastTimeFired = 0;
		heroMovingUpOrDown = 0;
		topFlag = leftFlag = bottomFlag = rightFlag = dontMove = 0;
		lasers.clear();
	}
}

void Hero::processCollisionWith(Drawable* aDrawable)
{
	if(!invincible)
	{
		if(aDrawable->isEnemy())
		{
			lev->soundEngine->playSound("tie_explode");
			lev->createExplosion(this->posX-this->width/2, this->posY - this->height, TIE_EXPL);
			this->toRemove = TRUE;
			state = DEAD;
			heroChangedState = TRUE;
			return;
		}

		if (aDrawable->isLaser())
		{
			Laser * aLaser = static_cast<Laser*>(aDrawable);
			health--;//= life - aLaser->power;
			if (health<=0)
			{
				lev->createExplosion(this->posX-this->width/2, this->posY - this->height, TIE_EXPL);
				lev->soundEngine->playSound("tie_explode");
				state = DEAD;
				heroChangedState = TRUE;
			}
			else
			{
				lev->soundEngine->playSound("tie_hit");
				lev->createExplosion(aLaser->posX-aLaser->width, aLaser->posY-this->height, SPARK);
				state = HIT;
				makeInvincible();
				heroChangedState = TRUE;
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
			fireRate = max(fireRate - 100, maxFireRate);
		}
		return;
	}
	return;
}

void Hero::makeInvincible()
{
	startInvincibility = GameTimer;
	invincible = TRUE;
	isBlinking = TRUE;
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
