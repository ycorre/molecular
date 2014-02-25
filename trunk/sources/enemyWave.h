#ifndef ENEMYWAVE_H
#define ENEMYWAVE_H

#include "enemy.h"

class EnemyWave;

class EnemyFactory
{
	public:
		map<string, EnemyType> enemyTypes;

		EnemyFactory(){};
	    static EnemyFactory * instance();
		Enemy * createEnemy(string anEnemyName);
		Enemy * createEnemy(Json::Value anEnemyConf, EnemyWave * anEnemyWave);
};

class EnemyWave
{
	public:
		int startingDate;
		int scoreBonus;
		bool active;
		bool blocking;
		list<Enemy *> enemies;
		vector<bonusType> possibleBonus;

		Json::Value enemyConfig;

		EnemyWave();
		EnemyWave(Json::Value aConfig);
		~EnemyWave();
		void animate();
	    void launchWave();
};

#endif

