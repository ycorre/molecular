#include "effect.h"

Effect::Effect()
{
	width = 0;
	height = 0;
	posX = 0;
	posY = 0;
	state = 0;
	setAnimX(0);
	setAnimY(0);
}

Effect::Effect(int x, int y, string typeEffect)
{
	Drawable();
	typeEffect = "effect" + typeEffect;
	this->addTexture(typeEffect);
	width = atoi(((lev->configurationElements.at(typeEffect)).at(0)).c_str());
	height = atoi(((lev->configurationElements.at(typeEffect)).at(1)).c_str());
	posX = x;
	posY = y;
	state = 0;
	type = typeEffect;
	setAnimX(0);
	setAnimY(0);
	parseAnimationState((lev->configurationElements.at(typeEffect)).at(2));
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
		if((*aLayer).second->updateAnimationFrame())
		{
			isUpdated = TRUE;
		}
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

	while(getline(aConf, line))
	{
		if(!line.empty()) //Ignore empty lines
		{
			istringstream myLine(line);
			AnimatedDrawable * aLayer = new AnimatedDrawable();
			getline(myLine, token, ';');
			aLayer->name = token;
			getline(myLine, token, ';');
			aLayer->width = atoi(token.c_str());
			getline(myLine, token, ';');
			aLayer->height = atoi(token.c_str());
			getline(myLine, token, ';');
			aLayer->parseAnimationState(token);
			getline(myLine, token, ';');
			aLayer->loadTexture(token);
			aLayer->toBlend = TRUE;
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
		(*aLayer).second->posX = x + (*aLayer).second->posXCorrection;
		(*aLayer).second->posY = y + (*aLayer).second->posYCorrection;
		(*aLayer).second->display = TRUE;
		frameCount = 0;
	}
}
