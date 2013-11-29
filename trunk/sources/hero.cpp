/*
 * Contains herodescription
 */

#include "hero.h"

Hero::Hero()
{
	health = 4;
	maxHealth = 8;
	nbLife = 5;
	posX = SCREEN_WIDTH/3 - 64;
	posY = GAMEZONE_HEIGHT/2 - 32;
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
	massPotential = 60;
	radioactivePotential = 0;
	toBlend = TRUE;
	ownedWeapons.insert(make_pair("electronGun", (new Weapon("electronGun", 50, 40, WEAPON_ELECTRON))));
	ownedWeapons.insert(make_pair("photonGun", (new Weapon("photonGun", 250, 750, WEAPON_PHOTON))));
	currentWeapon = ownedWeapons.at("photonGun");
}

void Hero::setTexture(Drawable * levelHero)
{
	copyFrom(lev->loadedObjects.at("atom"));

	firingEffect = new AnimatedDrawable();
	firingEffect->copyFrom(lev->loadedObjects.at("muzzl"));
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
	posY = GAMEZONE_HEIGHT/2 - width/2;
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
}

void Hero::animate()
{
	//If we have changed state between two calls to this function
	if (heroChangedState)
	{
		dontMove = FALSE;
		switch(state)
		{
			case STATIC:
				setAnimation("static");
				setAnimX(0);
				setAnimY(0);
				break;

			case DISPAR:
				dontMove = TRUE;
				isFiring = FALSE;
				makeInvincible(4000);
				startEffect("Dispar");
				currentAnimation->currentFrame = 0;
				//setAnimation("Dispar");
				lev->soundEngine->playSound("TeleportOff");
				setAnimX(0);
				setAnimY(0);
				break;

			case CURSOR:
				opacity = 1.0;
				display = FALSE;
				setAnimation("Cursor");
				makeInvincible(3000);
				startTeleporting = GameTimer;
				dontMove = TRUE;
				setAnimX(0);
				setAnimY(0);
				break;

			case APPAR:
				display = TRUE;
				setAnimation("Appar");
				lev->soundEngine->playSound("TeleportOn");
				dontMove = TRUE;
				makeInvincible(4000);
				setAnimX(0);
				setAnimY(0);
				break;

			case HIT:
				setAnimation("Hit");
				startEffect("Hit");
				dontMove = TRUE;
				backOffSpeed = 13.0;
				makeInvincible(2000);
				setAnimX(0);
				setAnimY(0);
				break;
		
			case ENTER:
				SDL_WarpMouse(SCREEN_WIDTH/3 - width, GAMEZONE_HEIGHT/2 - height/2);
				dontMove = TRUE;
				display = TRUE;
				isBlinking = FALSE;
				makeInvincible(1500);
				setAnimation("Enter");
				setAnimX(0);
				setAnimY(0);
				break;

			case DEAD:
				setAnimX(0);
				setAnimY(0);
				dontMove = TRUE;
				makeInvincible(4000);
				isBlinking = FALSE;
				isFiring = FALSE;
				display = FALSE;
				setAnimation("Dead");
				startEffect("Dead");
				break;

			case EXITING:
				setAnimX(0);
				setAnimY(0);
				dontMove = TRUE;
				makeInvincible(100000);
				setAnimation("Exiting");
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
					if(currentAnimation->currentFrame == 0 || currentAnimation->currentFrame == 2 || currentAnimation->currentFrame == 4 || currentAnimation->currentFrame == 6)
						opacity = 0;
					if(currentAnimation->currentFrame == 1)
						opacity = 1.0;
					if(currentAnimation->currentFrame == 3)
						opacity = 0.75;
					if(currentAnimation->currentFrame == 5)
						opacity = 0.5;

					if (currentAnimation->currentFrame == 14)
					{
						state = CURSOR;
						heroChangedState = TRUE;
					}
					break;

				case CURSOR:
					if (endTeleport())
					{
						posX = 20 + (rand() % (SCREEN_WIDTH - width - 20));
						posY = 20 + (rand() % (GAMEZONE_HEIGHT - height - 20));
						state = APPAR;
						heroChangedState = TRUE;
					}
					break;

				case APPAR:
					if (currentAnimation->hasEnded)
					{
						state = STATIC;
						heroChangedState = TRUE;
						startEffect("Appar");
					}
					break;

				case HIT:
					if (hitBackoff() == 0)
					{
						dontMove = FALSE;
					}
					if (currentAnimation->hasEnded)
					{
						state = STATIC;
						heroChangedState = TRUE;
					}
					break;

				case ENTER:
					if (currentAnimation->hasEnded)
					{
						state = STATIC;
						heroChangedState = TRUE;
					}
					break;

				case DEAD:
					if (currentAnimation->hasEnded)
					{
						loseLife();
						display = TRUE;
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
	if((isFiring || firingEffect->display) && (!currentWeapon->name.compare("electronGun")))
	{
		//We started firing
		if (isFiring && (!firingEffect->display))
		{
			firingEffect->display = TRUE;
			lev->soundEngine->playSound("mitAttack");
		}

		//Update position to match with the one of the ship
		firingEffect->posX = posX - 10;
		firingEffect->posY = posY - 16;
		
		lev->soundEngine->playSound("mitLoop");
		int updated = firingEffect->updateAnimationFrame();

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

void Hero::fireWeapon(string aWeaponName)
{
	if(state!=CURSOR)
	{
		ownedWeapons.at(aWeaponName)->fire(this);	//currentWeapon->fire(this);
		isFiring = ownedWeapons.at(aWeaponName)->isFiring;
	}
}

list<Laser*> * Hero::getLasers()
{
	shoots.clear();
	for(map<string, Weapon *>::iterator aWeapon = ownedWeapons.begin(); aWeapon != ownedWeapons.end(); ++aWeapon)
	{
		shoots.insert(shoots.begin(), (*aWeapon).second->shoots.begin(), (*aWeapon).second->shoots.end());
	}

	return &shoots;
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
	posX = max((int)posX,0);
	posY = max((int)posY,0);
	posX = min((int)posX, SCREEN_WIDTH - this->width);
	posY = min((int)posY, GAMEZONE_HEIGHT - this->height);
}

void Hero::animateLasers()
{
	for(map<string, Weapon *>::iterator aWeapon = ownedWeapons.begin(); aWeapon != ownedWeapons.end(); ++aWeapon)
	{
		(*aWeapon).second->animateLasers();
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
		posX = SCREEN_WIDTH/3 - width;
		posY = GAMEZONE_HEIGHT/2 - height/2;
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
				lev->soundEngine->playSound("tie_explode");
				state = DEAD;
				heroChangedState = TRUE;
			}
			else
			{
				lev->soundEngine->playSound("AtomHit");
				setState(HIT);
				hitAngle = aLaser->angle;
			}
			return;
		}
	}

	if (aDrawable->isBonus())
	{
		Bonus * aBonus = dynamic_cast<Bonus*>(aDrawable);
		if (aBonus->type == BONUS_LIFE)
		{
			startEffect("loadHealth");

			if(health < maxHealth)
			{
				health++;
			}
			if (health >= maxHealth)
			{
				lev->soundEngine->playSound("UpHealth100");
			}
			else
			{
				lev->soundEngine->playSound("UpHealth");
			}
		}
		if (aBonus->type == BONUS_FIRERATE)
		{
			startEffect("loadRadio");
			lev->soundEngine->playSound("UpDiamond");
			radioactivePotential = min(64.0f, radioactivePotential + 32);
		}
		return;
	}

	return;
}

//Perform a backing move when hit
float Hero::hitBackoff()
{
	float vx, vy;

	vx = backOffSpeed*cos(hitAngle);
	vy = backOffSpeed*sin(hitAngle);

	posX = posX + vx;
	posY = posY + vy;
	backOffSpeed = max(0.0f, backOffSpeed - 1);

	//Make sure we stay inside the game zone
	posX = max((int)posX, 0);
	posY = max((int)posY, 0);
	posX = min((int)posX, SCREEN_WIDTH - this->width);
	posY = min((int)posY, GAMEZONE_HEIGHT - this->height);

	return backOffSpeed;
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

void Hero::setWeapon(string aWeaponName)
{
	currentWeapon = ownedWeapons.at(aWeaponName);

	if(aWeaponName.compare("electronGun") && lev->soundEngine->sounds.at("mitLoop")->isPlaying)
	{
		lev->soundEngine->stopSound("mitLoop");
		lev->soundEngine->playSound("mitRelease");
	}
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
	unsigned int endTeleportTime = startTeleporting + 1000;
	if (endTeleportTime < GameTimer)
	{
		return TRUE;
	}
	return FALSE;
}

void Hero::startEffect(string anEffect)
{
	lev->createEffect(posX, posY, anEffect);
}

