#include "effect.h"

Effect::Effect()
{
	width = 0;
	height = 0;
	posX = 0;
	posY = 0;
	setAnimX(0);
	setAnimY(0);
}

Effect::Effect(int x, int y, string aName)
{
	name = aName;
	width = lev->loadedEffects.at(aName)->width;
	height = lev->loadedEffects.at(aName)->height;
	isMoving = lev->loadedEffects.at(aName)->isMoving;

	for (map<string, AnimatedDrawable *>::iterator aLayer = lev->loadedEffects.at(aName)->effectLayers.begin(); aLayer != lev->loadedEffects.at(aName)->effectLayers.end(); ++aLayer)
	{
		AnimatedDrawable * anAnimatedDrawable = new AnimatedDrawable();
		anAnimatedDrawable->copyFrom((*aLayer).second);
		if(anAnimatedDrawable->name.compare(0,8, "effect_c") == 0)
		{
			anAnimatedDrawable->posX = x + 32;
			anAnimatedDrawable->posY = y + 32;
		}
		else
		{
			anAnimatedDrawable->posX = x;
			anAnimatedDrawable->posY = y;
		}

		anAnimatedDrawable->display = TRUE;
		effectLayers.insert(make_pair(anAnimatedDrawable->name, anAnimatedDrawable));
	}

	frameCount = 0;
}

Effect::~Effect()
{

}

int Effect::animateEffect()
{
	int isFinished = TRUE;
	int isUpdated = FALSE;

	for (map<string, AnimatedDrawable *>::iterator aLayer = effectLayers.begin(); aLayer != effectLayers.end(); ++aLayer)
	{
		if((*aLayer).second->display)
		{
			if((*aLayer).second->updateAnimationFrame())
			{
				isUpdated = TRUE;
				if((*aLayer).second->name.compare(0, 4, "load") == 0)
				{
					(*aLayer).second->posY--;
				}
			}

			if(isMoving)
				(*aLayer).second->posX--;

			if((*aLayer).second->currentAnimation->hasEnded)
			{
				(*aLayer).second->display = FALSE;
			}
			else
			{
				(*aLayer).second->processDisplay();
				isFinished = FALSE;
			}
		}
	}

	if(isUpdated)
		frameCount++;

	return isFinished;
}

void Effect::loadConf(string aConfigString)
{
	string line;
	string token;
	istringstream aConf(aConfigString);
	getline(aConf, line);
	istringstream myLine(line);
	getline(myLine, token, ';');
	name = token;
	getline(myLine, token, ';');
	width = atoi(token.c_str());
	getline(myLine, token, ';');
	height = atoi(token.c_str());
	getline(myLine, token, ';');
	if(!token.compare("true"))
	{
		isMoving = TRUE;
	}
	else
	{
		isMoving = FALSE;
	}

	while(getline(aConf, line))
	{
		if(!line.empty()) //Ignore empty lines
		{
			istringstream myLine(line);
			AnimatedDrawable * aLayer = new AnimatedDrawable();
			aLayer->loadFrom(line);

			aLayer->setWidth(aLayer->width);

			aLayer->posXCorrection = width / 2 - aLayer->width / 2;
			aLayer->posYCorrection = height / 2 - aLayer->height / 2;
			effectLayers.insert(make_pair(aLayer->name, aLayer));
		}
	}
}

void Effect::startEffect(int x, int y)
{
	for (map<string, AnimatedDrawable *>::iterator aLayer = effectLayers.begin(); aLayer != effectLayers.end(); ++aLayer)
	{
		(*aLayer).second->posX = x;
		(*aLayer).second->posY = y;
		(*aLayer).second->display = TRUE;
		(*aLayer).second->animationUpdateFrequency = 400;
	}
	frameCount = 0;
}
