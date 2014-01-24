/*
 * Animation
 */

#include "animation.h"

Animation::Animation()
{
	name = "animDefaut";
	width = 1;
	height = 1;
	texturePosX = 0;
	texturePosY = 0;
	numberOfFrames = 1;
	loop = false;
	currentFrame = 0;
	animationUpdateFrequency = 40;
	drawable = NULL;
	texture = NULL;
	oglTexture = 0;
	rotationSpeed = 0;
	hasEnded = false;
	moveTexture = true;
	hasAnimatedTexture = true;
	reverse = false;
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
	moveTexture = anAnim->moveTexture;
	opacityValues = anAnim->opacityValues;
	scalingValues = anAnim->scalingValues;
	hasAnimatedTexture = anAnim->hasAnimatedTexture;
	reverse = anAnim->reverse;
	rotationSpeed = anAnim->rotationSpeed;

	loop = anAnim->loop;
	currentFrame = 0;
	animationUpdateFrequency = anAnim->animationUpdateFrequency;
	drawable = NULL;
	hasEnded = false;
}

Animation::Animation(Drawable * aDrawable)
{
	name = "animDefaut";
	width = aDrawable->width;
	height = aDrawable->height;
	texturePosX = 0;
	texturePosY = 0;
	numberOfFrames = 1;
	loop = false;
	currentFrame = 0;
	animationUpdateFrequency = 40;
	drawable = aDrawable;
	texture = aDrawable->texture;
	hasEnded = false;
	moveTexture = true;
	hasAnimatedTexture = true;
	oglTexture = aDrawable->oglTexture;
	rotationSpeed = 0;
	reverse = false;
}

Animation::Animation(Json::Value aConfig, Drawable * aDrawable)
{
	unsigned int i;

	name = aConfig.get("name", "animDefaut").asString();
	width = aConfig.get("width", 0).asInt();
	//If no width was specified
	if (width == 0)
		width = aDrawable-> width;

	height = aConfig.get("height", 0).asInt();
	if (height == 0)
		height = aDrawable-> height;

	texturePosX = aConfig.get("texturePosX", 0).asFloat();
	texturePosY = aConfig.get("texturePosY", 0).asFloat();
	numberOfFrames = aConfig.get("numberOfFrames", 1).asInt();
	loop = aConfig.get("loop", false).asBool();

	animationUpdateFrequency = aConfig.get("animationUpdateFrequency", 40).asInt();
	moveTexture = aConfig.get("moveTexture", false).asBool();
	reverse = aConfig.get("reverse", false).asBool();
	hasAnimatedTexture = aConfig.get("animatedTexture", true).asBool();
	rotationSpeed = aConfig.get("rotationSpeed", 0).asFloat();

	texture = NULL;
	oglTexture = 0;
	drawable = aDrawable;

	string texturePath = aConfig.get("dataPath", "").asString();
	if(!texturePath.empty())
	{
		loadTexture(texturePath);
	}

	Json::Value opacityConfigValue = aConfig["opacity"];
	vector<float> opacityConfig;
	for(i = 0; i < opacityConfigValue.size(); i++)
		opacityConfig.push_back(opacityConfigValue[i].asFloat());
	if(!opacityConfig.empty())
	{
		opacityValues = computeLinearValue(opacityConfig, numberOfFrames);
	}

	Json::Value scalingConfigValue = aConfig["scaling"];
	vector<float> scalingConfig;
	for(i = 0; i < scalingConfigValue.size(); i++)
		scalingConfig.push_back(scalingConfigValue[i].asFloat());
	if(!scalingConfig.empty())
	{
		scalingValues = computeLinearValue(scalingConfig, numberOfFrames);
	}

	currentFrame = 0;
	hasEnded = false;
}

void Animation::incrementCurrentFrame()
{
	if(reverse)
	{
		currentFrame = (currentFrame + numberOfFrames - 1) % numberOfFrames;
	}
	else
	{
		currentFrame = (currentFrame + 1) % numberOfFrames;
	}
}

int Animation::nextFrame()
{
	incrementCurrentFrame();

	//If we have to move inside the texture
	if(hasAnimatedTexture)
	{
		//Compute the coordinate of the texture to display
		drawable->setAnimY((currentFrame / (int)(drawable->texture->w/width)) * height);
		drawable->setAnimX((currentFrame % (drawable->texture->w/(int)width)) * width);
	}

	hasEnded = false;

	//If the current frame has an opacity value set
	if(!opacityValues.empty())
	{
		drawable->opacity = opacityValues.at(currentFrame);
	}

	//If the current frame has a scaling value set
	if(!scalingValues.empty())
	{
		drawable->scaleX = scalingValues.at(currentFrame);
		drawable->scaleY = scalingValues.at(currentFrame);
	}

	//If we are at the end of the animation, set the flag
	if(currentFrame == numberOfFrames - 1 && !loop)
	{
		hasEnded = true;
	}

	//If we are rotating the texture
	if(rotationSpeed != 0)
	{
		drawable->rotationAngle = drawable->rotationAngle + rotationSpeed;
	}

	return currentFrame;
}

void Animation::setFrameTo(int aNumber)
{
	drawable->setAnimY((aNumber / (int)(drawable->texture->w/width)) * height);
	drawable->setAnimX((aNumber % (drawable->texture->w/(int)width)) * width);
}

void Animation::loadTexture(string path)
{
	texture = drawable->ge->loadTexture(path, drawable->clampTexture);

	oglTexture = drawable->ge->openGLTextures.at(texture);
	drawable->setAnimX(drawable->getAnimX());
	drawable->setAnimY(drawable->getAnimY());
}
