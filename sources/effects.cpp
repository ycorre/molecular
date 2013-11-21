#include "effects.h"

Effect::Effect()
{
	width = texture->w;
	height = texture->h;
	posX = 0;
	posY = 0;
	state = 0;
	setAnimX(0);
	setAnimY(0);
}


Effect::Effect(int x, int y, string typeEffect)
{
	Drawable();
	this->addTexture(typeEffect);
	width = atoi(((lev->configurationElements.at("explosion")).at(0)).c_str());
	height = atoi(((lev->configurationElements.at("explosion")).at(1)).c_str());
	posX = x;
	posY = y;
	state = 0;
	type = typeEffect;
	setAnimX(0);
	setAnimY(state * height);
	parseAnimationState((lev->configurationElements.at("explosion")).at(2));
}

Effect::~Effect()
{

}

void Effect::animate()
{
	updateAnimationFrame();
	if (currentFrame == 0)
	{
		display = FALSE;
		toRemove = TRUE;

	}
	posX = posX - lev->cameraSpeed;
}
