/*
 * Contains herodescription
 */

#include "hero.h"

int heroChangedState;

Hero::Hero() {
	life = 4;
	maxHealth = 8;
	nbLife = 50;
	texture = ge->textures.at("tie");
	width = atoi(((lev->configurationElements.at("tie")).at(0)).c_str());
	height = atoi(((lev->configurationElements.at("tie")).at(1)).c_str());
	posX = 0;
	posY = 170;
	state = ENTER;
	animX = 0;
	invincible = FALSE;
	heroChangedState = TRUE;
	canFire = 1;
	maxFireRate = 200;
	fireRate = 500;
	lastTimeFired = 0;
	heroMovingUpOrDown = 0;
	topFlag = leftFlag = bottomFlag = rightFlag = dontMove = 0;
	nbFrames = parseAnimationState((lev->configurationElements.at("tie")).at(2));
}

void Hero::animate()
{
	//If we have changed state between two calls to this function
	//TODO can we do something more generic ? state loaded through a textfile
	if (heroChangedState)
	{
		invincible = FALSE;
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
				invincible = TRUE;
				break;
		
			case ENTER:
				animX = 0;
				animY = ENTER * height;
				dontMove = TRUE;
				invincible = TRUE;
				break;

			case DEAD:
				animX = width -1;
				animY = STATIC * height;
				dontMove = TRUE;
				invincible = TRUE;
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
						{state = STATIC;
						heroChangedState = TRUE;
						invincible = FALSE;}
					break;

				case ENTER:
					posX = posX + 6;
					if (animX == 0)
					{
						state = STATIC;
						heroChangedState = TRUE;
						invincible = FALSE;
					}
					break;

				default:
					break;
			}
		}
	}
	animateLasers();
	ge->toDisplay.push_back(this);
}

void Hero::fire()
{
	checkFire();
	if (canFire)
	{
		lev->soundEngine->PlaySound("sound/tie_fire.wav");
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
	unsigned int thing = lastTimeFired + fireRate;
	if (thing <  SDL_GetTicks() && state !=DEAD)
	{
		canFire = 1;
		lastTimeFired = SDL_GetTicks();
	}
}

void Hero::loseLife()
{
	if (nbLife <  0)
	{
		cout<<"GAME OVER!!!\n";
	}
	else
	{
		posX = -width;
		posY = 170;
		state = ENTER;
		heroChangedState = TRUE;
		nbLife --;
		life = 4;
	}
}

void Hero::processCollisionWith(Drawable* aDrawable)
{
	if(!invincible)
	{
		if(aDrawable->isEnemy())
		{
			lev->createExplosion(this->posX-this->width/2, this->posY - this->height, TIE_EXPL);
			this->toRemove = TRUE;
			state = DEAD;
			heroChangedState = TRUE;
			return;
		}

		if (aDrawable->isLaser())
		{
			Laser * aLaser = static_cast<Laser*>(aDrawable);
			life--;//= life - aLaser->power;
			if (life<=0)
			{
				lev->createExplosion(this->posX-this->width/2, this->posY - this->height, TIE_EXPL);
				state = DEAD;
				heroChangedState = TRUE;
			}
			else
			{
				lev->createExplosion(aLaser->posX-aLaser->width, aLaser->posY-this->height, SPARK);
				state = HIT;
				heroChangedState = TRUE;
			}
		return;
		}
	}

	if (aDrawable->isBonus())
	{
		Bonus * aBonus = static_cast<Bonus*>(aDrawable);
		if (aBonus->type == BLIFE)
		{
			if(life < maxHealth)
			{
				life++;// = life + 100;
			}
		}
		if (aBonus->type == BFIRERATE)
		{
			fireRate = max(fireRate - 100, maxFireRate);
		}
		return;
	}
	return;
}

void Hero::checkInvicibility()
{
	if (startInvincibility + invincibilityTime > gameTimer)
	{
		invincible = FALSE;
		display = TRUE;
	}
	else
	{
		//Blinking effect showing that we are invincible
		display = display + TRUE % 2;
	}
}
