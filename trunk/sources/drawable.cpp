/*
 * Graphic Engine
 * Contains graphic-related functions (loading texture, drawing, etc.)
 */

#include "drawable.h"

GraphicEngine * Drawable::ge;
Level * Drawable::lev;

Drawable::Drawable()
{
	name = "drawableDefaut";
	texture = NULL;
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
	blinkingCounter = 0;
	posXCorrection = 0;
	posYCorrection = 0;
	opacity = 1.0f;
	scaleX = 1.0f;
	scaleY = 1.0f;
	rotX = 0;
	rotY = 0;
	rotZ = 0;
	rotationAngle = 0;
	ogl_Xorigin = ogl_Xcorner = ogl_Ycorner = ogl_Yorigin = 0.0;
	oglTexture = 0;
}

Drawable::Drawable(Json::Value aConfig)
{
	name = aConfig.get("name", "drawableDefaut").asString();
	width = aConfig.get("width", 1).asInt();
	height = aConfig.get("height", 1).asInt();
	posX = aConfig.get("posX", 0).asFloat();
	posY = aConfig.get("posY", 0).asFloat();
	posZ = aConfig.get("posZ", 0).asFloat();
	animX = aConfig.get("animX", 0).asFloat();
	animY = aConfig.get("animY", 0).asFloat();
	display = aConfig.get("display", TRUE).asInt();
	toRemove = aConfig.get("toRemove", FALSE).asInt();
	isBlinking = aConfig.get("isBlinking", FALSE).asInt();
	toBlend = aConfig.get("blend", FALSE).asInt();
	blinkingCounter = aConfig.get("blinkingCounter", 0).asInt();
	posXCorrection = aConfig.get("posXCorrection", 0).asInt();
	posYCorrection = aConfig.get("posYCorrection", 0).asInt();
	scaleX = aConfig.get("scaleX", 1.0f).asFloat();
	scaleY = aConfig.get("scaleY", 1.0f).asFloat();
	opacity = aConfig.get("opacity", 1.0f).asFloat();
	rotX = aConfig.get("rotX", 0.0f).asFloat();
	rotY = aConfig.get("rotY", 0.0f).asFloat();
	rotZ = aConfig.get("rotZ", 0.0f).asFloat();
	rotationAngle = aConfig.get("rotationAngle", 0.0f).asFloat();
	loadTexture(aConfig.get("dataPath", " ").asString());
}

Drawable::~Drawable()
{
	cout<< name <<" deleted " << endl;
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
	//ge->openGLTextures.erase(this->texture);
#endif
	//ge->textures.erase(this->name);
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
	ogl_Xcorner = ogl_Xorigin + (float)this->getWidth()/(float)this->getTexture()->w;
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
	ogl_Ycorner = ogl_Yorigin + (float)this->getHeight()/(float)this->getTexture()->h;
}

void Drawable::copyFrom(Drawable * aDrawable)
{
	name = aDrawable->name;
	width = aDrawable->width;
	height = aDrawable->height;
	posX = aDrawable->posX;
	posY = aDrawable->posY;
	texture = aDrawable->texture;
	oglTexture = aDrawable->oglTexture;
	toBlend = aDrawable->toBlend;
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
	return width * scaleX;
}

int Drawable::getHeightBoundary()
{
	return height * scaleY;
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

float Drawable::getPosX()
{
	return posX;
}

float Drawable::getPosY()
{
	return posY;
}

float Drawable::getWidth()
{
	return width;
}

float Drawable::getHeight()
{
	return height;
}

/*
 * AnimatedDrawable Functions
 */
AnimatedDrawable::AnimatedDrawable()
{
	animationUpdateFrequency = 40;
	lastTimeUpdated = 0;
	currentAnimation = NULL;
}


AnimatedDrawable::AnimatedDrawable(Json::Value aConfig)
{
	name = aConfig.get("name", "drawableDefaut").asString();
	width = aConfig.get("width", 1).asInt();
	height = aConfig.get("height", 1).asInt();
	posX = aConfig.get("posX", 0).asFloat();
	posY = aConfig.get("posY", 0).asFloat();
	posZ = aConfig.get("posZ", 0).asFloat();
	setAnimX(aConfig.get("animX", 0).asFloat());
	setAnimY(aConfig.get("animY", 0).asFloat());
	display = aConfig.get("display", TRUE).asInt();
	toRemove = aConfig.get("toRemove", FALSE).asInt();
	isBlinking = aConfig.get("isBlinking", FALSE).asInt();
	toBlend = aConfig.get("blend", FALSE).asInt();
	blinkingCounter = aConfig.get("blinkingCounter", 0).asInt();
	posXCorrection = aConfig.get("posXCorrection", 0).asInt();
	posYCorrection = aConfig.get("posYCorrection", 0).asInt();
	scaleX = aConfig.get("scaleX", 1.0f).asFloat();
	scaleY = aConfig.get("scaleY", 1.0f).asFloat();
	opacity = aConfig.get("opacity", 1.0f).asFloat();
	rotX = aConfig.get("rotX", 0.0f).asFloat();
	rotY = aConfig.get("rotY", 0.0f).asFloat();
	rotZ = aConfig.get("rotZ", 0.0f).asFloat();
	rotationAngle = aConfig.get("rotationAngle", 0.0f).asFloat();
	currentAnimation = NULL;

	string texturePath = aConfig.get("dataPath", "").asString();
	if(!texturePath.empty())
	{
		loadTexture(texturePath);
	}
	animationUpdateFrequency = aConfig.get("animationUpdateFrequency", 40).asInt();

	const Json::Value confAnimations = aConfig["animations"];
	for (unsigned int i = 0; i < confAnimations.size(); i++)
	{
		Animation * anAnim = new Animation(confAnimations[i], this);
		anAnim->drawable = this;
		animations.insert(make_pair(anAnim->name, anAnim));
		if(currentAnimation == NULL)
			setAnimation(anAnim->name);
	}

	lastTimeUpdated = 0;
}

AnimatedDrawable::~AnimatedDrawable()
{
	cout<< "AD: " << name <<" deleted " << endl;
}

void AnimatedDrawable::clean()
{
	animations.clear();
}

float AnimatedDrawable::getWidth()
{
	return currentAnimation->width;
}

float AnimatedDrawable::getHeight()
{
	return currentAnimation->height;
}

int AnimatedDrawable::getCurrentFrame()
{
	return currentAnimation->currentFrame;
}

float AnimatedDrawable::getPosX()
{
	return posX + (posXCorrection * scaleX);
}

float AnimatedDrawable::getPosY()
{
	return posY + (posYCorrection * scaleY);
}

//Change the animation to display
int AnimatedDrawable::updateAnimationFrame()
{
	//41 ms ~= 24 FPS
	//33 ms ~= 30 FPS
	unsigned int updateTime = lastTimeUpdated + animationUpdateFrequency;
	if (updateTime < ProgramTimer)
	{
		currentAnimation->nextFrame();
		lastTimeUpdated = ProgramTimer;
		return TRUE;
	}

	return FALSE;
}

void AnimatedDrawable::setAnimation(string aName)
{
	currentAnimation = animations.at(aName);
	currentAnimation->currentFrame = 0;
	texture = currentAnimation->texture;
	oglTexture = currentAnimation->oglTexture;
	posXCorrection = width / 2 - currentAnimation->width / 2;
	posYCorrection = height / 2 - currentAnimation->height / 2;
}

void AnimatedDrawable::copyFrom(Drawable * aDrawable)
{
	copyFrom(dynamic_cast<AnimatedDrawable *> (aDrawable));
}

void AnimatedDrawable::copyFrom(AnimatedDrawable * aDrawable)
{
	name = aDrawable->name;
	width = aDrawable->width;
	height = aDrawable->height;
	for (std::map<string, Animation *>::const_iterator anElement = aDrawable->animations.begin(); anElement != aDrawable->animations.end(); ++anElement)
	{
		Animation * aNewAnim = new Animation((*anElement).second);
		aNewAnim->drawable = this;
		animations.insert(make_pair((*anElement).first, aNewAnim));
	}

	toBlend = aDrawable->toBlend;
	texture = aDrawable->texture;
	oglTexture = aDrawable->oglTexture;
	animationUpdateFrequency = aDrawable->animationUpdateFrequency;
	currentAnimation = aDrawable->currentAnimation;
	setAnimation(currentAnimation->name);
	posXCorrection = aDrawable->posXCorrection;
	posYCorrection = aDrawable->posYCorrection;
}

/*
 * Masked Drawable functions
 */
SDL_Surface * MaskedDrawable::getCollisionTexture()
{
	return collision;
}
