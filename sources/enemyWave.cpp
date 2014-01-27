#include "enemyWave.h"

EnemyWave::EnemyWave()
{
	startingDate = 0;
	scoreBonus = 0;
	active = false;
	blocking = false;
}

EnemyWave::EnemyWave(Json::Value aConfig)
{
	unsigned int j;
	startingDate = aConfig.get("startingDate", 0).asInt();
	blocking = aConfig.get("blocking", false).asBool();
	scoreBonus = aConfig.get("scoreBonus", 0).asInt();

	const Json::Value bonusReleased = aConfig["possibleBonus"];
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
		enemies.push_back(EnemyFactory::instance()->createEnemy(enemyConfig[i], this));
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
			//If we are deleting the last enemy
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
			enemies.remove(*anEnemy--);
		}
		else
		{
			(*anEnemy)->animate();
			(*anEnemy)->processDisplay();

			CurrentLevel->checkEnemyCollision(*anEnemy);
		}
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
    aFactory.enemyTypes["Pyroxene"] = PYROXENE;

    return &aFactory;
}

Enemy * EnemyFactory::createEnemy(Json::Value anEnemyConf, EnemyWave * anEnemyWave)
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
			return new SiliconField(anEnemyConf, anEnemyWave);
			break;

		case COPPER:
			return new Copper(anEnemyConf);
			break;

		case PYROXENE:
			return new PyroxeneField(anEnemyConf, anEnemyWave);
			break;

		default:
			cerr << "EnemyFactory: Tried to instantiated an unknown enemy type: " << aType << endl;
			break;
	}

	return NULL;
}
