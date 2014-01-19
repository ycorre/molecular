/*
 * Animation
 * Contains all the parameters necessary for an animation (changing frame, opacity, scaling...)
 */
#ifndef ANIMATION_H
#define ANIMATION_H
#include <iostream>
#include <vector>
#include <sstream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "common.h"
#include "graphicEngine.h"
#include "drawable.h"

class Drawable;

class Animation
{
	public:
	  string name;
	  int width;
	  int height;
	  float texturePosX;
	  float texturePosY;
	  int numberOfFrames;
	  int currentFrame;
	  bool loop;
	  SDL_Surface * texture;
	  GLuint oglTexture;

	  vector<float> opacityValues;
	  vector<float> scalingValues;
	  bool moveTexture;

	  //How often we update the frame in the animation
	  //41 ms ~= 24 FPS
	  //33 ms ~= 30 FPS
	  int animationUpdateFrequency;
	  Drawable * drawable;
	  bool hasEnded;
	  bool hasAnimatedTexture;

	  Animation();
	  Animation(Animation * anAnim);
	  Animation(Drawable * aDrawable);
	  Animation(Json::Value aConf, Drawable * aDrawable);
	  int nextFrame();
	  void incrementCurrentFrame();
	  void setFrameTo(int aNumber);
	  void configOpacity(vector<float> aConf);
	  void configScaling(vector<float> aConf);
	  void loadTexture(string path);
};


#endif