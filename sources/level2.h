/*
 * Contains level 2 description
 */
 
#ifndef LEVEL2_H
#define LEVEL2_H
 
#include <iostream>
#include <string>
#include <utility>
#include <fstream>
#include <sstream>
#include "level.h"

class Level2:  public Level
{
	public:
	//Frequency at which asteroid are generated
	int bombGenerationRate;
	//Keep track of the last asteroid generated
	unsigned int lastTimeBomb;

	Level2();
	virtual void loadLevel(Hero * aHero);
	void loadObject();
	void loadTextures();
	void loadBackGround();
	virtual void drawLevel();
	void checkEvent();
	void createWave1();
	int checkCollision(Drawable * anEnemy);
	void createExplosion(int x, int y, int type);
	void createBonus(int x, int y, int type);
	int checkEnemyCollision(Drawable * anElement);
	int checkBonusCollision(Drawable * anElement);
	int generateBomb();
	virtual void finishLevel();
};

#endif

