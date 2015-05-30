/*
 * Contains keyboard handling functions
 */
#ifndef KEYB_H
#define KEYB_H

#include "graphicEngine.h"
#include "game.h"
#include "hero.h"

class Game;
class Menu;

class Keyboard
{
	public:
		Game * game;
		bool p_pressed;
		bool firing;

		Keyboard();
		void handleKeyUp(int key);
		void processKeyPress();
		void processKeyState();
		void processKeyState(GLFWwindow* window, int key, int scancode, int action, int mods);
		void processKeyInGame(Hero* hero);
		void handleKeyPressMenu(int keys, Menu * menu);
		void processeMouseInGame(Hero * hero);
};

//Definition of the keymap
#define UP_KEY GLFW_KEY_UP
#define DOWN_KEY GLFW_KEY_DOWN
#define LEFT_KEY GLFW_KEY_LEFT
#define RIGHT_KEY GLFW_KEY_RIGHT

#define ELECTRON_KEY GLFW_KEY_C
#define PHOTON_KEY GLFW_KEY_V
#define BARYON_KEY GLFW_KEY_B
#define PLASMA_KEY GLFW_KEY_N

#define TELEPORT_KEY GLFW_KEY_SPACE
#define PAUSE_KEY GLFW_KEY_P

#define MUTE_MUSIC_KEY GLFW_KEY_O
#define MUTE_SOUND_KEY GLFW_KEY_I
#define MUTE_ALL_KEY GLFW_KEY_M

void quit(int returnCode);

#endif

