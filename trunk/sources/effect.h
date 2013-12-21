#ifndef EFFECTS_H
#define EFFECTS_H

#include "graphicEngine.h"
#include "level.h"
#include "text.h"

class Effect: public AnimatedDrawable
{
	public:
		int followsAnObject;
		int frameCount;
		map<string, AnimatedDrawable *> effectLayers;
		map<Drawable *, Drawable *> followedObjects;
		int numberOfLoops;
		int isMoving;
		float movingSpeed;

		Effect();
		Effect(Json::Value aConfig);
		Effect(int x, int y, string type);
		void loadConf(string aConf);
		virtual int animateEffect();
		virtual ~Effect();
};

class TextEffect: virtual public AnimatedText
{
	public:
		int frameCount;
		int isMoving;

		TextEffect(int x, int y, string type, string aText);
		virtual void animate();
};

#endif

