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
#include "common.h"
#include "graphicEngine.h"
#include "soundEngine.h"
#include "text.h"
#include "game.h"

class Game;

class Menu
{
	public:
		Drawable bkgTitle, bkgOption, bkgLevel, highScoreBg;
		GraphicEngine * ge;
		Game * game;
		SoundEngine * soundEngine;
		Drawable logo;
		Drawable credit, gameOver, success;
		Drawable title, bubble1, bubble2;
		Drawable optionTitle, levelTitle;
		int startingGame;

		AnimatedDrawable optionButton, quitButton, levelSelectButton;

		int introLength;
		int selected, selectedLevel;
		Uint32 startIntro, endIntro;
		vector<AnimatedDrawable> menuElements;
		list<Drawable> levelSelectElements;
		vector<AnimatedDrawable> levelSelectHalo;
		vector<Drawable> levelLocks;
		TTF_Font * menuFont;
		SDL_Color menuColor;
		string newName;
		int newHighScoreRank;
		int menuInTransition;

		map<string, Drawable> loadedMenuElements;
		map<string, AnimatedDrawable> loadedAnimMenuElements;

		map<string, vector<string> > configurationElements;

		pair<string, unsigned int> ahighScores[5];
		vector<pair<string, unsigned int> > highScores;
		list<Text *> highScoreElements;
		MenuState currentMenu, nextMenu;

		Menu(GraphicEngine * aGe, SoundEngine * aSe);
		void loadMenu();
		void loadSelectedLevel();
		void loadTextures();
		void transition();
		void updateLocks();

		void displayMenu();
		void displayLevelSelection();
		void displayIntro();
		void displayMainMenu();
		void displayHighScore();
		void displayCredit();
		void displayGameOver();
		void displaySuccess();

		void loadMenuElements(string path);
		void readHighScore(string path);
		void saveHighScore();
		void enterHighScore();
		void addChar(char aChar);
		void eraseChar();
		void finishEnteringName();
		void sortHighScores();
		void updateHighScore();
		int checkForNewHighScore();

		void selectionMove(int direction);
		void mainSelectionMove(int direction);
		void levelSelectionMove(int direction);
		void setSelectedLevel(int aValue);
		void setMainSelection(int aValue);
		void select();
		void selectMainMenu();
		void selectLevel();
};

bool sortElement(AnimatedDrawable a, AnimatedDrawable b);
bool sortScores(pair<string, int>  a, pair<string, int>  b);

#endif

