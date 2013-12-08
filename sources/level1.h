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

class Level1: public Level
{
	public:
	int cameraSpeed;

	Level1();
	void loadEnemies();
	void instantiateEnemies();
	virtual void loadLevel(Hero * aHero);
	virtual void drawLevel();
	virtual void checkEvent();
	virtual int checkCollision(Drawable * anElement);
	virtual int checkEnemyCollision(Drawable * anElement);
	virtual void finishLevel();
};
void loadEffects();
#endif

