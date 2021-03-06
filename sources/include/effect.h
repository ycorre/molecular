#ifndef EFFECTS_H
#define EFFECTS_H

#include "graphicEngine.h"
#include "level.h"
#include "text.h"

class Effect: public AnimatedDrawable
{
	public:
		bool followsAnObject;
		int frameCount;
		map<string, AnimatedDrawable *> effectLayers;
		int numberOfLoops;
		bool isMoving;
		float movingSpeed;
		vector<string> particleEffects;

		Effect();
		Effect(Json::Value aConfig);
		Effect(int x, int y, string type);
		void loadConf(string aConf);
		int animateEffect();
		~Effect();
};

class TextEffect: virtual public AnimatedText
{
	public:
		int frameCount;
		bool isMoving;

		TextEffect(int x, int y, string aText);
		void animate();
};

#endif

