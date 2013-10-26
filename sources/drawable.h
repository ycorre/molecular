/*
 * Graphic Engine
 * Contains graphic-related functions (loading texture, drawing, etc.)
 */
#ifndef DRAWABLE_H
#define DRAWABLE_H
#include <iostream>
#include <vector>
#include <sstream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "common.h"
#include "graphicEngine.h"

class GraphicEngine;
class Level;

class Drawable
{
	public:
	  int width;
	  int height;
	  float posX;
	  float posY;
	  int state;
	  int animX; //Position in the Sprite grid to display
	  int animY;
	  int display; //Boolean: should the object be display
	  int toRemove; //Boolean: should the object be destroyed
	  int isBlinking; //Boolean: should the object be blinking
	  unsigned int blinkingCounter; //Keep track of the blinking frames

	  //static since the graphic engine is the same for all the objects
	  static GraphicEngine * ge;
	  static Level * lev;

	  //last timestamp where the animation was updated; useful to control the animation speed
	  int lastTimeUpdated;
	  //The number of frames in each animation // TODO change to map when conf load by file
	  vector<int> nbFrames;
	  SDL_Surface * texture;

	  Drawable();
	  ~Drawable();
	  virtual void animate();
	  virtual void blink();
	  void processDisplay();
	  int updateAnimationFrame();
	  virtual int isEnemy() {return 0;}
	  virtual int isHero() {return 0;}
	  virtual int isLaser() {return 0;}
	  virtual int isBonus() {return 0;}
	  virtual void processCollision();
	  virtual void processCollisionWith(Drawable* aDrawable);
};

vector<int> parseAnimationState(string aConf);

#endif

