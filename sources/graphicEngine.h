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
#include <math.h>
#include "drawable.h"
#include "particleEffect.h"

// screen width, height, and bit depth
#define SCREEN_WIDTH  1200
#define SCREEN_HEIGHT 800
#define GAMEZONE_HEIGHT 682
#define SCREEN_BPP     32

class Drawable;
class ParticleEffect;
class LineEffect;

class GraphicEngine
{
	public:
	  SDL_Surface * screen;
	  vector<Drawable *> toDisplay;
	  list<ParticleEffect *> particleEffects;

	  map<string, TTF_Font *> availableFonts;
	  map<string, SDL_Color> availableColors;

	  map<string, SDL_Surface *> textures;
	  map<SDL_Surface *, GLuint> openGLTextures;

	  float aspectRatio;

	  //Black rectangle used for the fading effects
	  Drawable * blackBox;
	  int alphaFading;
	  int fadingSpeed;
	  int isFading;

	  GraphicEngine();
	  void initGe();
	  void displayFrame();
	  int draw(Drawable * sprite);
	  int drawEffect(ParticleEffect * anEffect);
	  void drawFrame();
	  void blitElement(SDL_Surface * anElement);
	  void addFont(string aName, string path);
	  void initColors();
	  void fadeIn();
	  void mergeImages(vector <Drawable*> drawables , Drawable * destination);
	  SDL_Surface * loadTexture(string path);
	  void createOGLTexture(SDL_Surface * aSurface, GLuint * oglTex);
	  void startFadingOut(int aFadingSpeed);

	private:
	  void fadeOut();
};


#endif

