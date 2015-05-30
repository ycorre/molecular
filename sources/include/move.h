#ifndef MOVE_H
#define MOVE_H

#include "drawable.h"

class Move
{
	public:
		Drawable * aDrawable;
		int moveIndex;
		list<Json::Value> moveConfigs;

		Move(Json::Value aConfig);
		void nextMove();
};

#endif
