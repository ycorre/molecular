/*
 * Graphic Engine header
 */
#ifndef GE_H
#define GE_H
 
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <SDL.h>
#include <SDL_image.h>
#include "drawable.h"

// screen width, height, and bit depth
#define SCREEN_WIDTH  1200
#define SCREEN_HEIGHT 600
#define GAMEZONE_HEIGHT 480
#define SCREEN_BPP     32

class Ge
{
	public:
	  SDL_Surface *screen;
	  vector<Drawable *> toDisplay;
	  map<string, SDL_Surface*> textures;
	  vector<TTF_Font *> availableFonts;
	  map<string, SDL_Color> availableColors;

	  void displayFrame();
	  int draw(Drawable * sprite);
	  void drawFrame();
	  void blitElement(SDL_Surface * anElement);
	  void addFont(string path);
	  void initColors();
	  SDL_Surface * loadTexture(string path);
};


#endif

