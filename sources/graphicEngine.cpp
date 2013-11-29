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

	for (list<ParticleEffect *>::iterator anEffect = particleEffects.begin() ; anEffect != particleEffects.end(); ++anEffect)
	{
		if((*anEffect)->currentFrame == (*anEffect)->animationLength)
		{
			delete (*anEffect);
			particleEffects.remove(*anEffect++);
		}
		else
		{
			drawEffect(*anEffect);
		}

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
			glVertex3f((sprite->getPosX()/(SCREEN_WIDTH/(aspectRatio*2))), (SCREEN_HEIGHT - sprite->getPosY())/(SCREEN_HEIGHT/2), 0.);

			glTexCoord2f(sprite->ogl_Xcorner, sprite->ogl_Yorigin);
			glVertex3f((sprite->getPosX() + sprite->getWidth())/(SCREEN_WIDTH/(aspectRatio*2)), (SCREEN_HEIGHT - sprite->getPosY())/(SCREEN_HEIGHT/2), 0.);

			glTexCoord2f(sprite->ogl_Xcorner, sprite->ogl_Ycorner);
			glVertex3f((sprite->getPosX() + sprite->getWidth())/(SCREEN_WIDTH/(aspectRatio*2)), (SCREEN_HEIGHT - (sprite->getPosY() + sprite->getHeight()))/(SCREEN_HEIGHT/2), 0.);

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

int GraphicEngine::drawEffect(ParticleEffect * anEffect)
{
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);
	glColor4f(anEffect->colorR, anEffect->colorG, anEffect->colorB, anEffect->opacity);

	for (list<LineEffect *>::iterator aLine = anEffect->lineEffects.begin(); aLine != anEffect->lineEffects.end(); ++aLine)
	{
		//Set the opacity
		glLineWidth(2.5);
		//glColor4f(1.0, 1.0, 1.0, 1);

		//Create a temporary context in case we perform specific transformation for that object
		glPushMatrix();

		//Draw a Line

			glVertex3f(((*aLine)->destX/(SCREEN_WIDTH/(aspectRatio*2))), (SCREEN_HEIGHT - (*aLine)->destY)/(SCREEN_HEIGHT/2), 0);
			glVertex3f(((*aLine)->posX/(SCREEN_WIDTH/(aspectRatio*2))), (SCREEN_HEIGHT - (*aLine)->posY)/(SCREEN_HEIGHT/2), 0);


		//Discard the temporary context
		glPopMatrix();
	}
	glEnd();
	glEnable(GL_TEXTURE_2D);

	anEffect->currentFrame++;
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

