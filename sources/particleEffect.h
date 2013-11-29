#ifndef PARTICULEEFFECTS_H
#define PARTICULEEFFECTS_H

#include "graphicEngine.h"

class LineEffect;

class ParticleEffect
{
	public:
		string type;
		float opacity;
		float scaleX, scaleY;
		float rotX, rotY, rotZ;
		float colorR, colorG, colorB;
		float angle;
		float posX, posY;
		int animationLength;
		int currentFrame;
		list<LineEffect *> lineEffects;

		ParticleEffect();
		~ParticleEffect();
		void createImpactFrom(float posX, float posY);
};

class LineEffect: public ParticleEffect
{
	public:
		float length;
		float destX, destY;
		pair<float, float> center;

		LineEffect();
		void createRandomLineFrom(float posX, float posY);
};

class CircleEffect: public ParticleEffect
{
	public:
		float radius;
		int slice;
		pair<float, float> center;

		CircleEffect();
};

#endif

