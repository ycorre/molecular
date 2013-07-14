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
	Ge * ge;
	Pe * pe;
	Hero * hero;
	HUD * hud;
	SoundEngine * soundEngine;
	list<Drawable*> activeElements;
	map<string, vector<string> > configurationElements;
	int cameraSpeed;

	virtual void loadLevel();
	virtual void loadConf();
	virtual void loadObject();
	virtual void loadTextures();
	virtual void loadBackGround();
	virtual void drawLevel();
	virtual void checkEvent();
	virtual void heroLoseLife();
	virtual void createExplosion(int x, int y, int type);
	virtual void createBonus(int x, int y, int type);
	virtual int checkCollision(Drawable * anEnemy);
	virtual int checkEnemyCollision(Drawable * anElement);
	//int checkBonusCollision(Drawable * anElement);
	int isOnScreen(Drawable * aDrawable);
};

#endif

