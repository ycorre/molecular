#include"explosion.h"

Explosion::Explosion()
{
	this->loadTexture("res/expl2.png");
	width = texture->w;
	height = texture->h;
	posX = 0;
	posY = 0;
	state = 0;
	setAnimX(0);
	setAnimY(0);
	type = XWING_EXPL;
}


Explosion::Explosion(int x, int y, int typeExpl)
{
	Drawable();
	this->addTexture("explosion");
	width = atoi(((lev->configurationElements.at("explosion")).at(0)).c_str());
	height =  atoi(((lev->configurationElements.at("explosion")).at(1)).c_str());
	posX = x;
	posY = y;
	type = typeExpl;
	state = type;
	setAnimX(0);
	setAnimY(state * height);
	parseAnimationState((lev->configurationElements.at("explosion")).at(2));
}

Explosion::~Explosion()
{

}

void Explosion::animate()
{
	updateAnimationFrame();
	if (getAnimX() == 0)
	{
		display = FALSE;
		toRemove = TRUE;
		if(type == TIE_EXPL)
		{
			lev->heroLoseLife();
		}
	}
	posX = posX - lev->cameraSpeed;
}
