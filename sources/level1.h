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

	Level1();
	void loadEnemies();
	void instantiateEnemies();
	virtual void loadLevel(Hero * aHero);
	virtual void loadObject();
	virtual void loadTextures();
	virtual void loadBackGround();
	virtual void drawLevel();
	virtual void checkEvent();
	virtual int checkCollision(Drawable * anElement);
	virtual void createExplosion(int x, int y, int type);
	virtual void createBonus(int x, int y, int type);
	virtual int checkEnemyCollision(Drawable * anElement);
	virtual void finishLevel();
};

#endif

