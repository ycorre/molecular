#include "include/move.h"

Move::Move(Json::Value aConfig)
{
	//moveConfigs = aConfig;
	moveIndex = 0;
	aDrawable = NULL;
}


void Move::nextMove()
{
	switch((moveConfigs.front()[0].asInt()))
	{
		case LINEAR_MOVE:
			break;

		case INERTIAL_MOVE:
			//computeInertialLinearMove(aDrawable->posX, aDrawable->posX, xDestination, yDestination, aDrawable->speed);
			break;

		case CURVY_MOVE:
			computeCircularMove(aDrawable->posX, aDrawable->posY, moveConfigs.front().get("radius", 20).asFloat(), moveConfigs.front()[0].get("startAngle", 20).asFloat(),  moveConfigs.front()[0].get("endAngle", 20).asFloat(), aDrawable->speed);
			break;

		default:
			break;
	}
}
