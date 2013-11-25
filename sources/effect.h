#ifndef EFFECTS_H
#define EFFECTS_H


#include "graphicEngine.h"
#include "level.h"

class Effect: public AnimatedDrawable
{
	public:
		string type;
		int followsAnObject;
		int frameCount;
		map<string, AnimatedDrawable *> effectLayers;
		map<Drawable *, Drawable *> followedObjects;
		int numberOfLoops;

		Effect();
		Effect(int x, int y, string type);
		void loadConf(string aConf);
		void startEffect(int x, int y);
		virtual int animateEffect();
		virtual ~Effect();
};

#endif

