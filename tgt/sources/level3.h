/*
 * Contains level 3 description
 */
 
#ifndef LEVEL3_H
#define LEVEL3_H
 
#include <iostream>
#include <string>
#include <utility>
#include <fstream>
#include <sstream>
#include "level.h"

class Level3:  public Level
{
	public:
	int cameraSpeed;
	Falcon * theFalcon;

	virtual void loadLevel(Hero * aHero);
	void loadConf();
	void loadObject();
	void loadTextures();
	void loadBackGround();
	virtual void drawLevel();
	void checkEvent();
	void createWave1();
	int checkCollision(Drawable * anEnemy);
	void createExplosion(int x, int y, int type);
	void createBonus(int x, int y, int type);
	virtual void finishLevel();
	int checkEnemyCollision(Drawable * anElement);
	int checkBonusCollision(Drawable * anElement);
};

#endif

