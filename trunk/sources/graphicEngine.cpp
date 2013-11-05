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
	blackBox->loadTexture("res/bbox.png");
	blackBox->texture = SDL_DisplayFormat(blackBox->texture);
	blackBox->setAnimX(0);
	blackBox->setAnimY(0);
}

//Load a texture as an SDL_Surface
SDL_Surface * GraphicEngine::loadTexture(string path)
{
		SDL_Surface * aSurface;
		SDL_Surface * tmp = IMG_Load(path.c_str());
		if (tmp == NULL) {
			cout << "Unable to load image: " << SDL_GetError() << endl;
			return NULL;
		}

		aSurface = SDL_DisplayFormatAlpha(tmp);
		SDL_FreeSurface(tmp);

#if USE_OPENGL
		GLuint oglTex;
		createOGLTexture(aSurface, &oglTex);

		openGLTextures.insert(make_pair(aSurface, oglTex));
#endif

		return aSurface;
}

void GraphicEngine::drawFrame()
{
	if(!toDisplay.empty())
	{
		for (std::vector<Drawable *>::iterator aDrawable = toDisplay.begin() ; aDrawable != toDisplay.end(); ++aDrawable)
		{
			draw(*aDrawable);
		}
	}
	else
	{
		cout<<"Warning: GraphicEngine (drawFrame): The set of elements to display is empty\n";
	}

	//Check for effects to perform
	if(isFading)
	{
		fadeOut();
	}

	displayFrame();
}

//Draw an object on the screen
int GraphicEngine::draw(Drawable * sprite)
{

#if USE_OPENGL
	if (!sprite->isComposite())
	{
		glBindTexture(GL_TEXTURE_2D, sprite->oglTexture);
		glBegin(GL_QUADS);
			glTexCoord2f(sprite->ogl_Xorigin, sprite->ogl_Yorigin);
			glVertex2i(sprite->posX, sprite->posY);
			//glVertex3i(sprite->posX, sprite->posY, -1);

			glTexCoord2f(sprite->ogl_Xcorner, sprite->ogl_Yorigin);
			glVertex2i(sprite->posX + sprite->width, sprite->posY);
			//glVertex3i(sprite->posX + sprite->width, sprite->posY, -1);

			glTexCoord2f(sprite->ogl_Xcorner, sprite->ogl_Ycorner);
			glVertex2i(sprite->posX + sprite->width, sprite->posY + sprite->height);
			//glVertex3i(sprite->posX + sprite->width, sprite->posY + sprite->height, -1);

			glTexCoord2f(sprite->ogl_Xorigin, sprite->ogl_Ycorner);
			glVertex2i(sprite->posX, sprite->posY + sprite->height);
			//glVertex3i(sprite->posX, sprite->posY + sprite->height, -1);

		glEnd();
	}
	else
	{
		CompositeDrawable * aCDraw = static_cast<CompositeDrawable*> (sprite);
		//Set the blending parameters
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);

		//Draw the textures
		for (std::vector<Drawable *>::iterator aDrawable = aCDraw->toMerge.begin() ; aDrawable != aCDraw->toMerge.end(); ++aDrawable)
		{
			draw(*aDrawable);
		}

		//Restore the default blending parameters
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
#else
		SDL_Rect src, dest;

		//Specify which part of the texture to display

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
#endif

    return 1;
}

void GraphicEngine::displayFrame()
{
#if USE_OPENGL
	SDL_GL_SwapBuffers();
#else
	SDL_Flip(screen);
#endif
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

void GraphicEngine::startFadingOut(int aFadingSpeed)
{
	fadingSpeed = aFadingSpeed;
	isFading = TRUE;
	alphaFading = 0;
}

//Perform a fade out effect
void GraphicEngine::fadeOut()
{
	alphaFading = min(255, alphaFading + fadingSpeed);

#if USE_OPENGL
	float alphaGL = (float) alphaFading / 255.0;

	glColor4f(0., 0., 0., alphaGL);
	glBegin(GL_QUADS);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(SCREEN_WIDTH, 0.0f);
		glVertex2f(SCREEN_WIDTH, SCREEN_HEIGHT);
		glVertex2f(0.0f, SCREEN_HEIGHT);
	glEnd();

	glColor4f(1.0, 1.0, 1.0, 1.0);
#else
	SDL_SetAlpha(blackBox->texture, SDL_SRCALPHA, alphaFading);
	toDisplay.push_back(blackBox);
#endif

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

#if USE_OPENGL
	float alphaGL = (float) alphaFading / 255.0;

	glColor4f(1., 1., 1., alphaGL);
	glBegin(GL_QUADS);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(SCREEN_WIDTH, 0.0f);
		glVertex2f(SCREEN_WIDTH, SCREEN_HEIGHT);
		glVertex2f(0.0f, SCREEN_HEIGHT);
	glEnd();

	glColor4f(1.0, 1.0, 1.0, 1.0);
#else
	SDL_SetAlpha(blackBox->texture, SDL_SRCALPHA, alphaFading);
	toDisplay.push_back(blackBox);
#endif

	if(alphaFading <= 0)
	{
		isFading = FALSE;
	}
}

void GraphicEngine::createOGLTexture(SDL_Surface * aSurface, GLuint * oglTex)
{
	glGenTextures(1, oglTex);
	glBindTexture(GL_TEXTURE_2D, *oglTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, aSurface->w, aSurface->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, aSurface->pixels);
}


//Merge several drawables which have a texture of 4 bits per pixel
//Following the formula destPixel.component = 255 - ((productOf(255-drawables.i.component))/powerOf(255, sizeOfdrawables)
//(cf. the "Screen" equation at this page : http://docs.gimp.org/en/gimp-concepts-layer-modes.html)
void GraphicEngine::mergeImages(vector <Drawable*> drawables, Drawable * destination)
{
	int x, y, i;
	vector<Uint8 *> pixelPointers;
	Uint32 pixelProd[4];
	Uint32 divisionValue = pow(255, (drawables.size()-1));

	for(std::vector<Drawable *>::iterator aDrawable = drawables.begin() ; aDrawable != drawables.end(); ++aDrawable)
	{
		SDL_LockSurface((*aDrawable)->texture);
	}

	SDL_LockSurface(destination->texture);
	Uint8 *pDest = (Uint8 *) destination->texture->pixels;

	for(y=0; y<destination->height; y++)
	{
		for(x=0; x<destination->width; x++)
		{
			for(i=0; i<4; i++)
			{
				pixelProd[i] = 1 ;
			}

			for(std::vector<Drawable *>::iterator aDrawable = drawables.begin() ; aDrawable != drawables.end(); ++aDrawable)
			{
		/*		Uint8 * aPixelPointer =(Uint8 *)  (*aDrawable)->texture->pixels + (y * (*aDrawable)->texture->pitch) + (((*aDrawable)->animX + x) * 4);
				pixelProd[0] *= (255 - aPixelPointer[0]); //min(255, pixelSum[0] + aPixelPointer[0]);
				pixelProd[1] *= (255 - aPixelPointer[1]);
				pixelProd[2] *= (255 - aPixelPointer[2]);*/

				//Alpha layer values (ignored since we worked with drawables with no alpha channel)
			//	pixelSum[3] *= (255 - aPixelPointer[3]);
			}

			pDest[0] = 255 - (pixelProd[0]/divisionValue);
			pDest[1] = 255 - (pixelProd[1]/divisionValue);
			pDest[2] = 255 - (pixelProd[2]/divisionValue);
			pDest[3] = 0;// - (pixelSum[3]/255);

			//Advance to the next pixel position to draw
			pDest = pDest + 4;
		}
	}

	//Unlock SDL surfaces
	for(std::vector<Drawable *>::iterator aDrawable = drawables.begin() ; aDrawable != drawables.end(); ++aDrawable)
	{
		SDL_UnlockSurface((*aDrawable)->texture);
	}

	SDL_UnlockSurface(destination->texture);
}
