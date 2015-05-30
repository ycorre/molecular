#ifndef GAME_H
#define GAME_H
 
#include <iostream>
#include <string>
#include <SDL/SDL_ttf.h>
#include <time.h>
#include "common.h"
#include "menu.h"
#include "graphicEngine.h"
#include "physicEngine.h"
#include "soundEngine.h"
#include "level.h"
#include "level1.h"
#include "level2.h"
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
		Level * aLevel;
		Menu * menu;
		Hero * hero;
		//List of levels
		//map<string, Level*> levels;
		vector<string> levelOrder;
		map<string, bool>lockedLevel;
		int score;
		bool done;

		//Hold the current state of the game (cf. types.h for the states)
		GameState gameState;
		GameState previousGameState;

		Game();
		int mainLoop();
		int initSDL();
		int initGame();
		void processEvents();
		void mainProcessing();
		void loadMenu(MenuState aMenu);
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

#endif

