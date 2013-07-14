#ifndef GAME_H
#define GAME_H
 
#include <iostream>
#include <string>
#include <SDL_ttf.h>
#include "common.h"
#include "menu.h"
#include "graphicEngine.h"
#include "physicEngine.h"
#include "soundEngine.h"
#include "level.h"
#include "level1.h"
#include "level2.h"
#include "keyb.h"

class Keyboard;

class Game
{
	public:
		Ge graphicEngine;
		SoundEngine soundEngine;
		Pe physicEngine;
		Keyboard * keyboard;
		Level * currentLevel;
		Menu * menu;
		//List of levels
		map<string, Level*> levels;

		//Hold the current state of the game (cf. below for the state)
		int gameState;
		int previousGameState;

		Game();
		int mainLoop();
		int initSDL();
		int initGame();
		void launchLevel(string lev);
		void pause();
};

//Definition of the values for the gameState variable
#define LOGO 0
#define MENU 1
#define INGAME 2
#define GAMEOVER 3
#define WIN 4
#define PAUSE 5

#endif

