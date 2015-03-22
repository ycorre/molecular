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
		float xImpactPos;
		float yImpactPos;

		bool collisionDetection(Drawable *aDrawable, Drawable *anotherDrawable);
		bool boundingBox(Drawable *aDrawable, Drawable* anotherDrawable);
		bool pixelPerfect(Drawable *aDrawable, Drawable *anotherDrawable);
		void overEdgesComputing(Drawable *aDrawable, Drawable *anotherDrawable);
		pair<int,int> getStartOffsetForOverlapRectangle(Drawable *aDrawable);
		Uint32 getPixel(int x, int y, Drawable * aDrawable);
		void stayOnScreen(Hero * aHero, pair<int, int> aPoint);
		bool isOnScreen(Drawable *aDrawable);
};

Uint32 getpixel(SDL_Surface *surface, int x, int y);

#endif /* PHYSCENGINE_H_ */
