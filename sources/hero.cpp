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
	maxFireRate = 80;
	fireRate = 80;
	lastTimeFired = 0;
	collision =  ge->loadTexture("res/Joueur_Col.png");
	heroMovingUpOrDown = 0;
	topFlag = leftFlag = bottomFlag = rightFlag = dontMove = 0;
	isFiring = FALSE;
}

void Hero::setTexture()
{
	addTexture("atom");
	width = atoi(((lev->configurationElements.at("atom")).at(0)).c_str());
	height = atoi(((lev->configurationElements.at("atom")).at(1)).c_str());
	nbFrames = parseAnimationState((lev->configurationElements.at("atom")).at(2));
	setAnimX(0);

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
	//firingEffect->animationUpdateFrequency = 33;
}


void Hero::resetHero()
{
	if(health < 4)
		health = 4;

	posX = 0;
	posY = 170;
	state = ENTER;
	//setAnimX(0);
	invincible = FALSE;
	invincibilityTime = 1250;
	heroChangedState = TRUE;
	canFire = 1;
	lastTimeFired = 0;
	heroMovingUpOrDown = 0;
	topFlag = leftFlag = bottomFlag = rightFlag = dontMove = 0;
	isFiring = FALSE;
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
				setAnimX(0);
				setAnimY(STATIC * height);
				break;

			case GO_UP:
				setAnimX(0);
				setAnimY(STATIC * height);
				break;

			case GO_DOWN:
				setAnimX(0);
				setAnimY(STATIC * height);
				break;

			case HIT:
				setAnimX(0);
				dontMove = TRUE;
				setAnimY(HIT * height);
				makeInvincible();
				break;
		
			case ENTER:
				setAnimX(0);
				setAnimY(STATIC * height);
				dontMove = TRUE;
				display = TRUE;
				makeInvincible();
				SDL_WarpMouse(0, 170);
				break;

			case DEAD:
				setAnimX(0);
				setAnimY(STATIC * height);
				dontMove = TRUE;
				makeInvincible();
				isBlinking = FALSE;
				isFiring = FALSE;
				display = FALSE;
				break;

			case EXITING:
				setAnimX(0);
				setAnimY(STATIC * height);
				dontMove = TRUE;
				invincibilityTime = 100000;
				makeInvincible();
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

				case GO_UP:
					if (getAnimX() == 0)
						{state = STATIC;
						heroChangedState = TRUE;}
					break;

				case GO_DOWN:
					if (getAnimX() == 0)
						{state = STATIC;
						heroChangedState = TRUE;}
					break;

				case HIT:
					posX = max(0, (int)posX - 9);
					if (getAnimX() == 0)
					{
						state = STATIC;
						heroChangedState = TRUE;
					}
					break;

				case ENTER:
					posX = posX + 6;
					if (getAnimX() == 0)
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
		firingEffect->posX = posX + this->width/2 - 8;
		firingEffect->posY = posY;
		
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

	animateLasers();
	if(state!=DEAD)
		checkInvicibility();

	processDisplay();
}

void Hero::fire()
{
	isFiring = TRUE;
	checkFire();
	if (canFire)
	{
		
		cout<<"piou piou!\n";
		lasers.push_back(new Laser(posX, posY + 32, RIGHT, GREEN_LASER));
		canFire = 0;
	}
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
	posX = max((int)posX, 0);
	posY = max((int)posY, 0);
	posX = min((int)posX, SCREEN_WIDTH - this->width);
	posY = min((int)posY, GAMEZONE_HEIGHT - this->height);
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
			(*aLaser)->processDisplay();
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
		display = FALSE;
		health = 4;
		fireRate = 80;
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
