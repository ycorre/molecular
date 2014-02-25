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
		GraphicEngine * ge;
		Game * game;
		SoundEngine * soundEngine;
		TTF_Font * menuFont;
		SDL_Color menuColor;
		MenuState currentMenu, nextMenu;
		bool startingGame;
		bool menuInTransition;

		Drawable bkgTitle, bkgOption, bkgLevel, highScoreBg;
		Drawable logo;
		Drawable credit, gameOver, success;
		Drawable title, bubble1, bubble2;
		Drawable optionTitle, levelTitle;
		Drawable soundVolumeBar, musicVolumeBar;
		Text soundVolume, musicVolume, soundVolumePercent, musicVolumePercent;

		AnimatedDrawable * optionButton, * quitButton, * levelSelectButton;

		int introLength;
		Uint32 startIntro, endIntro;

		int selected, selectedLevel, selectedOption;
		vector<AnimatedDrawable *> menuElements;
		list<Drawable> levelSelectElements;
		vector<AnimatedDrawable> levelSelectHalo;
		vector<Drawable> levelLocks;
		vector<Text*> optionMenuElements;

		map<string, Drawable> loadedMenuElements;
		map<string, AnimatedDrawable> loadedAnimMenuElements;
		map<string, vector<string> > configurationElements;

		string newName;
		int newHighScoreRank;
		pair<string, unsigned int> ahighScores[5];
		vector<pair<string, unsigned int> > highScores;
		list<Text *> highScoreElements;

		Menu(GraphicEngine * aGe, SoundEngine * aSe);
		void loadMenu();
		void loadSelectedLevel();
		void loadTextures();
		void transition();
		void updateLocks();

		void displayMenu();
		void displayOptions();
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
		bool checkForNewHighScore();

		void selectionMove(int direction);
		void mainSelectionMove(int direction);
		void levelSelectionMove(int direction);
		void optionSelectionMove(int direction);
		void setSelectedLevel(int aValue);
		void setMainSelection(int aValue);
		void setSelectedOption(int aValue);
		void select();
		void selectMainMenu();
		void selectLevel();
};

bool sortElement(AnimatedDrawable * a, AnimatedDrawable * b);
bool sortScores(pair<string, int>  a, pair<string, int>  b);

#endif

