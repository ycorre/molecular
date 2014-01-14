/*
 * physicEngine.cpp
 *
 *  Created on: 18 mai 2013
 *      Author: youenn
 */
#include "physicEngine.h"

//Check if a collision between two objects
bool Pe::collisionDetection(Drawable *aDrawable, Drawable *anotherDrawable)
{
	if (boundingBox(aDrawable, anotherDrawable) && isOnScreen(aDrawable) && isOnScreen(anotherDrawable))
	{
		if(pixelPerfect(aDrawable, anotherDrawable))
			return true;
	}
	return false;
}

//Compute first if the two object boxes overlap
bool Pe::boundingBox(Drawable *aDrawable, Drawable *anotherDrawable)
{
	if (aDrawable->getXBoundary() + aDrawable->getWidthBoundary() < anotherDrawable->getXBoundary())
		{return false;}
	if (aDrawable->getXBoundary() > anotherDrawable->getXBoundary() + anotherDrawable->getWidthBoundary())
		{return false;}
	if (aDrawable->getYBoundary() + aDrawable->getHeightBoundary() < anotherDrawable->getYBoundary())
		{return false;}
	if (aDrawable->getYBoundary() > anotherDrawable->getYBoundary() + anotherDrawable->getHeightBoundary())
		{return false;}

	return true;
}

//Check if the two objects have at least one overlapping pixel
bool Pe::pixelPerfect(Drawable *aDrawable, Drawable *anotherDrawable)
{
	pair<int, int> startingCoordinatesI, startingCoordinatesJ;
	int overX, overY, x, y;
	Uint32 pixA, pixB;

	overEdgesComputing(aDrawable, anotherDrawable);
	startingCoordinatesI = getStartOffsetForOverlapRectangle(aDrawable);
	startingCoordinatesJ = getStartOffsetForOverlapRectangle(anotherDrawable);

	if (startingCoordinatesI.second < 0 || startingCoordinatesJ.second < 0)
		cout<< "Error (pixelPerfect): second i: " << startingCoordinatesI.second << ",  j: " << startingCoordinatesJ.second << endl;

	if (startingCoordinatesI.first < 0 || startingCoordinatesJ.first < 0)
		cout<< "Error (pixelPerfect): first i: " << startingCoordinatesI.first << ",  j: " << startingCoordinatesJ.first << endl;

	overX = overRight - overLeft;
	overY = overBottom - overTop;
	if (overY < 0)
	{
		overY = overY * -1;
		cout<< "Error (pixelPerfect): overY is negative " << endl;
	}

	x = 0;
	y = 0;

	SDL_LockSurface(aDrawable->getTexture());
	SDL_LockSurface(anotherDrawable->getTexture());
	while (x < (overX - 1))
	{
		while (y < (overY - 1))
		{
			pixA = getPixel((startingCoordinatesI.first + x), (startingCoordinatesI.second + y), aDrawable);
			pixB = getPixel((startingCoordinatesJ.first + x), (startingCoordinatesJ.second + y), anotherDrawable);

			if ((pixA > 0) && (pixB > 0))
			{
				SDL_UnlockSurface(aDrawable->getTexture());
				SDL_UnlockSurface(anotherDrawable->getTexture());
				xImpactPos = startingCoordinatesI.first + x;
				yImpactPos = startingCoordinatesI.second + y;
				return true;
			}
			y = y + 1;
		}
		x = x + 1;
		y = 0;
	}
	SDL_UnlockSurface(aDrawable->getTexture());
	SDL_UnlockSurface(anotherDrawable->getTexture());
	return false;
}

void Pe::overEdgesComputing(Drawable *aDrawable, Drawable *anotherDrawable)
{
	if(aDrawable->getYBoundary() < anotherDrawable->getYBoundary())
		{overTop = anotherDrawable->getYBoundary();}
	else
		{overTop = aDrawable->getYBoundary();}

	if(aDrawable->getYBoundary() + aDrawable->getHeightBoundary() > (anotherDrawable->getYBoundary() + anotherDrawable->getHeightBoundary()))
		{overBottom = anotherDrawable->getYBoundary() + anotherDrawable->getHeightBoundary();}
	else
		{overBottom = aDrawable->getYBoundary() + aDrawable->getHeightBoundary();}

	if(aDrawable->getXBoundary() < anotherDrawable->getXBoundary())
		{overLeft = anotherDrawable->getXBoundary();}
	else
		{overLeft = aDrawable->getXBoundary();}

	if(aDrawable->getXBoundary() + aDrawable->getWidthBoundary() > (anotherDrawable->getXBoundary() + anotherDrawable->getWidthBoundary()))
		{overRight = anotherDrawable->getXBoundary() + anotherDrawable->getWidthBoundary();}
	else
		{overRight = aDrawable ->getXBoundary() + aDrawable->getWidthBoundary();}
}

pair<int,int> Pe::getStartOffsetForOverlapRectangle(Drawable *aDrawable)
{
	int startingX, startingY;
	startingX = overLeft - aDrawable->getXBoundary();
	startingY = overTop - aDrawable->getYBoundary();

	return make_pair(startingX, startingY);
}

Uint32 Pe::getPixel(int x, int y, Drawable * aDrawable)
{
	if (x < 0 || y < 0)
		cout<<"Warning: x: " << x <<",  y: " << y << endl;

/*	if(aDrawable->hasHitBox())
	{
		return 1;
	}*/

	int textureX, textureY;
	textureX = ((int) aDrawable->getAnimX() + x) % aDrawable->getCollisionTexture()->w;
	textureY = ((int) aDrawable->getAnimY() + y) % aDrawable->getCollisionTexture()->h;

	return getpixel(aDrawable->getCollisionTexture(), textureX, textureY);
}

//make sure that the hero stays within screen boundary (from (0,0) to aPoint coordinate)
void Pe::stayOnScreen(Hero * aHero, pair<int, int> aPoint)
{
	aHero->leftFlag = true;
	aHero->rightFlag = true;
	aHero->topFlag = true;
	aHero->bottomFlag = true;

	if(aHero->posX <= 0)
		{aHero->leftFlag = false;}
	if(aHero->posY <= 0)
		{aHero->topFlag = false;}
	if(aHero->posX + aHero->width >= aPoint.first)
		{aHero->rightFlag = false;}
	if(aHero->posY + aHero->height >= aPoint.second)
		{aHero->bottomFlag = false;}
}

//make sure that an item is on screen
bool Pe::isOnScreen(Drawable *aDrawable)
{
/*	if (aDrawable->posX + aDrawable->width < 0 ||
		aDrawable->posY + aDrawable->height < 0 ||
		aDrawable->posX > SCREEN_WIDTH - aDrawable->width ||
		aDrawable->posY > GAMEZONE_HEIGHT)*/
	if (aDrawable->posX + aDrawable->width < 0 ||
		aDrawable->posY + aDrawable->height < 0 ||
		aDrawable->posX > SCREEN_WIDTH ||
		aDrawable->posY > GAMEZONE_HEIGHT)
	{
		return false;
	}
	return true;
}

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = 4;// surface->format->BytesPerPixel;
    //Here p is the address to the pixel we want to retrieve
    Uint8 *p = (Uint8 *) surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp)
    {
		case 1:
			return *p;
			break;

		case 2:
			return *(Uint16 *)p;
			break;

		case 4:
			//if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			//else
			//	return p[0] | p[1] << 8 | p[2] << 16;
			break;

		case 3:
			return *(Uint32 *)p;
			break;

		default:
			cout << "Warning (getPixel): Error while getting pixel value" << endl;
			return 0;
    }
}
