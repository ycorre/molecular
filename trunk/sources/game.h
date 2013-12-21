#ifndef GAME_H
#define GAME_H
 
#include <iostream>
#include <string>
#include <SDL_ttf.h>
#include <time.h>
#include "common.h"
#include "menu.h"
#include "graphicEngine.h"
#include "physicEngine.h"
#include "soundEngine.h"
#include "level.h"
#include "level1.h"
#include "level2.h"
#include "level4.h"
#include "keyb.h"
#include "hero.h"

class Keyboard;

class Game
{
	public:
		GraphicEngine graphicEngine;
		SoundEngine soundEngine;
		Pe physicEngine;
		Keyboard * keyboard;
		Level * currentLevel;
		Level * aLevel;
		Menu * menu;
		Hero * hero;
		//List of levels
		map<string, Level*> levels;
		vector<string> levelOrder;
		map<string, int>lockedLevel;
		int score;

		//Hold the current state of the game (cf. below for the state)
		int gameState;
		int previousGameState;

		Game();
		int mainLoop();
		int initSDL();
		int initGame();
		void loadConf();
		void updateTimers();
		void newGame();
		void launchLevel(string lev);
		void launchNextLevel();
		void checkNewHighScore();
		void muteAll();
		void stopMusic();
		void quitGame();
		void pause();
};

void controlFPS();
int initOpenGL();

//Definition of the values for the gameState variable
enum {GAME_MENU, GAME_INGAME, GAME_GAMEOVER, GAME_WIN, GAME_PAUSE, GAME_LOADMENU, GAME_LOADLEVEL};

#endif

