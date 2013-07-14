/*
 * physicEngine.cpp
 *
 *  Created on: 18 mai 2013
 *      Author: youenn
 */
#include "physicEngine.h"

//Check if a collision between two objects
int Pe::collisionDetection(Drawable *aDrawable, Drawable *anotherDrawable)
{
	if (boundingBox(aDrawable, anotherDrawable))
	{
		if(pixelPerfect(aDrawable, anotherDrawable))
			return 1;
	}
	return 0;
}

//Compute first if the two object boxes overlap
int Pe::boundingBox(Drawable *aDrawable, Drawable *anotherDrawable)
{
	if (aDrawable->posX + aDrawable->width < anotherDrawable->posX)
		{return 0;}
	if (aDrawable->posX > anotherDrawable->posX + aDrawable->width)
		{return 0;}
	if (aDrawable->posY + aDrawable->height < anotherDrawable->posY)
		{return 0;}
	if (aDrawable->posY > anotherDrawable->posY + anotherDrawable->height)
		{return 0;}

	return 1;
}

//Check if the two object have at least one overlapping pixel
int Pe::pixelPerfect(Drawable *aDrawable, Drawable *anotherDrawable)
{
	pair<int, int> i, j;
	int overX, overY, X, Y, pixA, pixB;

	overEdgesComputing(aDrawable, anotherDrawable);
	i =  getStartOffsetForOverlapRectangle(aDrawable);
	j =  getStartOffsetForOverlapRectangle(anotherDrawable);
	overX = overRight - overLeft;
	overY = overBottom - overTop;
	if (overY < 0)
		{overY = overY * -1;}

	/*	effectiveOverlapRect := self overlapRect: i	extent: overX @ overY.
	rect2 := self overlapRect: (j x + 30) @ j y	extent: overX @ overY.*/
	X = 0;
	Y = 0;
	while (X < (overX - 1))
	{
		while (Y < (overY - 1))
		{
			pixA = getPixel((i.first + X), (i.second + Y), aDrawable);
			pixB = getPixel((j.first + X), (j.second + Y), anotherDrawable);
			if ((pixA > 0) && (pixB > 0))
				return 1;
			Y = Y + 1;
		}
		X = X + 1;
		Y = 0;
	}
	return 0;
}

void Pe::overEdgesComputing(Drawable *aDrawable, Drawable *anotherDrawable)
{
	if(aDrawable->posY < anotherDrawable->posY)
		{overTop = aDrawable->posY;}
	else
		{overTop = aDrawable->posY;}

	if(aDrawable->posY + aDrawable->height > (anotherDrawable->posY + anotherDrawable->height))
		{overBottom = anotherDrawable->posY + anotherDrawable->height;}
	else
		{overBottom = aDrawable->posY + aDrawable->height;}
	if(aDrawable->posX < anotherDrawable->posX)
		{overLeft = anotherDrawable->posX;}
	else
		{overLeft = aDrawable->posX;}
	if(aDrawable->posX + aDrawable->width > (anotherDrawable->posX + anotherDrawable->width))
		{overRight = anotherDrawable->posX + anotherDrawable->width;}
	else
		{overRight = aDrawable ->posX + aDrawable->width;}
}

pair<int,int> Pe::getStartOffsetForOverlapRectangle(Drawable *aDrawable)
{
	int t2, t3;
	t2 = overLeft - aDrawable->posX;
	t3 = overTop - aDrawable->posY;
	return make_pair(t2,t3);
}

int Pe::getPixel(int x, int y, Drawable * aDrawable)
{
	//Get position the coordinate of the pixel of the whole texture
	int pixelPos = aDrawable->animX + x + ((aDrawable->animY + y) * aDrawable->texture->w);

	return ((unsigned int*)aDrawable->texture->pixels)[pixelPos];
	//(((t2 currentClipRect memberAt: #x)  + t1 x) @ ((t2 currentClipRect memberAt: #y) + t1 y))
}

//make sure that the hero stays within screen boundary (from (0,0) to aPoint coordinate)
void Pe::stayOnScreen(Hero * aHero, pair<int, int> aPoint)
{
	aHero->leftFlag = 1;
	aHero->rightFlag = 1;
	aHero->topFlag = 1;
	aHero->bottomFlag = 1;

	if(aHero->posX <= 0)
		{aHero->leftFlag = 0;}
	if(aHero->posY <= 0)
		{aHero->topFlag = 0;}
	if(aHero->posX + aHero->width >= aPoint.first)
		{aHero->rightFlag = 0;}
	if(aHero->posY + aHero->height >= aPoint.second)
		{aHero->bottomFlag = 0;}
}

//make sure that an item is on screen
int Pe::isOnScreen(Drawable *aDrawable)
{
	if (aDrawable->posX < 0 ||
		aDrawable->posY < 0 ||
		aDrawable->posX > SCREEN_WIDTH - aDrawable->width ||
		aDrawable->posY > GAMEZONE_HEIGHT)
	{
		return FALSE;
	}
	return TRUE;
}
