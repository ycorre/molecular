/*
 * Contains the interface for the HUD
 */
 
#ifndef HUD_H
#define HUD_H
 
#include <iostream>
#include <string>
#include <utility>
#include <fstream>
#include <sstream>
#include <map>
#include "text.h"
#include "drawable.h"
#include "graphicEngine.h"

class HUD
{
	public:
	GraphicEngine * ge;
	Drawable backGround;
	map<string, vector<string> > configurationElements;
	map<string, Drawable> hudElements;
	Text * lifeHUD;
	Text * scoreHUD;

	HUD();
	HUD(GraphicEngine * ge);
	void loadHUDElements(string path);
	void displayUI();
	void displayHealth(int life);
	void displayLife(int nbLife);
	void displayScore(int score);
};


#endif

