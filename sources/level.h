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

class Hero;
class Pe;
class HUD;

enum{LEVEL_PLAYING, LEVEL_WON, LEVEL_LOST, LEVEL_FINISHING};

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
	int levelState;
	int cameraSpeed;
	int exiting, ending, fading;
	Drawable bkg_mid, bkg_near, bkg_distant;
	float bkg_nearSpeed, bkg_midSpeed, bkg_distantSpeed;
	int displayBackGround;

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
	void createBonus(int x, int y, int type);
	void createBonus(int x, int y, float aSpeed, float anAngle, int type);
	virtual int checkCollision(Drawable * anEnemy);
	virtual int checkEnemyCollision(Drawable * anElement);
	virtual void cleanLevel();
	virtual void endLevel();
	virtual void finishLevel();

	int isOnScreen(Drawable * aDrawable);
};

#endif

