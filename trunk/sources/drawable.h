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

enum ParamValues { undefinedValue,
				  pName,
				  pWidth,
				  pHeight,
				  pTexture,
				  pAnimations,
				  pAnim,
				  pNumberOfFrames,
				  pBlending,
			 	  pOpacity,
				  pMoveTexture,
				  pScale};

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

	  //static since the graphic engine is the same for all the objects
	  static GraphicEngine * ge;
	  static Level * lev;

	  //last timestamp where the animation was updated; useful to control the animation speed
	  int lastTimeUpdated;
	  SDL_Surface * texture;

	  //OpenGL stuff
	  GLuint oglTexture;
	  float textureXOrigin, textureYOrigin;

	  Drawable();
	  ~Drawable();
	  virtual void animate();
	  virtual void blink();
	  void processDisplay();
	  void loadTexture(string path);

	  void clean();
	  void loadFrom(string aConfString);
	  void copyFrom(Drawable * aDrawable);

	  void createOGLTexture();
	  void computeOGLXValues();
	  void computeOGLYValues();

	  virtual int isEnemy() {return FALSE;}
	  virtual int isHero() {return FALSE;}
	  virtual int isLaser() {return FALSE;}
	  virtual int isBonus() {return FALSE;}
	  virtual int hasHitBox() {return FALSE;}
	  virtual int isComposite() {return FALSE;}

	  virtual void addTexture(string name);
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

	  AnimatedDrawable();
	  virtual float getWidth();
	  virtual float getHeight();
	  virtual float getPosX();
	  virtual float getPosY();
	  virtual int getCurrentFrame();
	  void parseAnimationState(string aConf);
	  int updateAnimationFrame();
	  void copyFrom(AnimatedDrawable * aDrawable);
	  void setAnimation(string anAnimationName);
	  void loadFrom(string aConfString);
};

//Class for objects that use a mask for collision detection
class MaskedDrawable: virtual public Drawable
{
	public:
	  SDL_Surface * collision;

	  virtual SDL_Surface * getCollisionTexture();
};

//Class for objects that uses several textures that require blending to display
class CompositeDrawable: virtual public Drawable
{
	public:
	  vector<Drawable *> toMerge;

	  virtual int isComposite() {return TRUE;}
};

//Class for objects who have several images as texture
class MultiTextureDrawable: virtual public Drawable
{
	public:
	  map<string, SDL_Surface *> textures;
	  map<string, GLuint> oglTextures;
	  string textureState;

	  virtual void addTexture(string path);
	  virtual SDL_Surface * getTexture();
	  virtual GLuint getOpenGLTexture();
	  virtual void setTextureState(string aState);
};


#endif
