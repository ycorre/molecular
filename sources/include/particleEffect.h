#ifndef PARTICULEEFFECTS_H
#define PARTICULEEFFECTS_H

#include "graphicEngine.h"

class LineEffect;
class PointEffect;
class CircleEffect;
class Emitter;
class Drawable;
class AnimatedDrawable;
/*
class ParticleEffectFactory
{
	public:
		map<string, EnemyType> particleEffects;

		ParticleEffectFactory(){};
		static ParticleEffectFactory * instance();
		ParticleEffect * createEnemy(string anEnemyName);
		ParticleEffect * createEnemy(Json::Value anEnemyConf);
};*/

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
		float speed;
		Emitter * particleEmitter;

		int currentFrame;
		list<LineEffect *> lineEffects;
		list<CircleEffect *> circleEffects;
		list<PointEffect *> pointEffects;
		vector<float> pointCoordinateValues;
		vector<float> pointColorValues;
		vector<float> opacityValues;
		vector<vector<float> > colorValues;

		map<string, particleType> particleTypes;

		ParticleEffect();
		virtual ~ParticleEffect();
		virtual void animate();
		void instantiateEffects(Json::Value aConfig, float x, float y);
		void instantiatePointEffects(Json::Value aConfig, float x, float y);
		void instantiateLineEffects(Json::Value aConfig, float x, float y);
		void instantiatecircleEffects(Json::Value aConfig, float x, float y);
		void setColorValues(vector<float> startingColorValue, vector<float> endingColorValue);
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
		float slices;
		pair<float, float> center;

		CircleEffect();
		CircleEffect(float x, float y, float r);
};

class PointEffect: public ParticleEffect
{
	public:
		float size;
		float decelerationFactor;

		PointEffect();
		void animate();
		void createRandomPointFrom(float aPosX, float aPosY);
		void createRandomPointFrom(float aPosX, float aPosY, float anAngleLowBound, float anAngleHighBound);
};

class Emitter
{
	public:
		float posX, posY;

		Emitter();
		virtual ~Emitter() {};
		Emitter(float x, float y);
		virtual pair<float, float> getParticlePosition();
		virtual list<pair<float, float> > getParticlePosition(int numberOfParticles);
};

class SquareEmitter: Emitter
{
	public:
		float destX, destY;

		SquareEmitter(float originX, float originY, float cornerX, float cornerY);
		pair<float, float> getParticlePosition();
};


class CircleEmitter: Emitter
{
	public:
		float radius;

		CircleEmitter(float centerX, float centerY, float radius);
		pair<float, float> getParticlePosition();
};

#endif

