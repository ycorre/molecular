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
	  int state;
	  string stateString;
	  int display; //Boolean: should the object be display
	  int toRemove; //Boolean: should the object be destroyed
	  int isBlinking; //Boolean: should the object be blinking
	  unsigned int blinkingCounter; //Keep track of the blinking frames
	  string name;
	  float ogl_Xorigin, ogl_Yorigin, ogl_Xcorner, ogl_Ycorner;
	  int toBlend;
	  int currentFrame;
	  int animationUpdateFrequency; //How often we update the frame in the animation
			//41 ms ~= 24 FPS
			//33 ms ~= 30 FPS

	  //static since the graphic engine is the same for all the objects
	  static GraphicEngine * ge;
	  static Level * lev;

	  //last timestamp where the animation was updated; useful to control the animation speed
	  int lastTimeUpdated;
	  //The number of frames in each animation // TODO change to map when conf load by file
	  vector<int> nbFrames;
	  map<string, int> numberOfFrames;
	  SDL_Surface * texture;

	  //OpenGL stuff
	  GLuint oglTexture;
	  float textureXOrigin, textureYOrigin;

	  Drawable();
	  ~Drawable();
	  virtual void animate();
	  virtual void blink();
	  void processDisplay();
	  virtual int updateAnimationFrame();
	  void loadTexture(string path);
	  virtual void parseAnimationState(string aConf);
	  void clean();

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

//Class for objects that use a hit box for collision detection
class HitBoxedDrawable: public Drawable
{
	public:
	  int hitBoxWidth;
	  int hitBoxHeight;
	  int hitBoxX;
	  int hitBoxY;

	  virtual int hasHitBox() {return TRUE;}
	  virtual int getXBoundary();
	  virtual int getYBoundary();
	  virtual int getWidthBoundary();
	  virtual int getHeightBoundary();
};


//Class for objects that use a mask for collision detection
class MaskedDrawable: virtual public Drawable
{
	public:
	  SDL_Surface * collision;

	  virtual SDL_Surface * getCollisionTexture();
};

//Class for objects that uses several textures that require blending to display
class CompositeDrawable: public Drawable
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

//Class for objects who have a grid of sprites as texture
class FrameDrawable: virtual public Drawable
{
	public:
	  int numberOfFrameInLine;
	  int realWidth;

	  virtual int updateAnimationFrame();
	  virtual void setWidth(int aValue);
};

//Class for objects who have several images of different size as texture
class MultiSizeTextureDrawable: virtual public MultiTextureDrawable, virtual public FrameDrawable
{
	public:
	  int spriteWidth;
	  int currentWidth, currentHeight;
	  int posXCorrection, posYCorrection;
	  map<string, pair<int, int> > textureSizes;

	  virtual float getPosX();
	  virtual float getPosY();
	  virtual float getWidth();
	  virtual float getHeight();
	  virtual void addTexture(string path, vector<string> * aConf);
	  virtual void setTextureState(string aState);
	  virtual int updateAnimationFrame();
};

#endif

