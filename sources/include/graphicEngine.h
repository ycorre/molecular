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
#include <algorithm>
#include <SDL.h>
#include <SDL_image.h>
#include <math.h>
#include "drawable.h"
#include "particleEffect.h"

class Drawable;
class ParticleEffect;
class LineEffect;

class GraphicEngine
{
	private:
	  float aspectRatio;

	public:
	  SDL_Surface * screen;
	  vector<Drawable *> toDisplay;
	  list<ParticleEffect *> particleEffects;
	  map<string, TTF_Font *> availableFonts;
	  map<string, SDL_Color> availableColors;

	  map<string, SDL_Surface *> textures;
	  map<SDL_Surface *, GLuint> openGLTextures;

	  float xOffsetFactor, yOffsetFactor;

	  int alphaFading;
	  int fadingSpeed;
	  bool isFading;
	  bool shakingEffect;
	  bool shakeALot;
	  unsigned int shakeCounter;
	  vector<int> shakeValues;

	  GraphicEngine();
	  void init();
	  void displayFrame();
	  int draw(Drawable * sprite);
	  void drawEffect(ParticleEffect * anEffect);
	  void drawFrame();
	  void performRotation(Drawable * sprite);
	  void performScaling(Drawable * sprite);
	  int addFont(string aName, string path, int size);
	  void initColors();
	  void fadeIn();
	  void mergeImages(vector <Drawable*> drawables , Drawable * destination);
	  SDL_Surface * loadTexture(string path, bool clamp);
	  void createOGLTexture(SDL_Surface * aSurface, GLuint * oglTex, bool clamp);
	  void startFadingOut(int aFadingSpeed);
	  void freeTextures();
	  void deleteParticleEffect();
	  void startShaking(int aLength, bool aLot);
	  void shakeCamera(bool sense);
	  void setAspectRatio(float aValue);

	private:
	  void fadeOut();
};

bool sortDisplayedElement(const Drawable * a, const Drawable * b);

#endif

