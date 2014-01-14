#ifndef ENEMYWAVE_H
#define ENEMYWAVE_H

#include "enemy.h"

class EnemyFactory
{
	public:
		map<string, EnemyType> enemyTypes;

		EnemyFactory(){};
	    static EnemyFactory * instance();
		Enemy * createEnemy(string anEnemyName);
		Enemy * createEnemy(Json::Value anEnemyConf);
};

class EnemyWave
{
	public:
		int startingDate;
		int scoreBonus;
		bool active;
		list<Enemy *> enemies;
		vector<bonusType> possibleBonus;

		Json::Value enemyConfig;

		EnemyWave();
		EnemyWave(Json::Value aConfig);
		void animate();
	    void launchWave();
	    void createSiliconWave(Json::Value aConfig);
};

#endif

