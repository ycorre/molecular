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

	vector<vector<float> > stars;
	vector<vector<float> > starPoints;
	vector<Drawable *> starDrawable;
	int numberOfStarPoints, numberOfStars;
	map<string, int> configCategories;
	float maxDepth;

	Level();
	Level * launchLevel(string aLevel);
	virtual void loadLevel(Hero * anHero);
	void loadLevelConfiguration(string path);
	virtual void loadTextures();
	virtual void loadBackGround();
	virtual void moveBackGround();
	virtual void drawLevel();
	virtual void checkEvent();
	virtual void heroLoseLife();
	virtual void createEffect(int x, int y, string name);
	virtual void createTextEffect(int x, int y, string name, string aText);
	virtual void createExplosion(int x, int y);
	virtual void createBonus(int x, int y, int type);
	virtual int checkCollision(Drawable * anEnemy);
	virtual int checkEnemyCollision(Drawable * anElement);
	virtual void cleanLevel();
	virtual void endLevel();
	virtual void finishLevel();

	void loadStarConf();
	void generateStarfield();
	void displayStarField();
	int isOnScreen(Drawable * aDrawable);
};

#endif

