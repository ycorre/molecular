/*
 * Graphic Engine
 * Contains graphic-related functions (loading texture, drawing, etc.)
 */

#include "drawable.h"

GraphicEngine * Drawable::ge;
Level * Drawable::lev;

Drawable::Drawable()
{
	lastTimeUpdated = 0;
	animationUpdateFrequency = 40;
	nbFrames.push_back(1);
	texture = NULL;
	state = 0;
	posX = 0;
	posY = 0;
	posZ = 0;
	animX = 0;
	animY = 0;
	width = 1;
	height = 1;
	display = TRUE;
	toRemove = FALSE;
	toBlend = FALSE;
	isBlinking = FALSE;
	currentFrame = 0;
	blinkingCounter = 0;
}

Drawable::~Drawable()
{

}

void Drawable::animate()
{

}

void Drawable::processCollision()
{

}

void Drawable::processCollisionWith(Drawable* aDrawable)
{

}

//Change the animation to display
int Drawable::updateAnimationFrame()
{
	//41 ms ~= 24 FPS
	//33 ms ~= 30 FPS
	unsigned int updateTime = lastTimeUpdated + animationUpdateFrequency;
	if (updateTime < ProgramTimer)
	{
		//shift one sprite to the right; if we are at the end then go back to the beginning
		setAnimX((float)(((int)animX + width) % (nbFrames.at(state)*width)));
		lastTimeUpdated = ProgramTimer;
		return TRUE;
	}

	return FALSE;
}

vector<int> parseAnimationState(string aConf)
{
	istringstream aConfStream(aConf);
	string aState;
	vector<int> nbFrames;

	while(getline(aConfStream, aState,'#'))
	{
		if(aState.size()!=0){
			istringstream nbFrame(aState);
			string aNumber;
			getline(nbFrame, aNumber, '$');
			//Get the second element (the one we are interested in)
			getline(nbFrame, aNumber, '$');
			nbFrames.push_back(atoi(aNumber.c_str()));
		}
	}

	return nbFrames;
}

void Drawable::processDisplay()
{
	if (isBlinking)
	{
		blink();
	}

	if(display)
	{
		ge->toDisplay.push_back(this);
	}
}

void Drawable::loadTexture(string path)
{
	this->texture = ge->loadTexture(path);
#if USE_OPENGL
	createOGLTexture();
#endif
}

void Drawable::addTexture(string aName)
{
	this->name = aName;
	this->texture = ge->textures.at(this->name);
#if USE_OPENGL
	this->oglTexture = ge->openGLTextures.at(this->texture);
	setAnimX(animX);
	setAnimY(animY);
#endif
}

void Drawable::createOGLTexture()
{
	glGenTextures(1, &this->oglTexture);
	glBindTexture(GL_TEXTURE_2D, this->oglTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, this->texture->w, this->texture->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, this->texture->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	setAnimX(animX);
	setAnimY(animY);
}

void Drawable::clean()
{
#if USE_OPENGL
	ge->openGLTextures.erase(this->texture);
#endif
	ge->textures.erase(this->name);
}

void Drawable::blink()
{
	if (blinkingCounter == 0)
	{
		//Blinking effect showing that we are invincible
		display = (display + 1) % 2;
	}
	blinkingCounter = (blinkingCounter + 1) % 3;
}

float Drawable::getAnimX()
{
	return animX;
}

void Drawable::setAnimX(float animX)
{
	this->animX = animX;

#if USE_OPENGL
	if(texture!= NULL)
	{
		computeOGLXValues();
	}
#endif
}

void Drawable::computeOGLXValues()
{
	ogl_Xorigin = this->animX/(float)this->getTexture()->w;
	ogl_Xcorner = ogl_Xorigin + (float)this->width/(float)this->getTexture()->w;
}

float Drawable::getAnimY()
{
	return animY;
}

void Drawable::setAnimY(float animY)
{
	this->animY = animY;
#if USE_OPENGL
	if(texture!= NULL)
	{
		computeOGLYValues();
	}
#endif
}

void Drawable::computeOGLYValues()
{
	ogl_Yorigin = this->animY/(float)this->getTexture()->h;
	ogl_Ycorner = ogl_Yorigin + (float)this->height/(float)this->getTexture()->h;
}

int Drawable::getXBoundary()
{
	return posX;
}

int Drawable::getYBoundary()
{
	return posY;
}

int Drawable::getWidthBoundary()
{
	return width;
}

int Drawable::getHeightBoundary()
{
	return height;
}

SDL_Surface * Drawable::getCollisionTexture()
{
	return texture;
}

SDL_Surface * Drawable::getTexture()
{
	return texture;
}

GLuint Drawable::getOpenGLTexture()
{
	return oglTexture;
}

void Drawable::setWidth(int aValue)
{
	width = aValue;
}

/*
 * Hitboxed class functions
 */
int HitBoxedDrawable::getXBoundary()
{
	return hitBoxX;
}

int HitBoxedDrawable::getYBoundary()
{
	return hitBoxY;
}

int HitBoxedDrawable::getHeightBoundary()
{
	return hitBoxHeight;
}

int HitBoxedDrawable::getWidthBoundary()
{
	return hitBoxWidth;
}

/*
 * Masked Drawable functions
 */
SDL_Surface * MaskedDrawable::getCollisionTexture()
{
	return collision;
}

/*
 * Multi texture Drawable functions
 */

void MultiTextureDrawable::addTexture(string aName)
{
	textures.insert(make_pair(aName, ge->textures.at(aName)));
	if(texture==NULL)
	{
		texture = ge->textures.at(aName);
	}
	textureState = aName;
#if USE_OPENGL
	oglTextures.insert(make_pair(aName, ge->openGLTextures.at(textures.at(aName))));
	setAnimX(getAnimX());
	setAnimY(getAnimY());
#endif
}

SDL_Surface * MultiTextureDrawable::getTexture()
{
	return textures.at(textureState);
}

GLuint MultiTextureDrawable::getOpenGLTexture()
{
	return oglTextures.at(textureState);
}

void MultiTextureDrawable::setTextureState(string aState)
{
	textureState = name + aState;
	currentFrame = 0;
}


/*
 * FrameDrawable functions
 */
int FrameDrawable::updateAnimationFrame()
{

	unsigned int updateTime = lastTimeUpdated + animationUpdateFrequency;
	if (updateTime < ProgramTimer)
	{
		//increment to the next frame or loop back to the beginning
		//Textures are a grid of frame numbered from left to right and from top to bottom
		currentFrame = (currentFrame + 1) % nbFrames.at(state);
		//Compute the position in the texture
		setAnimY((currentFrame / (getTexture()->w/realWidth)) * height);
		setAnimX((currentFrame % (getTexture()->w/realWidth)) * realWidth);
		lastTimeUpdated = ProgramTimer;
		return 1;
	}

	return 0;
}

void FrameDrawable::setWidth(int aValue)
{
	width = aValue;
	realWidth = width;
	numberOfFrameInLine = getTexture()->w/width;
	setAnimY(0);
	setAnimX(0);
}
