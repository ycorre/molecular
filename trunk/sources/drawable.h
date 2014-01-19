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
#include "animation.h"

class GraphicEngine;
class Level;
class Animation;

class Drawable
{
	private:
	  //Positions inside the Sprite grid to display
	  float animX;
	  float animY;

	public:
	  float width;
	  float height;
	  float posX;
	  float posY;
	  float posZ; //Depth
	  float opacity;
	  float scaleX;
	  float scaleY;
	  float rotX;
	  float rotY;
	  float rotZ;
	  float rotationAngle;
	  float brightness;
	  float brightnessDecreasingFactor;
	  int virtualDepth;
	  map<int, float> scalingEffect;
	  map<int, float> opacityEffect;
	  bool display; //should the object be displayed
	  bool toRemove; //should the object be destroyed
	  bool isBlinking; //should the object be blinking
	  bool isBlinkingWhite; //should the object flash white
	  unsigned int blinkingCounter; //Keep track of the blinking frames
	  string name;
	  float ogl_Xorigin, ogl_Yorigin, ogl_Xcorner, ogl_Ycorner;
	  float posXCorrection, posYCorrection;
	  int toBlend;
	  map<string, int> confParameters;
	  SDL_Surface * collision;
	  bool clampTexture;

	  //static since the graphic engine is the same for all the objects
	  static GraphicEngine * ge;

	  SDL_Surface * texture;

	  //OpenGL stuff
	  GLuint oglTexture;

	  Drawable();
	  Drawable(Json::Value aConfig);
	  virtual ~Drawable();
	  virtual void animate();
	  void blink();
	  void startBlinkingWhite(int aNumberOfFrames);
	  void blinkWhite();
	  void processDisplay();
	  void loadTexture(string path);

	  virtual void clean();
	  virtual void copyFrom(Drawable * aDrawable);

	  void computeOGLXValues();
	  void computeOGLYValues();

	  virtual bool isEnemy() {return false;}
	  virtual bool isHero() {return false;}
	  virtual bool isLaser() {return false;}
	  virtual bool isBonus() {return false;}
	 // virtual int hasHitBox() {return FALSE;}
	  virtual bool isText() {return false;}

	  virtual void processCollision();
	  virtual void processCollisionWith(Drawable* aDrawable);
	  virtual float getXBoundary();
	  virtual float getYBoundary();
	  virtual float getWidthBoundary();
	  virtual float getHeightBoundary();
	  virtual float getAnimX();
	  virtual void setAnimX(float aValue);
	  virtual float getAnimY();
	  virtual float getPosX();
	  virtual float getPosY();
	  virtual void setAnimY(float aValue);
	  virtual void setWidth(int aValue);
	  virtual float getWidth();
	  virtual float getHeight();

	  virtual SDL_Surface * getTexture();
	  virtual GLuint getOpenGLTexture();
	  virtual SDL_Surface * getCollisionTexture();
};

class AnimatedDrawable: virtual public Drawable
{
	public:
	  //How often we update the frame in the animation
	  	//41 ms ~= 24 FPS
	  	//33 ms ~= 30 FPS
	  int animationUpdateFrequency;
	  Animation * currentAnimation;
	  map <string, Animation *> animations;
	  //last timestamp where the animation was updated; useful to control the animation speed
	  int lastTimeUpdated;

	  AnimatedDrawable();
	  AnimatedDrawable(Json::Value aConfig);
	  virtual ~AnimatedDrawable();
	  void setAnimationsPointer();
	  virtual void clean();
	  virtual float getWidth();
	  virtual float getHeight();
	  virtual int getCurrentFrame();
	  bool updateAnimationFrame();
	  virtual void copyFrom(Drawable * aDrawable);
	  void copyFrom(AnimatedDrawable * aDrawable);
	  void setAnimation(string anAnimationName);
};

#endif
