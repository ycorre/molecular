/*
 * Contains keyboard handling function
 */

#ifndef KEYB_H
#define KEYB_H

#include <SDL.h>
#include <SDL_image.h>
#include "graphicEngine.h"
#include "game.h"
#include "hero.h"
//#include "menu.h"

class Game;
class Menu;

class Keyboard
{
	public:
		Uint8 * keyStates;
		Game * game;
		int p_pressed;

		Keyboard();
		void handleKeyPress(SDL_keysym *keysym);
		//void handleKeyPressHero(SDL_keysym *keysym, Hero * hero);
		void handleKeyUpHero(SDL_keysym *keysym, Hero  * hero);
		void processKeyPress();
		void processKeyState();
		void processKeyPressHero(Hero* hero);
		void handleKeyPressMenu(SDL_keysym *keysym, Menu * menu);
		void processKeyPressMenu(Menu* menu);
};

//Definition of the keymap
#define UP_KEY SDLK_UP
#define DOWN_KEY SDLK_DOWN
#define LEFT_KEY SDLK_LEFT
#define RIGHT_KEY SDLK_RIGHT
#define FIRE_KEY SDLK_SPACE
#define PAUSE_KEY SDLK_p

void Quit(int returnCode);

#endif

