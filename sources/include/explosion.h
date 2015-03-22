#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "graphicEngine.h"
#include "level.h"

class Explosion: public AnimatedDrawable
{
	public:
		AnimatedDrawable smoke;

		Explosion();
		Explosion(int x, int y);
		void animate();
};

#endif

