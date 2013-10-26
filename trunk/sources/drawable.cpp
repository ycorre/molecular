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
	nbFrames.push_back(1);
	state = 0;
	posX = 0;
	posY = 0;
	animX = 0;
	animY = 0;
	width = 1;
	height = 1;
	display = TRUE;
	toRemove = FALSE;
	isBlinking = FALSE;
	blinkingCounter = 0;
}

Drawable::~Drawable()
{

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

//Change the animation to display
int Drawable::updateAnimationFrame()
{
	//41 ms ~= 24 FPS
	//33 ms ~= 30 FPS
	unsigned int updateTime = lastTimeUpdated + 33;
	if (updateTime < SDL_GetTicks())
	{
		//shift one sprite to the right; if we are at the end then go back to the beginning
		animX = (animX + width) % (nbFrames.at(state)*width);
		lastTimeUpdated = SDL_GetTicks();
		return 1;
	}

	return 0;
}

vector<int> parseAnimationState(string aConf)
{
	istringstream aConfStream(aConf);
	string aState;
	vector<int> nbFrames;

	while(getline(aConfStream, aState,'#'))
	{
		if(aState.size()!=0){
			istringstream nbFrame(aState);
			string aNumber;
			getline(nbFrame, aNumber, '$');
			//Get the second element (the one we are interested in)
			getline(nbFrame, aNumber, '$');
			nbFrames.push_back(atoi(aNumber.c_str()));
		}
	}

	return nbFrames;
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

void Drawable::blink()
{
	if (blinkingCounter == 0)
	{
		//Blinking effect showing that we are invincible
		display = (display + 1) % 2;
	}
	blinkingCounter = (blinkingCounter + 1) % 3;
}
