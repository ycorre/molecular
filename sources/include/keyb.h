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

class Game;
class Menu;

class Keyboard
{
	public:
		Uint8 * keyStates;
		Game * game;
		bool p_pressed;
		bool firing;

		Keyboard();
		void handleKeyUp(SDL_keysym * keysym);
		void processKeyPress();
		void processKeyState();
		void processKeyInGame(Hero* hero);
		void handleKeyPressMenu(SDL_keysym *keysym, Menu * menu);
		void processeMouseInGame(Hero * hero);
};

//Definition of the keymap
#define UP_KEY SDLK_UP
#define DOWN_KEY SDLK_DOWN
#define LEFT_KEY SDLK_LEFT
#define RIGHT_KEY SDLK_RIGHT

#define ELECTRON_KEY SDLK_c
#define PHOTON_KEY SDLK_v
#define BARYON_KEY SDLK_b
#define PLASMA_KEY SDLK_n

#define TELEPORT_KEY SDLK_SPACE
#define PAUSE_KEY SDLK_p

#define MUTE_MUSIC_KEY SDLK_o
#define MUTE_SOUND_KEY SDLK_i
#define MUTE_ALL_KEY SDLK_m

void quit(int returnCode);

#endif

