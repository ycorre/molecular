#include "effect.h"

Effect::Effect()
{
	name = "effectDefaut";
	width = 0;
	height = 0;
	posX = 0;
	posY = 0;
	movingSpeed = 0;
	numberOfLoops = 0;
	followsAnObject = false;
	frameCount = 0;
	isMoving = false;
	setAnimX(0);
	setAnimY(0);
}

Effect::Effect(int x, int y, string aName)
{
	name = aName;
	width = CurrentLevel->loadedEffects.at(aName)->width;
	height = CurrentLevel->loadedEffects.at(aName)->height;
	isMoving = CurrentLevel->loadedEffects.at(aName)->isMoving;
	movingSpeed = 0.1;
	numberOfLoops = 0;
	followsAnObject = false;

	for (map<string, AnimatedDrawable *>::iterator aLayer = CurrentLevel->loadedEffects.at(aName)->effectLayers.begin(); aLayer != CurrentLevel->loadedEffects.at(aName)->effectLayers.end(); ++aLayer)
	{
		AnimatedDrawable * anAnimatedDrawable = new AnimatedDrawable();
		anAnimatedDrawable->copyFrom((*aLayer).second);

		anAnimatedDrawable->posX = x;
		anAnimatedDrawable->posY = y;

		anAnimatedDrawable->display = true;
		effectLayers.insert(make_pair(anAnimatedDrawable->name, anAnimatedDrawable));
	}

	frameCount = 0;
}

Effect::Effect(Json::Value aConfig)
{
	unsigned int index;

	name = aConfig.get("name", "effectDefaut").asString();
	width = aConfig.get("width", 1).asInt();
	height = aConfig.get("height", 1).asInt();
	isMoving = aConfig.get("move", false).asBool();
	movingSpeed = aConfig.get("speed", 0.1f).asFloat();
	numberOfLoops = aConfig.get("loop", 0).asInt();
	followsAnObject = aConfig.get("follows", false).asBool();

	Json::Value layers = aConfig["effectLayer"];
	for (index = 0; index < layers.size(); ++index)
	{
		AnimatedDrawable * tmp = new AnimatedDrawable(layers[index]);
		tmp->posXCorrection = 1;//tmp->width / 2 - width / 2;
		tmp->posYCorrection = 1;//tmp->height / 2 - height / 2;

		effectLayers.insert(make_pair(tmp->name, tmp));
	}

	frameCount = 0;
}

Effect::~Effect()
{

}

int Effect::animateEffect()
{
	bool isFinished = true;
	bool isUpdated = false;

	for (map<string, AnimatedDrawable *>::iterator aLayer = effectLayers.begin(); aLayer != effectLayers.end(); ++aLayer)
	{
		if((*aLayer).second->display)
		{
			if((*aLayer).second->updateAnimationFrame())
			{
				isUpdated = true;
			}

			if(isMoving)
			{
				movingSpeed = min(1.5, movingSpeed + 0.01);
				(*aLayer).second->posX = (*aLayer).second->posX - movingSpeed;
			}

			if((*aLayer).second->currentAnimation->hasEnded)
			{
				(*aLayer).second->display = false;
			}
			else
			{
				(*aLayer).second->processDisplay();
				isFinished = false;
			}
		}
	}

	if(isUpdated)
		frameCount++;

	return isFinished;
}

/*
 * TextEffect functions
 */
TextEffect::TextEffect(int x, int y, string aText)
{
	color = ge->availableColors.at("WHITE");
	font = ge->availableFonts.at("lCrystal_16");

	name = aText;
	width = 300;
	height = 300;
	isMoving = false;
	posX = x;
	posY = y;
	display = true;

	Animation * aNewAnim = new Animation(this);
	aNewAnim->texture = texture;
	aNewAnim->oglTexture = oglTexture;
	aNewAnim->numberOfFrames = 36;

	float initArray[6] = {0,1,24,1,36,0};
	vector<float> opacityValue(&initArray[0], &initArray[0]+6);
	aNewAnim->configOpacity(opacityValue);
	animations.insert(make_pair(name, aNewAnim));
	currentAnimation = aNewAnim;

	write(aText);
	currentAnimation->width = width;
	currentAnimation->height = height;
	aNewAnim->texture = texture;
	frameCount = 0;
}

void TextEffect::animate()
{
	if(updateAnimationFrame())
	{
		posY--;
	}

	if(isMoving)
		posX--;

	if(!currentAnimation->hasEnded)
	{
		processDisplay();
	}
	else
	{
		toRemove = true;
	}
}
