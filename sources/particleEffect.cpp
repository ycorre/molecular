#include "particleEffect.h"


ParticleEffect::ParticleEffect()
{
	type = "";
	opacity = 1;
	scaleX = scaleY = 1;
	rotX = rotY = rotZ = 0;
	posX = posY = 0;
	angle = 0;
	animationLength = 0;
	currentFrame = 0;
}

ParticleEffect::~ParticleEffect()
{
	lineEffects.clear();
}

//Create impact from 3 lines
void ParticleEffect::createImpactFrom(float x, float y)
{
	int i = 0;
	colorR = 51.0/255.0;
	colorG = 118.0/255.0;
	colorB = 208.0/255.0;
	for(i = 0; i < 3; i++)
	{
		LineEffect * aLine = new LineEffect();
		aLine->createRandomLineFrom(x, y);
		lineEffects.push_back(aLine);
	}
	animationLength = 5;
	currentFrame = 0;
}

LineEffect::LineEffect()
{
	length = 1;
	destX = destY = 0;
	center = make_pair(0,0);
}

void LineEffect::createRandomLineFrom(float aPosX, float aPosY)
{
	center = make_pair(aPosX, aPosY);
	posX = aPosX;
	posY = aPosY;
	destX = aPosX - (rand() % 70);
	destY = aPosY + (- 35 + rand() % 70);
}

CircleEffect::CircleEffect()
{
	radius = 0;
	slice = 0;
	center = make_pair(0,0);
}



