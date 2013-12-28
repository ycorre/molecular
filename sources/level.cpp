/*
 * Contains the level superclass functions
 */

#include "level.h"

Level::Level()
{
	bkg_nearSpeed = 0.4;
	bkg_midSpeed = 0.2;
	bkg_distantSpeed = 0.1;
	pe = NULL;
	hud = NULL;
	exiting = FALSE;
}

void Level::loadLevel(Hero * anHero)
{

}

Level * Level::launchLevel(string aLevelName)
{
	int aLevel;
	int last_index = aLevelName.find_last_not_of("0123456789");
	aLevel = atoi(aLevelName.substr(last_index + 1).c_str());
	switch(aLevel)
	{
		/*case 1:
			return new Level1();
			break;
		case 2:
			return new Level2();
			break;*/

		default:
			cerr << "Trying to load unknown level " << aLevelName << endl;
			return NULL;
	}

	return NULL;
}

void Level::loadBackGround()
{
	bkg_near = *loadedObjects.at("bkgNear");
	bkg_near.width = SCREEN_WIDTH;
	bkg_near.height = SCREEN_HEIGHT;

	bkg_mid = *loadedObjects.at("bkgMid");
	bkg_mid.width = SCREEN_WIDTH;
	bkg_mid.height = SCREEN_HEIGHT;

	bkg_distant = *loadedObjects.at("bkgDistant");
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

int Level::checkEnemyCollision(Drawable * anElement)
{
	if(pe->collisionDetection(hero, anElement))
	{
		anElement->processCollisionWith(hero);
		hero->processCollisionWith(anElement);
		return 1;
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

	return 0;
}

int Level::checkCollision(Drawable * anElement)
{
	if(hero->shielded)
	{
		if(pe->collisionDetection(hero->shield, anElement))
		{
			anElement->processCollisionWith(hero);
			hero->processCollisionWith(anElement);
			return 1;
		}
	}
	else
	{
		if(pe->collisionDetection(hero, anElement))
		{
			anElement->processCollisionWith(hero);
			hero->processCollisionWith(anElement);
			return 1;
		}
	}
	return 0;
}

void Level::createExplosion(int x, int y)
{
	activeElements.push_back(new Explosion(x, y));
    ParticleEffect * aParticleEffect = new ParticleEffect();
    aParticleEffect->createExplosionFrom(x, y);
    ge->particleEffects.push_back(aParticleEffect);
}

void Level::createBonus(int x, int y, int type)
{
	activeElements.push_back(new Bonus(x, y, 0, 0, type));
}

void Level::createBonus(int x, int y, float aSpeed, float anAngle, int type)
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

	const Json::Value e = root["enemies"];
	//cout << "rrrrrrrrrrrrrrrrrrrrrrrrrrrrr " << e.get("sinusHeight", 80).asFloat() << endl;
	configurations.insert(make_pair("enemies", e));
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

	map<SDL_Surface *, string> freedTextures;
	for (map<string, Drawable *>::iterator anElement = loadedObjects.begin() ; anElement != loadedObjects.end(); ++anElement)
	{
		(*anElement).second->clean();
		delete (*anElement).second;
	}
	loadedObjects.clear();

	//activeElements.clear();
	loadedEffects.clear();
	activeEffects.clear();
	configurationElements.clear();
	enemyConfigurationElements.clear();
	effectConfigurationElements.clear();
	objectConfiguration.clear();

	soundEngine->stopMusic("hybridQuarks");
	soundEngine->stopAllSounds();

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


