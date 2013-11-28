/*
 * Graphic Engine
 * Contains graphic-related functions (loading texture, drawing, etc.)
 */
 
#include "graphicEngine.h"

GraphicEngine::GraphicEngine()
{
	alphaFading = 0;
	aspectRatio = 1.0;
}

void GraphicEngine::initGe()
{
#if !USE_OPENGL
	blackBox = new Drawable();
	blackBox->width = 1200;
	blackBox->height = 600;
	blackBox->loadTexture("res/bbox.png");
	blackBox->texture = SDL_DisplayFormat(blackBox->texture);
	blackBox->setAnimX(0);
	blackBox->setAnimY(0);
#endif
}

//Load a texture as an SDL_Surface
SDL_Surface * GraphicEngine::loadTexture(string path)
{
		SDL_Surface * aSurface;
		SDL_Surface * tmp = IMG_Load(path.c_str());
		if (tmp == NULL) {
			cerr << "Unable to load image: " << SDL_GetError() << endl;
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
#if USE_OPENGL
	//Reset the view matrix
    glLoadIdentity();
    //Perform a translation so that we are starting our coordinates at point (0,0)
	glTranslatef(-aspectRatio, -1 , -1);
#endif

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

		if(sprite->toBlend)
		{
			glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
		}

		//Set the opacity
		glColor4f(1.0, 1.0, 1.0, sprite->opacity);

		//Load a new context in case we perform specific transformation for that object
		glPushMatrix();

		//If some scaling operations are required
		if (sprite->scaleX != 1 || sprite->scaleY != 1)
		{
			float zX =  sprite->getPosX()/(SCREEN_WIDTH/(aspectRatio*2));
			float zY =  (SCREEN_HEIGHT- sprite->getPosY())/(SCREEN_HEIGHT/2);

			//Translate back to 0,0 in order to perform the scaling
			glTranslatef(zX, zY, 0.0);
			//Scale
			glScalef(sprite->scaleX, sprite->scaleY, 1.0f);
			//Translate back to the normal position
			glTranslatef(-zX, -zY, 0.0);
		}

		//If some rotation operations are required
		if (sprite->rotationAngle != 0)
		{
			float zX =  sprite->getPosX()/(SCREEN_WIDTH/(aspectRatio*2));
			float zY =  (SCREEN_HEIGHT- sprite->getPosY())/(SCREEN_HEIGHT/2);

			//Translate back to 0,0 in order to perform the scaling
			glTranslatef(zX, zY, 0.0);
			//Rotate
			glRotatef(sprite->rotationAngle, sprite->rotX, sprite->rotY, sprite->rotZ);
			//Translate back to the normal position
			glTranslatef(-zX, -zY, 0.0);
		}

		//Use the object texture
		glBindTexture(GL_TEXTURE_2D, sprite->getOpenGLTexture());

		//Draw a quadrilateral
		glBegin(GL_QUADS);
			//Texture coordinates to display
			glTexCoord2f(sprite->ogl_Xorigin, sprite->ogl_Yorigin);
			//Coordinates of the quadrilateral
			glVertex3f((sprite->getPosX()/(SCREEN_WIDTH/(aspectRatio*2))), (SCREEN_HEIGHT- sprite->getPosY())/(SCREEN_HEIGHT/2), 0.);

			glTexCoord2f(sprite->ogl_Xcorner, sprite->ogl_Yorigin);
			glVertex3f((sprite->getPosX() + sprite->getWidth())/(SCREEN_WIDTH/(aspectRatio*2)), (SCREEN_HEIGHT-sprite->getPosY())/(SCREEN_HEIGHT/2), 0.);

			glTexCoord2f(sprite->ogl_Xcorner, sprite->ogl_Ycorner);
			glVertex3f((sprite->getPosX() + sprite->getWidth())/(SCREEN_WIDTH/(aspectRatio*2)), (SCREEN_HEIGHT-(sprite->getPosY() + sprite->getHeight()))/(SCREEN_HEIGHT/2), 0.);

			glTexCoord2f(sprite->ogl_Xorigin, sprite->ogl_Ycorner);
			glVertex3f((sprite->getPosX()/(SCREEN_WIDTH/(aspectRatio*2))), (SCREEN_HEIGHT - (sprite->getPosY() + sprite->getHeight()))/(SCREEN_HEIGHT/2), 0.);

		glEnd();

		//Discard the new context
		glPopMatrix();

		if(sprite->toBlend)
		{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
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

		SDL_BlitSurface(sprite->getTexture(), &src, screen, &dest);
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


void GraphicEngine::addFont(string aName, string path)
{
	TTF_Font * font;
	font = TTF_OpenFont(path.c_str(), 18);
	if (font == NULL)
    {
        cerr << "TTF_OpenFont() Failed: " << TTF_GetError() << endl;
        TTF_Quit();
        SDL_Quit();
    	exit(1);
    }
	availableFonts.insert(make_pair(aName, font));
}

void GraphicEngine::initColors()
{
	SDL_Color white = {255, 255, 255};
	SDL_Color red = {255, 0, 0};
	SDL_Color blue = {29, 210, 199};

	availableColors.insert(make_pair("WHITE", white));
	availableColors.insert(make_pair("RED", red));
	availableColors.insert(make_pair("BLUE", blue));
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
	glDisable(GL_TEXTURE_2D);
	glColor4f(0.0f, 0.0f, 0.0f, alphaGL);

	glBegin(GL_QUADS);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(SCREEN_WIDTH, 0.0f, 0.0f);
		glVertex3f(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		glVertex3f(0.0f, SCREEN_HEIGHT, 0.0f);
	glEnd();

	glColor4f(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
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
	glDisable(GL_TEXTURE_2D);
	glColor4f(1., 1., 1., alphaGL);

	glBegin(GL_QUADS);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(SCREEN_WIDTH, 0.0f);
		glVertex2f(SCREEN_WIDTH, SCREEN_HEIGHT);
		glVertex2f(0.0f, SCREEN_HEIGHT);
	glEnd();

	glEnable(GL_TEXTURE_2D);
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
	glTexImage2D(GL_TEXTURE_2D, 0, 4 , aSurface->w, aSurface->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, aSurface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


//Merge several drawables which have a texture of 4 bits per pixel
//Following the formula destPixel.component = 255 - ((productOf(255-drawables.i.component))/powerOf(255, sizeOfdrawables)
//(cf. the "Screen" equation at this page : http://docs.gimp.org/en/gimp-concepts-layer-modes.html)
//To be removed as it is not performant enough
void GraphicEngine::mergeImages(vector <Drawable*> drawables, Drawable * destination)
{
	int x, y, i;
	vector<Uint8 *> pixelPointers;
	Uint32 pixelProd[4];
	Uint32 divisionValue = pow(255, (drawables.size()-1));

	for(std::vector<Drawable *>::iterator aDrawable = drawables.begin() ; aDrawable != drawables.end(); ++aDrawable)
	{
		SDL_LockSurface((*aDrawable)->getTexture());
	}

	SDL_LockSurface(destination->getTexture());
	Uint8 *pDest = (Uint8 *) destination->getTexture()->pixels;

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
		SDL_UnlockSurface((*aDrawable)->getTexture());
	}

	SDL_UnlockSurface(destination->getTexture());
}