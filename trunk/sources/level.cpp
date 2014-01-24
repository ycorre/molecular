/*
 * Contains the level superclass functions
 */

#include "level.h"

Level::Level()
{
	bkg_nearSpeed = 0.4;
	bkg_midSpeed = 0.2;
	bkg_distantSpeed = 0.1;
	levelPosition = 0;
	levelState = LEVEL_PLAYING;
	fading = false;
	ending = false;
	cameraSpeed = 1;
	activeBlockingWave = 0;

	ge = NULL;
	pe = NULL;
	hero = NULL;
	hud = NULL;
	soundEngine = NULL;
	exiting = false;
}

void Level::loadLevel(Hero * anHero)
{

}

void Level::loadBackGround()
{
	bkg_near = *loadedObjects.at("bkgNear");
	bkg_near.posX = SCREEN_WIDTH/2;
	bkg_near.posY = SCREEN_HEIGHT/2;
	bkg_near.width = SCREEN_WIDTH;
	bkg_near.height = SCREEN_HEIGHT;

	bkg_mid = *loadedObjects.at("bkgMid");
	bkg_mid.posX = SCREEN_WIDTH/2;
	bkg_mid.posY = SCREEN_HEIGHT/2;
	bkg_mid.width = SCREEN_WIDTH;
	bkg_mid.height = SCREEN_HEIGHT;

	bkg_distant = *loadedObjects.at("bkgDistant");
	bkg_distant.posX = SCREEN_WIDTH/2;
	bkg_distant.posY = SCREEN_HEIGHT/2;
	bkg_distant.width = SCREEN_WIDTH;
	bkg_distant.height = SCREEN_HEIGHT;
}

void Level::moveBackGround()
{
	//Move the backgrounds
	bkg_distant.setAnimX(bkg_distant.getAnimX() + bkg_distantSpeed);
	bkg_mid.setAnimX(bkg_mid.getAnimX() + bkg_midSpeed);
	bkg_near.setAnimX(bkg_near.getAnimX() + bkg_nearSpeed);

	bkg_distant.processDisplay();
	bkg_mid.processDisplay();
	bkg_near.processDisplay();
}

void Level::drawLevel()
{
/*	checkEvent();

	pe->stayOnScreen(hero, make_pair(SCREEN_WIDTH, GAMEZONE_HEIGHT));

	for (list<Drawable *>::iterator anElement = activeElements.begin() ; anElement != activeElements.end(); ++anElement)
	{
		(*anElement)->animate();
		if((*anElement)->display)
		{
			ge->toDisplay.push_back(*anElement);
		}
	}
	hero->animate();
	background.setAnimX(background.getAnimX() + cameraSpeed);*/
}


//function that handle the events (enemies apparitions, collision checks,  etc...)
void Level::checkEvent()
{

	/*for (vector<Drawable *>::iterator anElement = activeElements.begin() ; anElement != activeElements.end(); ++anElement)
	{
		if((*anElement)->toRemove)
		{
			activeElements.erase(anElement++);
		}
		else
		{
			if((*anElement)->isEnemy())
			{
				checkEnemyCollision(*anElement);
				Enemy * anEnemy = dynamic_cast<Enemy *>(*anElement);
				anEnemy->fire();
			}
			if((*anElement)->isBonus() ||(*anElement)->isLaser())
			{
				checkCollision(*anElement);
			}
		}
	}*/
}

void Level::checkEnemyCollision(list<Enemy *> enemies)
{
	for(list<Enemy *>::iterator anEnemy = enemies.begin(); anEnemy != enemies.end(); ++anEnemy)
	{
		checkEnemyCollision(*anEnemy);
	}
}

bool Level::checkEnemyCollision(Drawable * anElement)
{
	if(pe->collisionDetection(hero, anElement))
	{
		anElement->processCollisionWith(hero);
		hero->processCollisionWith(anElement);
		return true;
	}

/*	for (list<Laser*>::iterator aLaser = hero->lasers.begin(); aLaser != hero->lasers.end(); ++aLaser)
	{
		Laser * aL = *aLaser;
		if(pe->collisionDetection(aL, anElement))
		{
			anElement->processCollisionWith(aL);
			aL->toRemove = TRUE;
			return 1;
		}
	}*/

	return false;
}

bool Level::checkCollision(Drawable * anElement)
{
	if(hero->shielded)
	{
		if(pe->collisionDetection(hero->shield, anElement))
		{
			anElement->processCollisionWith(hero);
			hero->processCollisionWith(anElement);
			return true;
		}
	}
	else
	{
		if(pe->collisionDetection(hero, anElement))
		{
			anElement->processCollisionWith(hero);
			hero->processCollisionWith(anElement);
			return true;
		}
	}
	return false;
}

//If the hero is close to a bonus then it is drawn toward it
void Level::magnetBonus(Drawable * aBonus)
{
	float xDist = aBonus->posX - hero->posX;
	float yDist = aBonus->posY - hero->posY;
	float distance = sqrt(xDist * xDist + yDist * yDist);

	if(distance < 150)
	{
		Bonus * aRealBonus = dynamic_cast<Bonus*>(aBonus);
		aRealBonus->drawnToHero();
	}
	else
	{
		Bonus * aRealBonus = dynamic_cast<Bonus*>(aBonus);
		aRealBonus->drawn = false;
	}
}

void Level::createExplosion(int x, int y)
{
	if(rand() % 2)
	{
		createEffect(x, y, "explosionA");
	}
	else
	{
		createEffect(x, y, "explosionB");
	}
    ge->startShaking(8, false);
}

void Level::createParticleEffect(int x, int y, string aName)
{
    ParticleEffect * aParticleEffect = new ParticleEffect();
    aParticleEffect->instantiateEffects(particleEffectConf.at(aName), x, y);
    ge->particleEffects.push_back(aParticleEffect);
}


void Level::createBonus(int x, int y, bonusType type)
{
	activeElements.push_back(new Bonus(x, y, 0, 0, type));
}

void Level::createBonus(int x, int y, float aSpeed, float anAngle, bonusType type)
{
	activeElements.push_back(new Bonus(x, y, aSpeed, anAngle, type));
}

void Level::loadLevelConfiguration(string path)
{
	Json::Value root;
	Json::Reader reader;
	ifstream file;
	file.open(path.c_str());
	unsigned int index;

	int parsingSuccessful = reader.parse(file, root);
	if (!parsingSuccessful)
	{
		//report to the user the failure and their locations in the document.
		cout << "Failed to parse configuration:" << endl << reader.getFormattedErrorMessages();
	}

	const Json::Value drawable = root["drawable"];
	for (index = 0; index < drawable.size(); ++index)
	{
		Drawable * tmp = new Drawable(drawable[index]);
		loadedObjects.insert(make_pair(tmp->name, tmp));
	}

	const Json::Value animDrawable = root["animatedDrawable"];
	for (index = 0; index < animDrawable.size(); ++index)
	{
		AnimatedDrawable * tmp = new AnimatedDrawable(animDrawable[index]);
		loadedObjects.insert(make_pair(tmp->name, tmp));
	}

	//Load effects
	const Json::Value effect = root["effect"];
	for (index = 0; index < effect.size(); ++index)
	{
		Effect * tmp = new Effect(effect[index]);
		loadedEffects.insert(make_pair(tmp->name, tmp));
	}

	//Load Sounds
	const Json::Value sounds = root["sounds"];
	for (index = 0; index < sounds.size(); ++index)
	{
		soundEngine->loadSoundFrom(sounds[index]);
	}

	//Load Music
	const Json::Value music = root["music"];
	for (index = 0; index < music.size(); ++index)
	{
		soundEngine->loadMusicFrom(music[index]);
	}

	//Load particle effect configuration
	const Json::Value particleEffect = root["particleEffect"];
	for (index = 0; index < particleEffect.size(); ++index)
	{
		particleEffectConf.insert(make_pair(particleEffect[index].get("name", "error").asString(), particleEffect[index]));
	}

	//Load enemy waves
	Json::Value enemyWavesConfig = root["enemyWaves"];
	for (index = 0; index < enemyWavesConfig.size(); ++index)
	{
		EnemyWave * aWave = new EnemyWave(enemyWavesConfig[index]);

		//If this the first time we encountered
		if(enemyWaves.find(aWave->startingDate) == enemyWaves.end())
		{
			enemyWaves.insert(make_pair(aWave->startingDate, vector<EnemyWave *>()));
		}

		enemyWaves.at(aWave->startingDate).push_back(aWave);
	}
}

void Level::createEffect(int x, int y, string name)
{
	activeEffects.push_back(new Effect(x, y, name));
}

void Level::createTextEffect(int x, int y, string aText)
{
	activeElements.push_back(new TextEffect(x, y, aText));
}

void Level::heroLoseLife()
{
	hero->loseLife();
}

int Level::isOnScreen(Drawable * aDrawable)
{
	return pe->isOnScreen(aDrawable);
}

void Level::cleanLevel()
{
	for (list<Drawable *>::iterator anElement = activeElements.begin() ; anElement != activeElements.end(); ++anElement)
	{
		//(*anElement)->clean();
		cout << (*anElement)->name << endl;
		delete (*anElement);
		//activeElements.erase(anElement++);
	}

	for (map<string, Drawable *>::iterator anElement = loadedObjects.begin() ; anElement != loadedObjects.end(); ++anElement)
	{
		(*anElement).second->clean();
		delete (*anElement).second;
	}
	loadedObjects.clear();

	activeElements.clear();
	loadedEffects.clear();
	activeEffects.clear();
	configurationElements.clear();
	enemyConfigurationElements.clear();
	effectConfigurationElements.clear();
	objectConfiguration.clear();
	enemyWaves.clear();
	activeWaves.clear();

	soundEngine->stopMusic("hybridQuarks");
	soundEngine->stopAllSounds();
	soundEngine->freeLoadedSounds();

	ge->freeTextures();
}

void Level::endLevel()
{
	cleanLevel();
	levelState = LEVEL_WON;
}

void Level::finishLevel()
{

}


