#ifndef PARTICULEEFFECTS_H
#define PARTICULEEFFECTS_H

#include "graphicEngine.h"

class LineEffect;
class PointEffect;
class Emitter;

//Particle effect can be made of several types of effect (line, point, circle, etc.)
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
		int animationLength; //
		int animationUpdateFrequency, lastTimeUpdated;
		int lifeTime;
		int numberOfParticle;
		float speed;
		Emitter * particleEmitter;
		//decroissance de la vitesse

		int currentFrame;
		list<LineEffect *> lineEffects;
		list<PointEffect *> pointEffects;
		vector<float> opacityValues;
		vector<vector<float> > colorValues;

		ParticleEffect();
		~ParticleEffect();
		virtual void animate();
		void setOpacityValues(float startingValue, float endingValue);
		void setColorValues(vector<float> startingColorValue, vector<float> endingColorValue);

		void createImpactFrom(float posX, float posY);
		void createExplosionFrom(float x, float y);
		void createSparkFrom(float x, float y);
};

class LineEffect: public ParticleEffect
{
	public:
		float length;
		float lineWidth;
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

class PointEffect: public ParticleEffect
{
	public:

		PointEffect();
		virtual void animate();
		void createRandomPointFrom(float aPosX, float aPosY);
		void createRandomPointFrom(float aPosX, float aPosY, float anAngleLowBound, float anAngleHighBound);
};

class Emitter
{
	public:
		float posX, posY;

		Emitter();
		Emitter(float x, float y);
		virtual pair<float, float> getParticlePosition();
		virtual list<pair<float, float> > getParticlePosition(int numberOfParticles);
};

class SquareEmitter: Emitter
{
	public:
		float destX, destY;

		SquareEmitter(float originX, float originY, float cornerX, float cornerY);
		virtual pair<float, float> getParticlePosition();
		//virtual list<float, float> getParticlePosition(int numberOfParticles);
};


class CircleEmitter: Emitter
{
	public:
		float radius;

		CircleEmitter(float centerX, float centerY, float radius);
		virtual pair<float, float> getParticlePosition();
		//virtual list<float, float> getParticlePosition(int numberOfParticles);
};

#endif

