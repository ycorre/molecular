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
	void loadLevel(Hero * aHero);
	void drawLevel();
	void checkEvent();
	bool checkCollision(Drawable * anElement);
	bool checkEnemyCollision(Drawable * anElement);
	void finishLevel();
};
#endif

