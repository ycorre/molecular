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
#include "drawable.h"
#include "graphicEngine.h"

class HUD
{
	public:
	Ge * ge;
	Drawable backGround;
	map<string, vector<string> > configurationElements;
	map<string, Drawable> hudElements;

	HUD();
	void loadHUDElements(string path);
	void displayUI();
	void displayHealth(int life);
};


#endif

