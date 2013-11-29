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

	//confParameters = {("name", pName), ("width", pWidth), ("height", pHeight), ("texture", pTexture), ("anim", pAnim), ("animations", pAnimations)};
	confParameters["name"] = pName;
	confParameters["width"] = pWidth;
	confParameters["height"] = pHeight;
	confParameters["texture"] = pTexture;
	confParameters["anim"] = pAnim;
	confParameters["animations"] = pAnimations;
	confParameters["blending"] = pBlending;
	confParameters["opacity"] = pOpacity;
	confParameters["moveTexture"] = pMoveTexture;
	confParameters["scale"] = pScale;
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

void Drawable::loadFrom(string aConfString)
{
	istringstream aConfStream(aConfString);
	string aLine;
	string token;

	getline(aConfStream, aLine);

	istringstream myLine(aLine);
	getline(myLine, token, ';');
	name = token;

	while(getline(myLine, token, ';'))
	{
		istringstream aParam(token);
		string paramType;
		string paramValue;
		getline(aParam, paramType, ':');
		getline(aParam, paramValue, ':');

		switch(confParameters.at(paramType))
		{
			case pName:
				name = paramValue;
				cout<< name << endl;
				break;

			case pWidth:
				width = atoi(paramValue.c_str());
				break;

			case pHeight:
				height = atoi(paramValue.c_str());
				break;

			case pTexture:
				loadTexture(paramValue.c_str());
				break;

			case pBlending:
				if(!paramValue.compare("true"))
					toBlend = TRUE;
				break;

			default:
				cerr << "Drawable loadFrom(): Unknown configuration parameter: " << paramType << endl;
				break;
		}
	}
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
	texture = aDrawable->texture;
	oglTexture = aDrawable->oglTexture;
}

int Drawable::getXBoundary()
{
	return posX; //+ (posXCorrection * scaleX);
}

int Drawable::getYBoundary()
{
	return posY; //+ (posYCorrection * scaleY);
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
/*
float Drawable::getUnscaledPosX()
{
	return posX;
}

float Drawable::getUnscaledPosY()
{
	return posY;
}
*/
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
	Drawable();
	animationUpdateFrequency = 40;
	currentAnimation = NULL;
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
	//textures = aDrawable->textures;
	//oglTextures = aDrawable->oglTextures;
	animationUpdateFrequency = aDrawable->animationUpdateFrequency;
	setAnimation("static");
	posXCorrection = aDrawable->posXCorrection;
	posYCorrection = aDrawable->posYCorrection;
}


void AnimatedDrawable::parseAnimationState(string aConf)
{
	istringstream aConfStream(aConf);
	string aState;
	animations.clear();

	while(getline(aConfStream, aState,'#'))
	{
		if(aState.size()!=0){
			istringstream nbFrame(aState);
			string aName;
			string aNumber;
			getline(nbFrame, aName, '$');
			//Get the second element (the one we are interested in)
			getline(nbFrame, aNumber, '$');

			Animation * anAnim = new Animation(this);
			anAnim->name = aName;
			anAnim->numberOfFrames = atoi(aNumber.c_str());

			if (currentAnimation == NULL)
			{
				currentAnimation = anAnim;
			}

			animations.insert(make_pair(anAnim->name, anAnim));
		}
	}
}

void AnimatedDrawable::loadFrom(string aConfString)
{
	istringstream aConfStream(aConfString);
	string aLine;
	string token;

	getline(aConfStream, aLine);

	istringstream myLine(aLine);
	getline(myLine, token, ';');
	name = token;

	while(getline(myLine, token, ';'))
	{
		istringstream aParam(token);
		string paramType;
		string paramValue;
		getline(aParam, paramType, ':');
		if(!paramType.compare("anim"))
		{
			getline(aParam, paramValue);
		}
		else
		{
			getline(aParam, paramValue, ':');
		}


		switch(confParameters.at(paramType))
		{
			case pName:
				name = paramValue;
				cout<< name << endl;
				break;

			case pWidth:
				width = atoi(paramValue.c_str());
				break;

			case pHeight:
				height = atoi(paramValue.c_str());
				break;

			case pTexture:
				loadTexture(paramValue.c_str());
				break;

			case pAnim:
				animations.at("static")->setAdditionalAnimationParameter(paramValue);
				break;

			case pAnimations:
				parseAnimationState(paramValue);
				break;

			case pBlending:
				if(!paramValue.compare("true"))
					toBlend = TRUE;
				break;

			default:
				cerr << "Drawable loadFrom(): Unknown configuration parameter: " << paramType << endl;
				break;
		}
	}

	//If there are several animations
	//Configure them
	while(getline(aConfStream, aLine))
	{
		istringstream myLine(aLine);
		getline(myLine, token, ';');
		istringstream aParam(token);
		string paramType;
		string paramValue;
		getline(aParam, paramType, ':');
		getline(aParam, paramValue, ':');

		if (!paramType.compare("anim"))
		{
			animations.at(paramValue)->setAnimationParameter(aLine);
		}
	}

	if(animations.empty())
	{
		Animation * anAnim = new Animation(this);
		anAnim->name = "static";
		anAnim->numberOfFrames = 1;
		animations.insert(make_pair(anAnim->name, anAnim));
	}

	for (std::map<string, Animation *>::const_iterator anElement = animations.begin(); anElement != animations.end(); ++anElement)
	{
		if((*anElement).second->texture == NULL)
		{
			(*anElement).second->texture = texture;
			(*anElement).second->oglTexture = oglTexture;
		}
	}
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
}
