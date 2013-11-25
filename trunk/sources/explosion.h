#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "graphicEngine.h"
#include "level.h"

class Explosion: public AnimatedDrawable
{
	public:
		int type;

		Explosion();
		Explosion(int x, int y, int type);
		virtual void animate();
		virtual ~Explosion();
};

#define XWING_EXPL 0
#define TIE_EXPL 1
#define SPARK 2
#define FALCON_EXPL 3

#endif

