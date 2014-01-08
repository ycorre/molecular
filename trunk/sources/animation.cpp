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
	loop = FALSE;
	currentFrame = 0;
	animationUpdateFrequency = 40;
	drawable = NULL;
	texture = NULL;
	oglTexture = 0;
	hasEnded = FALSE;
	moveTexture = TRUE;
	hasAnimatedTexture = TRUE;
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

	loop = anAnim->loop;
	currentFrame = 0;
	animationUpdateFrequency = anAnim->animationUpdateFrequency;
	drawable = NULL;
	hasEnded = FALSE;
}

Animation::Animation(Drawable * aDrawable)
{
	name = "animDefaut";
	width = aDrawable->width;
	height = aDrawable->height;
	texturePosX = 0;
	texturePosY = 0;
	numberOfFrames = 1;
	loop = FALSE;
	currentFrame = 0;
	animationUpdateFrequency = 40;
	drawable = aDrawable;
	texture = aDrawable->texture;
	hasEnded = FALSE;
	moveTexture = TRUE;
	hasAnimatedTexture = TRUE;
	oglTexture = aDrawable->oglTexture;
}

Animation::Animation(Json::Value aConfig, Drawable * aDrawable)
{
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
	loop = aConfig.get("loop", FALSE).asInt();

	animationUpdateFrequency = aConfig.get("animationUpdateFrequency", 40).asInt();
	moveTexture = aConfig.get("moveTexture", FALSE).asInt();
	hasAnimatedTexture = aConfig.get("animatedTexture", TRUE).asInt();

	texture = NULL;
	oglTexture = 0;
	drawable = aDrawable;

	string texturePath = aConfig.get("dataPath", "").asString();
	if(!texturePath.empty())
	{
		loadTexture(texturePath);
	}

	string opacityConfig = aConfig.get("opacity", "").asString();
	if(!opacityConfig.empty())
	{
		configOpacity(opacityConfig);
	}

	string scalingConfig = aConfig.get("scaling", "").asString();
	if(!scalingConfig.empty())
	{
		configScaling(scalingConfig);
	}

	currentFrame = 0;
	hasEnded = FALSE;
}

void Animation::incrementCurrentFrame()
{
	currentFrame = (currentFrame + 1) % numberOfFrames;
}

int Animation::nextFrame()
{

	incrementCurrentFrame();

	//If we have to move inside the texture
	if(hasAnimatedTexture)
	{
		//Compute the coordinate of the texture to display
		drawable->setAnimY((currentFrame / (drawable->texture->w/width)) * height);
		drawable->setAnimX((currentFrame % (drawable->texture->w/width)) * width);
	}

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

void Animation::setFrameTo(int aNumber)
{
	drawable->setAnimY((aNumber / (drawable->texture->w/width)) * height);
	drawable->setAnimX((aNumber % (drawable->texture->w/width)) * width);
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

//Opacity values for an animation is given as a series of pairs of number (x, y)
//*The x is the number of a key frame, where a specific value of opacity should be reached
//*The y is the actual opacity value to be taken at frame x (value is between 0.0 and 1.0)
//This is given in the configuration file as a string of numbers representing separated by comma
//Given the key frames, the opacity values and the number of frame in the animation
//we compute for each frame of the animation the opacity value, by computing the value for each frame between two key frames
//assuming that the opacity is increasing (or decreasing) in a linear way
void Animation::configOpacity(string aConfigString)
{
	string aFrameNumber;
	istringstream aConf(aConfigString);
	unsigned int i, j;
	map<int, float> opacityTempValues;
	vector<int> frameIndices;
	float startingOpacity;
	float currentOpacity;
	float finishingOpacity;
	float opacityModifyingFactor;

	//Parse the configuration string to get pair of values
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

	//Compute all the opacity values to be taken for each frame
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

//Works similarly to configOpacity (see above), except that scaling value can go higher than 1.0
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

	//Parse the configuration string to get pair of values
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

	//Compute all the scaling values to be taken for each frame
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
