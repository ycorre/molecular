#include "enemyWave.h"

EnemyWave::EnemyWave()
{
	startingDate = 0;
	scoreBonus = 0;
	active = false;
}

EnemyWave::EnemyWave(Json::Value aConfig)
{
	unsigned int j;
	startingDate = aConfig.get("startingDate", 0).asInt();
	scoreBonus = aConfig.get("scoreBonus", 0).asInt();

	const Json::Value bonusReleased = aConfig["possibleBonus"];
	//possibleBonus.assign(&bonusReleased, &bonusReleased + bonusReleased.size());
	for (j = 0; j < bonusReleased.size(); j++)
	{
		possibleBonus.push_back((bonusType)(bonusReleased[j].asInt()));
	}

	enemyConfig = aConfig["enemies"];
	active = false;
}

void EnemyWave::launchWave()
{
	unsigned int i;

	//Instantiate the enemies of the wave
	for (i = 0; i < enemyConfig.size(); i++)
	{
		if (enemyConfig[i].get("name", " ").compare("Silicon") == 0)
		{
			createSiliconWave(enemyConfig[i]);
		}
		else
		{
			enemies.push_back(EnemyFactory::instance()->createEnemy(enemyConfig[i]));
		}
	}

	active = true;
}

//Animate the elements of the wave
void EnemyWave::animate()
{
	for(list<Enemy *>::iterator anEnemy = enemies.begin(); anEnemy != enemies.end(); ++anEnemy)
	{
		if((*anEnemy)->toRemove)
		{
			//If we are deleting the last enemies
			if (enemies.size() == 1)
			{
				if(!possibleBonus.empty())
				{
					bonusType aBonusType = possibleBonus.at((rand() % possibleBonus.size()));
					CurrentLevel->createBonus((*anEnemy)->posX, (*anEnemy)->posY, aBonusType);
				}
				active = false;
			}

			delete (*anEnemy);
			anEnemy = enemies.erase(anEnemy);
		}
		else
		{
			(*anEnemy)->animate();
			(*anEnemy)->processDisplay();

			CurrentLevel->checkEnemyCollision(*anEnemy);
		}
	}
}

//Create a field of silicon
void EnemyWave::createSiliconWave(Json::Value aConfig)
{
	int i;
	int generatedNumber = aConfig.get("generatedNumber", 50).asInt();
	for(i = 0; i < generatedNumber; i++)
	{
		enemies.push_back(EnemyFactory::instance()->createEnemy(aConfig));
	}
}

/*
 * EnemyFactory Functions
 */
EnemyFactory * EnemyFactory::instance()
{
    static EnemyFactory aFactory;
    aFactory.enemyTypes["Cadmium"] = CADMIUM;
    aFactory.enemyTypes["Iron"] = IRON;
    aFactory.enemyTypes["Silicon"] = SILICON;
    aFactory.enemyTypes["Copper"] = COPPER;

    return &aFactory;
}

Enemy * EnemyFactory::createEnemy(Json::Value anEnemyConf)
{
	EnemyType aType = enemyTypes.at(anEnemyConf.get("name", "error").asString());

	switch(aType)
	{
		case CADMIUM:
			return new Cadmium(anEnemyConf);
			break;

		case IRON:
			return new Iron(anEnemyConf);
			break;

		case SILICON:
			return new Silicon(anEnemyConf);
			break;

		case COPPER:
			return new Copper(anEnemyConf);
			break;

		default:
			cerr << "EnemyFactory: Tried to instantiated an unknown enemy type: " << aType << endl;
			break;
	}

	return NULL;
}
