#ifndef EFFECTS_H
#define EFFECTS_H


#include "graphicEngine.h"
#include "level.h"

class Effect: public Drawable
{
	public:
		map<string, Drawable*> loadedEffects;
		string type;

		Effect();
		Effect(int x, int y, string type);
		virtual void animate();
		virtual ~Effect();
};

#define XWING_EXPL 0
#define TIE_EXPL 1
#define SPARK 2
#define FALCON_EXPL 3

#endif

