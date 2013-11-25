/*
 * Animation
 */

#include "drawable.h"

Animation::Animation()
{
	name = "";
	width = 1;
	height = 1;
	texturePosX = 0;
	texturePosY = 0;
	numberOfFrames = 1;
	loop = FALSE;
	currentFrame = 0;
	animationUpdateFrequency = 40;
	drawable = NULL;
	texture = NULL;
	oglTexture = 0;
	hasEnded = FALSE;
}

Animation::Animation(Animation * anAnim)
{
	name = anAnim->name;
	width = anAnim->width;
	height = anAnim->height;
	texturePosX = anAnim->texturePosX;
	texturePosY = anAnim->texturePosY;
	texture = anAnim->texture;
	oglTexture = anAnim->oglTexture;
	numberOfFrames = anAnim->numberOfFrames;
	loop = anAnim->loop;
	currentFrame = anAnim->currentFrame;
	animationUpdateFrequency = anAnim->animationUpdateFrequency;
	drawable = NULL;
	hasEnded = FALSE;
}

Animation::Animation(Drawable * aDrawable)
{
	name = "";
	width = aDrawable->width;
	height = aDrawable->height;
	texturePosX = 0;
	texturePosY = 0;
	numberOfFrames = 1;
	loop = FALSE;
	currentFrame = 0;
	animationUpdateFrequency = 40;
	drawable = aDrawable;
	texture = NULL;
	hasEnded = FALSE;
	oglTexture = 0;
}

int Animation::nextFrame()
{
	currentFrame = (currentFrame + 1) % numberOfFrames;

	//Compute the position in the texture
	drawable->setAnimY((currentFrame / (drawable->texture->w/width)) * height);
	drawable->setAnimX((currentFrame % (drawable->texture->w/width)) * width);

	hasEnded = FALSE;

	if (currentFrame == numberOfFrames - 1 && !loop)
	{
		hasEnded = TRUE;
	}

	return currentFrame;
}

void Animation::setAnimationParameter(string aConfigString)
{
	string token;
	istringstream aConf(aConfigString);

	while(getline(aConf, token, ';'))
	{
		istringstream aParam(token);
		string paramType;
		string paramValue;
		getline(aParam, paramType, ':');
		getline(aParam, paramValue, ':');

		switch(drawable->confParameters.at(paramType))
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
				break;

			case pAnimations:
				break;

			case pNumberOfFrames:
				numberOfFrames =  atoi(paramValue.c_str());
				break;

			default:
				cerr << "Drawable loadFrom(): Unknown configuration parameter: " << paramType << endl;
				break;
		}
	}
}


void Animation::loadTexture(string path)
{
	texture = drawable->ge->loadTexture(path);
	drawable->textures.insert(make_pair(name, texture));

#if USE_OPENGL
	oglTexture = drawable->ge->openGLTextures.at(texture);
	drawable->oglTextures.insert(make_pair(name, drawable->ge->openGLTextures.at(texture)));
	drawable->setAnimX(drawable->getAnimX());
	drawable->setAnimY(drawable->getAnimY());
#endif
}

