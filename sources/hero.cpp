/*
 * Contains herodescription
 */

#include "include/hero.h"

Hero::Hero()
{
	name = "hero";
	health = 10;
	maxHealth = 10;
	maxLife = 6;
	nbLife = 3;
	posX = SCREEN_WIDTH/3;
	posY = GAMEZONE_HEIGHT/2;
	state = ENTER;
	invincible = false;
	invincibilityTime = 1500;
	heroChangedState = true;
	canFire = false;
	heroMovingUpOrDown = 0;
	topFlag = leftFlag = bottomFlag = rightFlag = dontMove = 0;
	isFiring = false;
	regenMassPo = 0.1;
	massPotential = 60;
	radioactivePotential = 0;
	toBlend = true;
	backOffSpeed = 0;
	startTeleporting = 0;
	startInvincibility = 0;
	hitAngle = 0;
	shielded = false;
	currentWeapon = NULL;
	shield = NULL;

	quarkLevels[QuarkB] = 0;
	quarkLevels[QuarkT] = 0;
	quarkLevels[QuarkU] = 0;
	quarkLevels[QuarkD] = 0;
	quarkLevels[QuarkC] = 0;
	quarkLevels[QuarkS] = 0;
}

Hero::~Hero()
{
	for(map<weaponName, Weapon *>::iterator aWeapon = ownedWeapons.begin(); aWeapon != ownedWeapons.end(); ++aWeapon)
	{
		delete (*aWeapon).second;
	}

	if(shielded)
		delete shield;
}

void Hero::setTexture(Drawable * levelHero)
{
	copyFrom(CurrentLevel->loadedObjects.at("atom"));
	posX = SCREEN_WIDTH/3;
	posY = GAMEZONE_HEIGHT/2;
	ownedWeapons.insert(make_pair(ElectronGun, new Electron()));
	ownedWeapons.insert(make_pair(HadronGun, new Hadron()));
	ownedWeapons.insert(make_pair(BaryonGun, new Baryon()));
	ownedWeapons.insert(make_pair(PlasmaGun, new Plasma()));
	currentWeapon = ownedWeapons.at(ElectronGun);
}


void Hero::resetHero()
{
	health = 10;
	posX = SCREEN_WIDTH/3;
	posY = GAMEZONE_LIMIT + GAMEZONE_HEIGHT/2;
	state = ENTER;
	invincible = false;
	invincibilityTime = 1250;
	heroChangedState = true;
	canFire = false;
	heroMovingUpOrDown = 0;
	topFlag = leftFlag = bottomFlag = rightFlag = dontMove = 0;
	massPotential = 0;
	radioactivePotential = 0;
	isFiring = false;
}

void Hero::animate()
{
	//If we have changed state between two calls to this function
	if (heroChangedState)
	{
		dontMove = false;
		switch(state)
		{
			case STATIC:
				setAnimation("animDefaut");
				setAnimX(0);
				setAnimY(0);
				break;

			case DISPAR:
				dontMove = true;
				isFiring = false;
				makeInvincible(4000);
				startEffect("dispar");
				currentAnimation->currentFrame = 0;
				CurrentLevel->soundEngine->playSound("TeleportOff");
				if(shielded)
					shield->display = false;
				setAnimX(0);
				setAnimY(0);
				break;

			case CURSOR:
				opacity = 1.0;
				display = false;
				setAnimation("cursor");
				makeInvincible(3000);
				startTeleporting = GameTimer;
				dontMove = true;
				setAnimX(0);
				setAnimY(0);
				break;

			case APPAR:
				display = true;
				setAnimation("appar");
				CurrentLevel->soundEngine->playSound("TeleportOn");
				dontMove = true;
				isFiring = false;
				if(shielded)
					shield->display = true;
				makeInvincible(4000);
				setAnimX(0);
				setAnimY(0);
				break;

			case HIT:
				setAnimation("hit");
				startEffect("hit");
				dontMove = true;
				backOffSpeed = 13.0;
				isFiring = false;
				makeInvincible(2000);
				setAnimX(0);
				setAnimY(0);
				break;
		
			case ENTER:
				//SDL_WarpMouse(SCREEN_WIDTH/3, GAMEZONE_HEIGHT/2);
				dontMove = true;
				display = true;
				isBlinking = false;
				makeInvincible(1500);
				setAnimation("enter");
				setAnimX(0);
				setAnimY(0);
				break;

			case DEAD:
				setAnimX(0);
				setAnimY(0);
				dontMove = true;
				makeInvincible(4000);
				isBlinking = false;
				isFiring = false;
				display = false;
				spreadQuarks();
				setAnimation("dead");
				startEffect("dead");
				break;

			case EXITING:
				setAnimX(0);
				setAnimY(0);
				dontMove = true;
				makeInvincible(10000);
				display = true;
				isFiring = false;
				isBlinking = false;
				break;
		
			default:
				break;
		}
		heroChangedState = false;
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
						heroChangedState = true;
					}
					break;

				case CURSOR:
					if (endTeleport())
					{
						posX = width/2 + 20 + (rand() % (SCREEN_WIDTH - (int)width/2 - 20));
						posY = GAMEZONE_LIMIT + height/2 + 20 + (rand() % (GAMEZONE_HEIGHT - (int)height/2 - 20));
						state = APPAR;
						heroChangedState = true;
					}
					break;

				case APPAR:
					if (currentAnimation->hasEnded)
					{
						state = STATIC;
						heroChangedState = true;
						startEffect("appar");
					}
					break;

				case HIT:
					if (hitBackoff() == 0)
					{
						dontMove = false;
					}
					if (currentAnimation->hasEnded)
					{
						state = STATIC;
						heroChangedState = true;
					}
					break;

				case ENTER:
					if (currentAnimation->hasEnded)
					{
						state = STATIC;
						heroChangedState = true;
						canFire = true;
					}
					break;

				case DEAD:
					if (currentAnimation->hasEnded)
					{
						loseLife();
						display = true;
					}
					break;


				case EXITING:
					//Make the ship exit to the right
					posX = posX + 12;

					//Make the ship go toward the middle of the screen
					if(posY > GAMEZONE_LIMIT + GAMEZONE_HEIGHT/2 - 3)
					{
						posY = posY - 4;
					}
					if(posY < GAMEZONE_LIMIT + GAMEZONE_HEIGHT/2 + 3)
					{
						posY = posY + 4;
					}
					break;

				default:
					break;
			}
		}
	}

	if (!isFiring && CurrentLevel->soundEngine->sounds.at("mitLoop")->isPlaying)
	{
		CurrentLevel->soundEngine->stopSound("mitLoop");
		CurrentLevel->soundEngine->playSound("mitRelease");
	}

	massPotential = min(64.0f, massPotential + regenMassPo);

	animateLasers();

	if(state!=DEAD)
		checkInvicibility();

	if(shielded)
	{
		shield->updateAnimationFrame();
		shield->posX = posX;
		shield->posY = posY;
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

void Hero::stopFiring()
{
	if(state!=CURSOR)
	{
		currentWeapon->releaseFire(this);
		isFiring = currentWeapon->isFiring;
	}
}

void Hero::setWeapon(weaponName aWeaponName)
{
	//if (ownedWeapons.at(aWeaponName)->activated)
		currentWeapon = ownedWeapons.at(aWeaponName);

	if(!aWeaponName == ElectronGun && CurrentLevel->soundEngine->sounds.at("mitLoop")->isPlaying)
	{
		CurrentLevel->soundEngine->stopSound("mitLoop");
		CurrentLevel->soundEngine->playSound("mitRelease");
	}
}


void Hero::fireWeapon(weaponName aWeaponName)
{
	if(state != CURSOR )//&& (ownedWeapons.at(aWeaponName)->activated))
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
	for(map<weaponName, Weapon *>::iterator aWeapon = ownedWeapons.begin(); aWeapon != ownedWeapons.end(); ++aWeapon)
	{
		shoots.insert(shoots.begin(), (*aWeapon).second->shoots.begin(), (*aWeapon).second->shoots.end());
	}

	return &shoots;
}


void Hero::moveUp()
{
	if(topFlag)
	{
		posY = posY + 6;
	}
}

void Hero::moveDown()
{
	if(bottomFlag)
	{
		posY = posY - 6;
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
	posX = max(posX, width/2.0f);
	posY = max(posY, GAMEZONE_LIMIT + height/2);
	posX = min(posX, SCREEN_WIDTH - width/2);
	posY = min(posY, SCREEN_HEIGHT - height/2);
}

void Hero::animateLasers()
{
	for(map<weaponName, Weapon *>::iterator aWeapon = ownedWeapons.begin(); aWeapon != ownedWeapons.end(); ++aWeapon)
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
		CurrentLevel->levelState = LEVEL_LOST;
	}
	else
	{
		posX = SCREEN_WIDTH/3;
		posY = GAMEZONE_LIMIT + GAMEZONE_HEIGHT/2;
		state = ENTER;
		heroChangedState = true;
		display = false;
		health = 10;
		canFire = false;
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
				Enemy * anEnemy = dynamic_cast<Enemy *>(aDrawable);
				health = health - anEnemy->damage;
				if (health<=0)
				{
					CurrentLevel->soundEngine->playSound("playerExplode", posX);
					setState(DEAD);
				}
				else
				{
					CurrentLevel->soundEngine->playSound("AtomHit", posX);
					setState(HIT);
					hitAngle = PI;
				}
				return;
			}
			else
			{
				shielded = false;
				delete shield;
			}
		}

		if (aDrawable->isLaser())
		{
			Shoot * aShoot = dynamic_cast<Shoot*>(aDrawable);
			if (!shielded)
			{
				health--;//= health - aShoot->power;
				if (health<=0)
				{
					CurrentLevel->soundEngine->playSound("playerExplode", posX);
					setState(DEAD);
				}
				else
				{
					CurrentLevel->soundEngine->playSound("AtomHit", posX);
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
					shielded = false;
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
				CurrentLevel->createTextEffect(posX, posY, stringToDisplay.str());

				health = min(maxHealth, health + aBonus->quantity);

				if (health >= maxHealth)
				{
					CurrentLevel->soundEngine->playSound("UpHealth100");
				}
				else
				{
					CurrentLevel->soundEngine->playSound("UpHealth");
				}
				break;

			case BONUS_SHIELD:
				CurrentLevel->createTextEffect(posX, posY, "Bouclier!");
				CurrentLevel->soundEngine->playSound("getShield");

				if(shielded)
					delete shield;

				shield = new Shield(posX, posY);
				shielded = true;
				break;

			default:
				break;
		}

		if (aBonus->isQuarkBonus)
		{
			stringToDisplay <<  "Quark : +" << aBonus->quantity;
			CurrentLevel->createTextEffect(posX, posY,  stringToDisplay.str());
			if(aBonus->quantity == 1)
			{
				CurrentLevel->soundEngine->playSound("jQuarkSmall");
			}
			else
			{
				CurrentLevel->soundEngine->playSound("jQuarkBig");
			}
			quarkLevels.at((QuarkType)aBonus->quarkType) = min(34, quarkLevels.at((QuarkType) aBonus->quarkType) + aBonus->quantity);
			checkQuarkLevels();

			//radioactivePotential = min(64.0f, radioactivePotential + 32);
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
	posX = max(posX, width/2.0f);
	posY = max(posY, GAMEZONE_LIMIT + height/2);
	posX = min(posX, SCREEN_WIDTH - width/2);
	posY = min(posY, SCREEN_HEIGHT - height/2);

	return backOffSpeed;
}

void Hero::makeInvincible(int aTime)
{
	invincibilityTime = aTime;
	startInvincibility = GameTimer;
	invincible = true;
}

void Hero::checkInvicibility()
{
	if (startInvincibility + invincibilityTime < GameTimer)
	{
		invincible = false;
		display = true;
	}
}

void Hero::setState(int aState)
{
	state = aState;
	heroChangedState = true;
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

bool Hero::endTeleport()
{
	unsigned int endTeleportTime = startTeleporting + 1000;
	if (endTeleportTime < GameTimer)
	{
		return true;
	}
	return false;
}

void Hero::startEffect(string anEffect)
{
	CurrentLevel->createEffect(posX, posY, anEffect);
}

void Hero::spreadQuarks()
{
	int numberOfGold;
	float aSpeed, anAngle;
	int i;

	for(map<QuarkType, int>::iterator aQuark = quarkLevels.begin(); aQuark != quarkLevels.end(); ++aQuark)
	{
		numberOfGold = ((*aQuark).second / 5);

		for(i = 0; i < numberOfGold; i++)
		{
			aSpeed = 10.5f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/10.0f));
			anAngle = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/360.0f)) * (PI/180);
			CurrentLevel->createBonus(posX, posY, aSpeed, anAngle, (bonusType) (((*aQuark).first * 2) + 2));
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
	checkQuarkLevels();
}

void Hero::checkQuarkLevels()
{
	for(map<weaponName, Weapon *>::iterator aWeapon = ownedWeapons.begin(); aWeapon != ownedWeapons.end(); ++aWeapon)
	{
		(*aWeapon).second->checkActivation(this);
	}

	//If the currentWeapon is deactivated then we fall back on the electron
	if(!currentWeapon->activated)
	{
		setWeapon(ElectronGun);
	}
}

/*
 * Shield function
 */
Shield::Shield()
{
	energy = 3;
	copyFrom(CurrentLevel->loadedObjects.at("jSp_Bouclier"));
}

Shield::Shield(float x, float y)
{
	energy = 3;
	copyFrom(CurrentLevel->loadedObjects.at("jSp_Bouclier"));
	posX = x;
	posY = y;
	setAnimX(0);
	setAnimY(0);
}
