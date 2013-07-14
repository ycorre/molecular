/*
 * Contains level 2 description

 */
 
#ifndef LEVEL1_H
#define LEVEL1_H
 
#include <iostream>
#include <string>
#include <utility>
#include <fstream>
#include <sstream>
#include "level.h"

class Level1:  public Level
{
	public:
	int cameraSpeed;
	unsigned int lastTimeAsteroid;
	int asteroidRate;

	//Level1();
	virtual void  loadLevel();
	virtual void loadConf();
	virtual void loadObject();
	virtual void loadTextures();
	virtual void loadBackGround();
	virtual void drawLevel();
	virtual void checkEvent();
	void createWave1();
	virtual int checkCollision(Drawable * anElement);
	virtual void createExplosion(int x, int y, int type);
	virtual void createBonus(int x, int y, int type);
	virtual int checkEnemyCollision(Drawable * anElement);
	int generateAsteroid();
};

#endif

