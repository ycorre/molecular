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
	  int width;
	  int height;
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
	  map<int, float> scalingEffect;
	  map<int, float> opacityEffect;
	  int display; //Boolean: should the object be display
	  int toRemove; //Boolean: should the object be destroyed
	  int isBlinking; //Boolean: should the object be blinking
	  unsigned int blinkingCounter; //Keep track of the blinking frames
	  string name;
	  float ogl_Xorigin, ogl_Yorigin, ogl_Xcorner, ogl_Ycorner;
	  int posXCorrection, posYCorrection;
	  int toBlend;
	  map<string, int> confParameters;
	  SDL_Surface * collision;

	  //static since the graphic engine is the same for all the objects
	  static GraphicEngine * ge;
	  static Level * lev;

	  SDL_Surface * texture;

	  //OpenGL stuff
	  GLuint oglTexture;

	  Drawable();
	  Drawable(Json::Value aConfig);
	  virtual ~Drawable();
	  virtual void animate();
	  virtual void blink();
	  void processDisplay();
	  void loadTexture(string path);

	  virtual void clean();
	  virtual void copyFrom(Drawable * aDrawable);

	  void computeOGLXValues();
	  void computeOGLYValues();

	  virtual int isEnemy() {return FALSE;}
	  virtual int isHero() {return FALSE;}
	  virtual int isLaser() {return FALSE;}
	  virtual int isBonus() {return FALSE;}
	  virtual int hasHitBox() {return FALSE;}
	  virtual int isText() {return FALSE;}

	  virtual void processCollision();
	  virtual void processCollisionWith(Drawable* aDrawable);
	  virtual int getXBoundary();
	  virtual int getYBoundary();
	  virtual int getWidthBoundary();
	  virtual int getHeightBoundary();
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
	  virtual float getPosX();
	  virtual float getPosY();
	  virtual int getCurrentFrame();
	  int updateAnimationFrame();
	  virtual void copyFrom(Drawable * aDrawable);
	  void copyFrom(AnimatedDrawable * aDrawable);
	  void setAnimation(string anAnimationName);
};

#endif
