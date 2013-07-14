/*
 * Graphic Engine
 * Contains graphic-related functions (loading texture, drawing, etc.)
 */
 
#include "graphicEngine.h"

//Load a texture as an SDL_Surface
SDL_Surface * Ge::loadTexture(string path)
{
		SDL_Surface * aSurface;
		SDL_Surface *tmp = IMG_Load(path.c_str());
		if (tmp == NULL) {
			printf("Unable to load image: %s\n", SDL_GetError());
			return NULL;
		}

		aSurface = SDL_DisplayFormatAlpha(tmp);
		SDL_FreeSurface(tmp);

		return aSurface;
}

void Ge::drawFrame()
{
	for (std::vector<Drawable *>::iterator it = toDisplay.begin() ; it != toDisplay.end(); ++it)
	{
		draw(*it);
	}
	displayFrame();
}

//Draw an object on the screen
int Ge::draw(Drawable * sprite)
{
	SDL_Rect src, dest;
	 
	//Specifiy which part of the texture to display
	src.x = sprite->animX;
	src.y = sprite->animY;
	src.w = sprite->width;
	src.h = sprite->height;
	
	//Specify where to display on the screen 
	dest.x = sprite->posX;
	dest.y = sprite->posY;
	dest.w = sprite->width;
	dest.h = sprite->height;
	
	SDL_BlitSurface(sprite->texture, &src, screen, &dest);

    return(TRUE);
}

void Ge::displayFrame()
{
	SDL_Flip(screen);
}

void Ge::blitElement(SDL_Surface * anElement)
{
	SDL_BlitSurface(anElement, NULL, screen, NULL);
}


void Ge::addFont(string path)
{
	TTF_Font *font;
	font = TTF_OpenFont("res/Arial.ttf", 24);
	availableFonts.push_back(font);
}

void Ge::initColors()
{
	SDL_Color white = {255, 255, 255};
	SDL_Color red = {255, 0, 0};

	availableColors.insert(make_pair("WHITE", white));
	availableColors.insert(make_pair("RED", red));
}
