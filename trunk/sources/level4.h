/*
 * Contains level 4 description
 */
 
#ifndef LEVEL4_H
#define LEVEL4_H
 
#include <iostream>
#include <string>
#include <utility>
#include <fstream>
#include <sstream>
#include <SDL_image.h>
#include "level.h"

class Level4:  public Level
{
	public:
	int cameraSpeed;
	Drawable fullBg, smallStars, bigStars, nebulae;
	Drawable rayonH, rayonV;
	vector<Drawable *> toMerge;

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

	void mergeBg();
};

#endif

