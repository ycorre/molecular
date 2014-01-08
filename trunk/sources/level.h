/*
 * Contains the interface for a basic level
 */
 
#ifndef LEVEL_H
#define LEVEL_H
 
#include <iostream>
#include <string>
#include <utility>
#include <fstream>
#include <sstream>
#include "graphicEngine.h"
#include "laser.h"
#include "enemy.h"
#include "physicEngine.h"
#include "soundEngine.h"
#include "bonus.h"
#include "hero.h"
#include "hud.h"
#include "effect.h"
#include "enemyWave.h"

class Hero;
class Pe;
class HUD;
class EnemyWave;

class LevelFactory
{

};

class Level
{
	public:
	GraphicEngine * ge;
	Pe * pe;
	Hero * hero;
	HUD * hud;
	SoundEngine * soundEngine;
	list<Drawable*> activeElements;
	map<string, vector<string> > configurationElements;
	map<string, Effect *> loadedEffects;
	list<Effect *> activeEffects;
	list<vector<string> > enemyConfigurationElements;
	vector<string> effectConfigurationElements;
	vector<string> objectConfiguration;
	map<string, Drawable *> loadedObjects;
	string name;
	LevelState levelState;
	int cameraSpeed;
	int exiting, ending, fading;
	Drawable bkg_mid, bkg_near, bkg_distant;
	float bkg_nearSpeed, bkg_midSpeed, bkg_distantSpeed;
	int levelPosition;
	map<int, vector<EnemyWave *> > enemyWaves;
	list<EnemyWave *> activeWaves;

	map<string, Json::Value> configurations;

	Level();
	Level * launchLevel(string aLevel);
	virtual void loadLevel(Hero * anHero);
	void loadLevelConfiguration(string path);
	virtual void loadBackGround();
	virtual void moveBackGround();
	virtual void drawLevel();
	virtual void checkEvent();
	virtual void heroLoseLife();
	void createEffect(int x, int y, string name);
	void createTextEffect(int x, int y, string aText);
	void createExplosion(int x, int y);
	void createBonus(int x, int y, bonusType type);
	void createBonus(int x, int y, float aSpeed, float anAngle, bonusType type);
	void startWave(int aNumber);
	virtual int checkCollision(Drawable * anEnemy);
	virtual int checkEnemyCollision(Drawable * anElement);
	virtual void cleanLevel();
	virtual void endLevel();
	virtual void finishLevel();

	int isOnScreen(Drawable * aDrawable);
};

#endif

