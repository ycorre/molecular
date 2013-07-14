/*
 * Contains the interface for the menu and the intro
 */
 
#ifndef MENU_H
#define MENU_H
 
#include <iostream>
#include <string>
#include <utility>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "graphicEngine.h"
#include "soundEngine.h"
#include "text.h"
#include "game.h"

class Game;

class Menu
{
	public:
	Drawable * bg, * arrow;
	Ge * ge;
	Game * game;
	SoundEngine * soundEngine;
	Drawable * logo, *bbox;
	int introLength;
	int selected;
	Uint32 startIntro, endIntro;
	vector<Text *> menuElements;
	TTF_Font * menuFont;
	SDL_Color menuColor;

	list<Drawable*> activeElements;
	map<string, vector<string> > configurationElements;

	Menu(Ge * aGe, SoundEngine * aSe);
	void loadMenu();
	void loadIntro();
	void displayMenu();
	void playIntro();
	void loadMenuElements(string path);
	void selectUp();
	void selectDown();
	void select();
//	bool sortElement(Text *a, Text* b);
};

bool sortElement(Text * a, Text * b);

#endif

