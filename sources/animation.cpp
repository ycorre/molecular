/*
 * Animation
 */

#include "animation.h"

Animation::Animation()
{
	name = "AnimDfault";
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
	moveTexture = TRUE;
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

	loop = anAnim->loop;
	currentFrame = 0;
	animationUpdateFrequency = anAnim->animationUpdateFrequency;
	drawable = NULL;
	hasEnded = FALSE;
}

Animation::Animation(Drawable * aDrawable)
{
	name = "AnimDfault";
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
	moveTexture = TRUE;
	oglTexture = 0;
}

int Animation::nextFrame()
{
	currentFrame = (currentFrame + 1) % numberOfFrames;

	//Compute the position in the texture
	drawable->setAnimY((currentFrame / (drawable->texture->w/width)) * height);
	drawable->setAnimX((currentFrame % (drawable->texture->w/width)) * width);

	hasEnded = FALSE;

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

			case pMoveTexture:
				if(!paramValue.compare("false"))
					moveTexture = FALSE;
				break;

			case pOpacity:
				break;

			case pNumberOfFrames:
				numberOfFrames = atoi(paramValue.c_str());
				break;

			default:
				cerr << "Animation loadFrom(): Unknown configuration parameter: " << paramType << endl;
				break;
		}
	}
}

void Animation::setAdditionalAnimationParameter(string aConfigString)
{
	string token;
	istringstream aConf(aConfigString);

	while(getline(aConf, token, '/'))
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

			case pMoveTexture:
				if(!paramValue.compare("false"))
					moveTexture = FALSE;
				break;

			case pOpacity:
				configOpacity(paramValue);
				break;

			case pScale:
				configScaling(paramValue);
				break;

			case pNumberOfFrames:
				numberOfFrames =  atoi(paramValue.c_str());
				break;

			default:
				cerr << "Animation loadFrom(): Unknown configuration parameter: " << paramType << endl;
				break;
		}
	}
}


void Animation::loadTexture(string path)
{
	texture = drawable->ge->loadTexture(path);

#if USE_OPENGL
	oglTexture = drawable->ge->openGLTextures.at(texture);
	drawable->setAnimX(drawable->getAnimX());
	drawable->setAnimY(drawable->getAnimY());
#endif
}

void Animation::configOpacity(string aConfigString)
{
	string aFrameNumber;
	istringstream aConf(aConfigString);
	int i, j;
	map<int, float> opacityTempValues;
	vector<int> frameIndices;
	float startingOpacity;
	float currentOpacity;
	float finishingOpacity;
	float opacityModifyingFactor;
	while(getline(aConf, aFrameNumber, ','))
	{
		string anOpacityValue;
		getline(aConf, anOpacityValue, ',');
		opacityTempValues.insert(make_pair(atoi(aFrameNumber.c_str()), atof(anOpacityValue.c_str())));
		frameIndices.push_back(atoi(aFrameNumber.c_str()));
	}

	j = 0;
	startingOpacity = opacityTempValues.at(frameIndices[j]);
	finishingOpacity = opacityTempValues.at(frameIndices[j+1]);
	opacityModifyingFactor = (finishingOpacity - startingOpacity) / (frameIndices[j+1] - frameIndices[j]);
	currentOpacity = startingOpacity;
	j++;

	for (i = 0; i <numberOfFrames; i++)
	{
		opacityValues.push_back(currentOpacity);
		if(j < frameIndices.size() - 1)
		{
			if(i == frameIndices[j])
			{
				startingOpacity = opacityTempValues.at(frameIndices[j]);
				finishingOpacity = opacityTempValues.at(frameIndices[j+1]);
				opacityModifyingFactor = (finishingOpacity - startingOpacity) / (frameIndices[j+1] - frameIndices[j]);
				currentOpacity = startingOpacity;
				j++;
			}
			currentOpacity = currentOpacity + opacityModifyingFactor;
		}
		else
		{
			if(i < frameIndices[j])
				currentOpacity = currentOpacity + opacityModifyingFactor;
		}
	}
}

void Animation::configScaling(string aConfString)
{
	string aFrameNumber;
	istringstream aConf(aConfString);
	int i, j;
	map<int, float> scaleTempValues;
	vector<int> frameIndices;
	float startingScale;
	float currentScale;
	float finishingScale;
	float scaleModifyingFactor;

	while(getline(aConf, aFrameNumber, ','))
	{
		string anOpacityValue;
		getline(aConf, anOpacityValue, ',');
		scaleTempValues.insert(make_pair(atoi(aFrameNumber.c_str()), atof(anOpacityValue.c_str())));
		frameIndices.push_back(atoi(aFrameNumber.c_str()));
	}

	j = 0;
	startingScale = scaleTempValues.at(frameIndices[j]);
	finishingScale = scaleTempValues.at(frameIndices[j+1]);
	scaleModifyingFactor = (finishingScale - startingScale) / (frameIndices[j+1] - frameIndices[j]);
	currentScale = startingScale;
	j++;

	for (i = 0; i <numberOfFrames; i++)
	{
		scalingValues.push_back(currentScale);
		if(j < frameIndices.size() - 1)
		{
			if(i == frameIndices[j])
			{
				startingScale = scaleTempValues.at(frameIndices[j]);
				finishingScale = scaleTempValues.at(frameIndices[j+1]);
				scaleModifyingFactor = (finishingScale - startingScale) / (frameIndices[j+1] - frameIndices[j]);
				currentScale = startingScale;
				j++;
			}
			currentScale = currentScale + scaleModifyingFactor;
		}
		else
		{
			if(i < frameIndices[j])
				currentScale = currentScale + scaleModifyingFactor;
		}
	}
}
