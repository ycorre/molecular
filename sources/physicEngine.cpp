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
	if (aDrawable->posX > anotherDrawable->posX + anotherDrawable->width)
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

/*	cout<< "Falcin width "<< aDrawable->width << " h " << aDrawable->height << "\n";
	cout<< "Falcin2 width "<< aDrawable->posX << " h " << aDrawable->posY << "\n";
	cout<< "Falcon width "<< anotherDrawable->width << " h " << anotherDrawable->height << "\n";
	cout<< "Falcon2 width "<< anotherDrawable->posX << " h " << anotherDrawable->posY << "\n";*/
	pair<int, int> i, j;
	int overX, overY, x, y;
	Uint32 pixA, pixB;

	overEdgesComputing(aDrawable, anotherDrawable);
	i = getStartOffsetForOverlapRectangle(aDrawable);
	j = getStartOffsetForOverlapRectangle(anotherDrawable);

	if (i.second < 0 || j.second <0)
		cout<<"!!!!!!!!!! i: "<<i.second<<",  j: "<<j.second<<" \n";

	overX = overRight - overLeft;
	overY = overBottom - overTop;
	if (overY < 0)
		{overY = overY * -1;}

	/*	effectiveOverlapRect := self overlapRect: i	extent: overX @ overY.
	rect2 := self overlapRect: (j x + 30) @ j y	extent: overX @ overY.*/
	x = 0;
	y = 0;

	SDL_LockSurface(aDrawable->texture);
	SDL_LockSurface(anotherDrawable->texture);
	while (x < (overX - 1))
	{
		while (y < (overY - 1))
		{
			pixA = getPixel((i.first + x), (i.second + y), aDrawable);
			pixB = getPixel((j.first + x), (j.second + y), anotherDrawable);
			if ((pixA > 0) && (pixB > 0 && pixB != 16777215))
			{
				SDL_UnlockSurface(aDrawable->texture);
				SDL_UnlockSurface(anotherDrawable->texture);
				return 1;
			}
			y = y + 1;
		}
		x = x + 1;
		y = 0;
	}
	SDL_UnlockSurface(aDrawable->texture);
	SDL_UnlockSurface(anotherDrawable->texture);
	return 0;
}

void Pe::overEdgesComputing(Drawable *aDrawable, Drawable *anotherDrawable)
{
	if(aDrawable->posY < anotherDrawable->posY)
		{overTop = anotherDrawable->posY;}
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

Uint32 Pe::getPixel(int x, int y, Drawable * aDrawable)
{
	if (x < 0 || y < 0)
		cout<<"Warning: x: " <<x<<",  y: "<<y<<" \n";
	//Get position the coordinate of the pixel of the whole texture
//	int pixelPos = aDrawable->animX + x + ((aDrawable->animY + y) * aDrawable->texture->w);
	//int myX = aDrawable->animX + x;
	//int myY = aDrawable->animY + y;
//	return ((unsigned int*)aDrawable->texture->pixels)[pixelPos];
	return getpixel(aDrawable->texture, x, y);// myX, myY);
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
	if (aDrawable->posX + aDrawable->width < 0 ||
		aDrawable->posY + aDrawable->height < 0 ||
		aDrawable->posX > SCREEN_WIDTH - aDrawable->width ||
		aDrawable->posY > GAMEZONE_HEIGHT)
	{
		return FALSE;
	}
	return TRUE;
}

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    // Here p is the address to the pixel we want to retrieve
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp)
    {
		case 1:
			return *p;
			break;

		case 2:
			return *(Uint16 *)p;
			break;

		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[0] | p[1] << 8 | p[2] << 16;
			break;

		case 4:
			return *(Uint32 *)p;
			break;

		default:
			cout<<"Error while getting pixel value";
			return 0;       // shouldn't happen, but avoids warnings
    }
}
