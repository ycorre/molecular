/*
 * Graphic Engine
 * Contains graphic-related functions (loading texture, drawing, etc.)
 */
 
#include "graphicEngine.h"

GraphicEngine::GraphicEngine()
{
	alphaFading = 0;
}

void GraphicEngine::initGe()
{
	blackBox = new Drawable();
	blackBox->width = 1200;
	blackBox->height = 600;
	blackBox->animX = 0;
	blackBox->animY = 0;
	blackBox->texture = loadTexture("res/bbox.png");
	blackBox->texture = SDL_DisplayFormat(blackBox->texture);
}

//Load a texture as an SDL_Surface
SDL_Surface * GraphicEngine::loadTexture(string path)
{
		SDL_Surface * aSurface;
		SDL_Surface * tmp = IMG_Load(path.c_str());
		if (tmp == NULL) {
			printf("Unable to load image: %s\n", SDL_GetError());
			return NULL;
		}

		aSurface = SDL_DisplayFormatAlpha(tmp);
		SDL_FreeSurface(tmp);

		return aSurface;
}

void GraphicEngine::drawFrame()
{
	if(!toDisplay.empty())
	{
		for (std::vector<Drawable *>::iterator it = toDisplay.begin() ; it != toDisplay.end(); ++it)
		{
			draw(*it);
		}

		displayFrame();
	}
	else
	{
		cout<<"Warning: GraphicEngine (drawFrame): The set of elements to display is empty\n";
	}
}

//Draw an object on the screen
int GraphicEngine::draw(Drawable * sprite)
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

    return 1;
}

void GraphicEngine::displayFrame()
{
	SDL_Flip(screen);
}

void GraphicEngine::blitElement(SDL_Surface * anElement)
{
	SDL_BlitSurface(anElement, NULL, screen, NULL);
}


void GraphicEngine::addFont(string path)
{
	TTF_Font * font;
	font = TTF_OpenFont(path.c_str(), 24);
	if (font == NULL)
    {
        cerr << "TTF_OpenFont() Failed: " << TTF_GetError() << endl;
        TTF_Quit();
        SDL_Quit();
    	exit(1);
    }
	availableFonts.push_back(font);
}

void GraphicEngine::initColors()
{
	SDL_Color white = {255, 255, 255};
	SDL_Color red = {255, 0, 0};

	availableColors.insert(make_pair("WHITE", white));
	availableColors.insert(make_pair("RED", red));
}

//Perform a fade out effect
void GraphicEngine::fadeOut()
{
	if(!isFading)
	{
		isFading = TRUE;
		alphaFading = 0;
	}

	alphaFading = min(255, alphaFading + 2);
	SDL_SetAlpha(blackBox->texture, SDL_SRCALPHA, alphaFading);
	toDisplay.push_back(blackBox);

	if(alphaFading == 255)
	{
		isFading = FALSE;
	}
}

void GraphicEngine::fadeOut(int fadeOutSpeed)
{
	if(!isFading)
	{
		isFading = TRUE;
		alphaFading = 0;
	}

	alphaFading = min(255, alphaFading + fadeOutSpeed);
	SDL_SetAlpha(blackBox->texture, SDL_SRCALPHA, alphaFading);
	toDisplay.push_back(blackBox);

	if(alphaFading == 255)
	{
		isFading = FALSE;
	}
}

void GraphicEngine::fadeIn()
{
	if(!isFading)
	{
		isFading = TRUE;
		alphaFading = 255;
	}

	alphaFading = max(0, alphaFading - 1);
	SDL_SetAlpha(blackBox->texture, SDL_SRCALPHA, alphaFading);
	toDisplay.push_back(blackBox);

	if(alphaFading == 0)
	{
		isFading = FALSE;
	}
}
