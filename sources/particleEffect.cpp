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
	colorR = colorG = colorB = 0;
	animationUpdateFrequency = 40;
	lastTimeUpdated = 0;
	particleEmitter = NULL;
	lifeTime = 1;
	speed = 0;
	numberOfParticle = 0;
}

ParticleEffect::~ParticleEffect()
{
	lineEffects.clear();
	pointEffects.clear();
}

//Set opacity in linear decreasing from the starting value to the finishing one
void ParticleEffect::setOpacityValues(float startingOpacity, float finishingOpacity)
{
	int i;
	float currentOpacity;
	float opacityModifyingFactor;

	opacityModifyingFactor = (finishingOpacity - startingOpacity) / animationLength;
	currentOpacity = startingOpacity;

	for (i = 0; i < animationLength; i++)
	{
		opacityValues.push_back(currentOpacity);
		currentOpacity = currentOpacity + opacityModifyingFactor;
	}
}

void ParticleEffect::animate()
{
	//if(currentFrame != animationLength-1)
	unsigned int updateTime = lastTimeUpdated + animationUpdateFrequency;
	if (updateTime < ProgramTimer)
	{
		for (list<PointEffect *>::iterator aPoint = pointEffects.begin(); aPoint != pointEffects.end(); ++aPoint)
			{
				(*aPoint)->animate();
			}

		if(!opacityValues.empty())
		{
			opacity = opacityValues.at(currentFrame);
		}

		currentFrame++;
		lastTimeUpdated = ProgramTimer;
	}
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
	animationLength = 3;
	currentFrame = 0;
	setOpacityValues(1, 0.5);
}

void ParticleEffect::createExplosionFrom(float x, float y)
{
	int i = 0;
	colorR = 125.0/255.0;
	colorG = 169.0/255.0;
	colorB = 255.0/255.0;
	for(i = 0; i < 1000; i++)
	{
		PointEffect * aPoint = new PointEffect();
		aPoint->createRandomPointFrom(x, y);
		pointEffects.push_back(aPoint);
	}
	animationLength = 75;
	currentFrame = 0;
	setOpacityValues(1, 0.0);
}

LineEffect::LineEffect()
{
	length = 1;
	destX = destY = 0;
	center = make_pair(0,0);
	lineWidth = 2.0f;
}

void LineEffect::createRandomLineFrom(float aPosX, float aPosY)
{
	center = make_pair(aPosX, aPosY);
	posX = aPosX;
	posY = aPosY;
	destX = aPosX - (rand() % 70);
	destY = aPosY + (- 35 + rand() % 70);
	lineWidth = 2.0 + ((rand() % 40)/10.0);
}

CircleEffect::CircleEffect()
{
	radius = 0;
	slice = 0;
	center = make_pair(0,0);
}

PointEffect::PointEffect()
{

}

void PointEffect::createRandomPointFrom(float aPosX, float aPosY)
{
	posX = aPosX;
	posY = aPosY;
	angle = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/360.0f)) * (PI/180);
	speed = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/20.0f));
}

void PointEffect::animate()
{
	float vx, vy;
	vx = speed*cos(angle);
	vy = speed*sin(angle);

	posX = posX + vx;
	posY = posY + vy;

	speed = speed / 1.15f;

	if(!opacityValues.empty())
	{
		opacity = opacityValues.at(currentFrame);
	}

	currentFrame++;
}

/*
 * Emitters functions
 */

Emitter::Emitter()
{
	posX = 0;
	posY = 0;
}

Emitter::Emitter(float aX, float aY)
{
	posX = aX;
	posY = aY;
}

pair<float, float> Emitter::getParticlePosition()
{
	return make_pair(posX, posY);
}

list<pair<float, float> >  Emitter::getParticlePosition(int numberOfParticles)
{
	int i = 0;
	list<pair<float, float> > aList;

	for(i = 0; i < numberOfParticles; i++)
	{
		aList.push_back(this->getParticlePosition());
	}

	return aList;
}

SquareEmitter::SquareEmitter(float originX, float originY, float cornerX, float cornerY)
{
	posX = originX;
	posY = originY;
	destX = cornerX;
	destY = cornerY;
}

pair<float, float> SquareEmitter::getParticlePosition()
{
	float diffX = destX - posX;
	float diffY = destY - posY;

	float x = posX + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/diffX));
	float y = posY + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/diffY));

	return make_pair(x, y);
}

CircleEmitter::CircleEmitter(float centerX, float centerY, float aRadius)
{
	posX = centerX;
	posY = centerY;
	radius = aRadius;
}

//X coordinate given by radius*cos(theta)
pair<float, float> CircleEmitter::getParticlePosition()
{
	float angle = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(2.0f*PI)));
	float aRadius = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/radius));

	float x = aRadius * cos(angle);
	float y = aRadius * sin(angle);

	return make_pair(x, y);
}

