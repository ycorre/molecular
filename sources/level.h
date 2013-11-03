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

class Hero;
class Pe;

class Level
{
	public:
	Drawable bg;
	GraphicEngine * ge;
	Pe * pe;
	Hero * hero;
	HUD * hud;
	SoundEngine * soundEngine;
	list<Drawable*> activeElements;
	map<string, vector<string> > configurationElements;
	string name;
	int levelState;
	int cameraSpeed;
	int exiting, ending, fading;
	Drawable smallStars, bigStars, nebulae;

	virtual void loadLevel(Hero * aHero);
	virtual void loadConf();
	virtual void loadObject();
	virtual void loadTextures();
	virtual void loadBackGround();
	virtual void moveBackGround();
	virtual void drawLevel();
	virtual void checkEvent();
	virtual void heroLoseLife();
	virtual void createExplosion(int x, int y, int type);
	virtual void createBonus(int x, int y, int type);
	virtual int checkCollision(Drawable * anEnemy);
	virtual int checkEnemyCollision(Drawable * anElement);
	virtual void endLevel();
	virtual void finishLevel();
	int isOnScreen(Drawable * aDrawable);
};

#define LEVEL_PLAYING 0
#define LEVEL_WON 1
#define LEVEL_LOST 2
#define LEVEL_FINISHING 3

#endif

