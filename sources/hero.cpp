/*
 * Contains herodescription
 */

#include "hero.h"

Hero::Hero()
{
	name = "hero";
	health = 10;
	maxHealth = 10;
	maxLife = 6;
	nbLife = 3;
	posX = SCREEN_WIDTH/3 - 64;
	posY = GAMEZONE_HEIGHT/2 - 32;
	state = ENTER;
	invincible = FALSE;
	invincibilityTime = 1500;
	heroChangedState = TRUE;
	canFire = 1;
	heroMovingUpOrDown = 0;
	topFlag = leftFlag = bottomFlag = rightFlag = dontMove = 0;
	isFiring = FALSE;
	regenMassPo = 0.1;
	massPotential = 60;
	radioactivePotential = 0;
	toBlend = TRUE;
	backOffSpeed = 0;
	startTeleporting = 0;
	startInvincibility = 0;
	hitAngle = 0;
	shielded = FALSE;

	quarkLevels[QuarkB] = 0;
	quarkLevels[QuarkT] = 0;
	quarkLevels[QuarkU] = 0;
	quarkLevels[QuarkD] = 0;
	quarkLevels[QuarkC] = 0;
	quarkLevels[QuarkS] = 0;
}

void Hero::setTexture(Drawable * levelHero)
{
	copyFrom(lev->loadedObjects.at("atom"));
	posX = SCREEN_WIDTH/3 - 64;
	posY = GAMEZONE_HEIGHT/2 - 32;
	ownedWeapons.insert(make_pair("electronGun", new Electron()));
	ownedWeapons.insert(make_pair("hadronGun", new Hadron()));
	ownedWeapons.insert(make_pair("baryonGun", new Baryon()));
	ownedWeapons.insert(make_pair("plasmaGun", new Plasma()));
	currentWeapon = ownedWeapons.at("electronGun");
}


void Hero::resetHero()
{
	health = 10;
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
				setAnimation("animDefaut");
				setAnimX(0);
				setAnimY(0);
				break;

			case DISPAR:
				dontMove = TRUE;
				isFiring = FALSE;
				makeInvincible(4000);
				startEffect("dispar");
				currentAnimation->currentFrame = 0;
				lev->soundEngine->playSound("TeleportOff");
				setAnimX(0);
				setAnimY(0);
				break;

			case CURSOR:
				opacity = 1.0;
				display = FALSE;
				setAnimation("cursor");
				makeInvincible(3000);
				startTeleporting = GameTimer;
				dontMove = TRUE;
				setAnimX(0);
				setAnimY(0);
				break;

			case APPAR:
				display = TRUE;
				setAnimation("appar");
				lev->soundEngine->playSound("TeleportOn");
				dontMove = TRUE;
				makeInvincible(4000);
				setAnimX(0);
				setAnimY(0);
				break;

			case HIT:
				setAnimation("hit");
				startEffect("hit");
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
				setAnimation("enter");
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
				spreadQuarks();
				setAnimation("dead");
				startEffect("dead");
				break;

			case EXITING:
				setAnimX(0);
				setAnimY(0);
				dontMove = TRUE;
				makeInvincible(10000);
				//setAnimation("exiting");
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
						startEffect("appar");
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
					if(posY > GAMEZONE_HEIGHT/2 - height/2 - 3)
					{
						posY = posY - 4;
					}
					if(posY < GAMEZONE_HEIGHT/2 - height/2 + 3)
					{
						posY = posY + 4;
					}
					break;

				default:
					break;
			}
		}
	}

/*	if(isFiring && (!currentWeapon->name.compare("electronGun")))
	{
		//We started firing
		if (!lev->soundEngine->sounds.at("mitAttack")->isPlaying && !lev->soundEngine->sounds.at("mitLoop")->isPlaying)
		{
			lev->soundEngine->playSound("mitAttack");
		}
		
		lev->soundEngine->playSound("mitLoop");
	}*/

	if (!isFiring && lev->soundEngine->sounds.at("mitLoop")->isPlaying)
	{
		lev->soundEngine->stopSound("mitLoop");
		lev->soundEngine->playSound("mitRelease");
	}

	massPotential = min(64.0f, massPotential + regenMassPo);

	animateLasers();

	if(state!=DEAD)
		checkInvicibility();

	if(shielded)
	{
		shield->updateAnimationFrame();
		shield->posX = posX - 16;
		shield->posY = posY - 16;
		shield->processDisplay();
		if(shield->energy == 1)
		{
			shield->display = rand() % 2;
		}
	}

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
	if(state != CURSOR && (ownedWeapons.find(aWeaponName) != ownedWeapons.end()))
	{
		ownedWeapons.at(aWeaponName)->fire(this);	//currentWeapon->fire(this);
		isFiring = ownedWeapons.at(aWeaponName)->isFiring;
	}
}

void Hero::upgradeWeaponTo(int aLevel)
{
	currentWeapon->upgradeTo(aLevel);
}

list<Shoot *> * Hero::getLasers()
{
	shoots.clear();
	//Get all the currently active ammunitions from all the weapons
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
	//Bound the move by the maximum possible value
	x = min(x, 30);
	x = max(x, -30);

	y = min(y, 30);
	y = max(y, -30);

	//And move
	posX = posX + x;
	posY = posY + y;

	//Make sure we stay inside the game zone
	posX = max((int)posX, 0);
	posY = max((int)posY, 0);
	posX = min((int)posX, SCREEN_WIDTH - width);
	posY = min((int)posY, GAMEZONE_HEIGHT - height);
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
		health = 10;
		canFire = 1;
		heroMovingUpOrDown = 0;
		topFlag = leftFlag = bottomFlag = rightFlag = dontMove = 0;
	}
}

void Hero::processCollisionWith(Drawable * aDrawable)
{
	if(!invincible)
	{
		if(aDrawable->isEnemy())
		{
			if (!shielded)
			{
				lev->soundEngine->playSound("tie_explode");
				toRemove = TRUE;
				state = DEAD;
				heroChangedState = TRUE;
				return;
			}
			else
			{
				shielded = FALSE;
				delete shield;
			}
		}

		if (aDrawable->isLaser())
		{
			Shoot * aShoot = dynamic_cast<Shoot*>(aDrawable);
			if (!shielded)
			{
				health--;//= health - aLaser->power;
				if (health<=0)
				{
					lev->soundEngine->playSound("tie_explode");
					setState(DEAD);
				}
				else
				{
					lev->soundEngine->playSound("AtomHit");
					setState(HIT);
					hitAngle = aShoot->angle;
				}
			}
			else
			{
				shield->energy--;
				makeInvincible(400);
				if(shield->energy <= 0)
				{
					shielded = FALSE;
					delete shield;
				}
				else
				{
					shield->opacity = 1.0f / (3.0f/shield->energy);
				}
			}
			return;
		}
	}

	if (aDrawable->isBonus())
	{
		Bonus * aBonus = dynamic_cast<Bonus*>(aDrawable);
		stringstream stringToDisplay;
		switch(aBonus->type)
		{
			case BONUS_LIFE_BIG:
			case BONUS_LIFE_SMALL:
			case BONUS_LIFE_MEDIUM:
				stringToDisplay << "Sante : +" << aBonus->quantity;
				lev->createTextEffect(posX, posY, stringToDisplay.str());

				health = min(maxHealth, health + aBonus->quantity);

				if (health >= maxHealth)
				{
					lev->soundEngine->playSound("UpHealth100");
				}
				else
				{
					lev->soundEngine->playSound("UpHealth");
				}
				break;

			case BONUS_SHIELD:
				lev->createTextEffect(posX, posY, "Bouclier!");
				if(shielded)
					delete shield;

				shield = new Shield(posX, posY);
				shielded = TRUE;
				break;

			default:
				break;
		}

		if (aBonus->isQuarkBonus)
		{
			stringToDisplay <<  "Quark : +" << aBonus->quantity;
			lev->createTextEffect(posX, posY,  stringToDisplay.str());
			lev->soundEngine->playSound("UpDiamond");
			quarkLevels.at(aBonus->quarkType) = min(34, quarkLevels.at(aBonus->quarkType) + aBonus->quantity);
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

	vx = backOffSpeed * cos(hitAngle);
	vy = backOffSpeed * sin(hitAngle);

	posX = posX + vx;
	posY = posY + vy;
	backOffSpeed = max(0.0f, backOffSpeed - 1);

	//Make sure we stay inside the game zone
	posX = max((int)posX, 0);
	posY = max((int)posY, 0);
	posX = min((int)posX, SCREEN_WIDTH - width);
	posY = min((int)posY, GAMEZONE_HEIGHT - height);

	return backOffSpeed;
}

void Hero::makeInvincible(int aTime)
{
	invincibilityTime = aTime;
	startInvincibility = GameTimer;
	invincible = TRUE;
}

void Hero::checkInvicibility()
{
	if (startInvincibility + invincibilityTime < GameTimer)
	{
		invincible = FALSE;
		display = TRUE;
	}
}

void Hero::setState(int aState)
{
	state = aState;
	heroChangedState = TRUE;
}

void Hero::setWeapon(string aWeaponName)
{
	if (ownedWeapons.find(aWeaponName)!= ownedWeapons.end())
		currentWeapon = ownedWeapons.at(aWeaponName);

	if(aWeaponName.compare("electronGun") && lev->soundEngine->sounds.at("mitLoop")->isPlaying)
	{
		lev->soundEngine->stopSound("mitLoop");
		lev->soundEngine->playSound("mitRelease");
	}
}


void Hero::teleport()
{
	if(massPotential == 64)
	{
		setState(DISPAR);
		startEffect("dispar");
		massPotential = 0;
	}
}

int Hero::endTeleport()
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

void Hero::spreadQuarks()
{
	int numberOfGold;
	float aSpeed, anAngle;
	int i;

	for(map<int, int>::iterator aQuark = quarkLevels.begin(); aQuark != quarkLevels.end(); ++aQuark)
	{
		numberOfGold = ((*aQuark).second / 5);

		for(i = 0; i < numberOfGold; i++)
		{
			aSpeed = 10.5f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/10.0f));
			anAngle = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/360.0f)) * (PI/180);
			lev->createBonus(posX, posY, aSpeed, anAngle, ((*aQuark).first * 2) + 2);
		}

		/*numberOfSilver = ((*aQuark).second % 5);
		for(i = 0; i < numberOfSilver; i++)
		{
			aSpeed = 10.5f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/10.0f));
			anAngle = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/360.0f)) * (PI/180);
			lev->createBonus(posX, posY, aSpeed, anAngle, ((*aQuark).first * 2) + 1);
		}*/

		(*aQuark).second = 0;
	}
}

/*
 * Shield function
 */
Shield::Shield()
{
	energy = 3;
	copyFrom(lev->loadedObjects.at("jSp_Bouclier"));
}

Shield::Shield(float x, float y)
{
	energy = 3;
	copyFrom(lev->loadedObjects.at("jSp_Bouclier"));
	posX = x - 16;
	posY = y - 16;
	setAnimX(0);
	setAnimY(0);
}
