/*
 * physcEngine.h
 *
 *  Created on: 18 mai 2013
 *      Author: youenn
 */

#ifndef PHYSCENGINE_H_
#define PHYSCENGINE_H_

#include <iostream>
#include <string>
#include <utility>
#include "common.h"
#include "graphicEngine.h"
#include "hero.h"

class Hero;

class Pe
{
	public:
		int overTop;
		int overLeft;
		int overRight;
		int overBottom;

		int collisionDetection(Drawable *aDrawable, Drawable *anotherDrawable);
		int pixelPerfect(Drawable *aDrawable, Drawable *anotherDrawable);
		int boundingBox(Drawable *aDrawable, Drawable* anotherDrawable);
		void overEdgesComputing(Drawable *aDrawable, Drawable *anotherDrawable);
		pair<int,int> getStartOffsetForOverlapRectangle(Drawable *aDrawable);
		int getPixel(int x, int y, Drawable * aDrawable);
		void stayOnScreen(Hero * aHero, pair<int, int> aPoint);
		int isOnScreen(Drawable *aDrawable);
};

#endif /* PHYSCENGINE_H_ */
