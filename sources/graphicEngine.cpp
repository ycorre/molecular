/*
 * Graphic Engine
 * Contains graphic-related functions (loading texture, drawing, etc.)
 */
 
#include "graphicEngine.h"

GraphicEngine::GraphicEngine()
{
	alphaFading = 0;
	aspectRatio = 1.0;
	fadingSpeed = 1.0;
	screen = NULL;
	isFading = FALSE;
	shakingEffect = FALSE;
	shakeCounter = 0;
}

void GraphicEngine::init()
{
	shakeValues.push_back(4);
	shakeValues.push_back(-4);
	shakeValues.push_back(4);
	shakeValues.push_back(-4);
	shakeValues.push_back(2);
	shakeValues.push_back(-2);
	shakeValues.push_back(1);
	shakeValues.push_back(-1);

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

	//Check if the texture is already loaded
	if(textures.find(path) == textures.end())
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

		//Save texture for later reuse
		textures.insert(make_pair(path, aSurface));
		return aSurface;
	}
	else
	{
		return textures.at(path);
	}
}

void GraphicEngine::drawFrame()
{
#if USE_OPENGL
	//Reset the view matrix
    glLoadIdentity();
    //Perform a translation so that we are starting our coordinates at point (0,0)
	glTranslatef(-aspectRatio, -1 , -1);
#endif

	//Shift the camera in one direction
	if(shakingEffect)
	{
		shakeCamera(0);
	}

	if(!toDisplay.empty())
	{
		//Sort the toDisplay so that they are displayed from back to front
		//sort(toDisplay.begin(), toDisplay.end(), sortDisplayedElement);

		for (std::vector<Drawable *>::iterator aDrawable = toDisplay.begin() ; aDrawable != toDisplay.end(); ++aDrawable)
		{
			draw(*aDrawable);
		}
	}
	else
	{
		cout<<"Warning: GraphicEngine (drawFrame): The set of elements to display is empty\n";
	}

	for (list<ParticleEffect *>::iterator anEffect = particleEffects.begin() ; anEffect != particleEffects.end(); anEffect++)
	{
		drawEffect(*anEffect);
		(*anEffect)->animate();

		if((*anEffect)->currentFrame >= (*anEffect)->animationLength - 1 )
		{
			delete (*anEffect);
			particleEffects.remove(*anEffect--);
		}
	}

	//Check for effects to perform
	if(isFading)
	{
		fadeOut();
	}

	//Put the camera back in place to create the illusion of shaking
	if(shakingEffect)
	{
		shakeCamera(1);
		if(shakeCounter == 8)
		{
			shakingEffect = FALSE;
			shakeCounter = 0;
		}
	}

	displayFrame();
}

//Draw an object on the screen
int GraphicEngine::draw(Drawable * sprite)
{

#if USE_OPENGL
	//If we need blending
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
		float zX = sprite->getPosX()/(SCREEN_WIDTH/(aspectRatio*2));
		float zY = (SCREEN_HEIGHT - sprite->getPosY())/(SCREEN_HEIGHT/2);

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
		float zX = sprite->getPosX()/(SCREEN_WIDTH/(aspectRatio*2));
		float zY = (SCREEN_HEIGHT - sprite->getPosY())/(SCREEN_HEIGHT/2);

		//Translate back to 0,0 in order to perform the rotation
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

	//Deactivate blending
	if(sprite->toBlend)
	{
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

//Draw particle effects
void GraphicEngine::drawEffect(ParticleEffect * anEffect)
{
	glDisable(GL_TEXTURE_2D);

	glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);

	//Set the width for the lines
	//glLineWidth(25.5f);

	//Draw Lines
	glBegin(GL_LINES);
	//Set color values and opacity
	glColor4f(anEffect->colorR, anEffect->colorG, anEffect->colorB, anEffect->opacity);

	for (list<LineEffect *>::iterator aLine = anEffect->lineEffects.begin(); aLine != anEffect->lineEffects.end(); ++aLine)
	{
		//Set the width for the lines
		glLineWidth((*aLine)->lineWidth);

		//Create a temporary context in case we perform specific transformation for that object
		glPushMatrix();

		//Draw a Line
		glVertex3f(((*aLine)->destX/(SCREEN_WIDTH/(aspectRatio*2))), (SCREEN_HEIGHT - (*aLine)->destY)/(SCREEN_HEIGHT/2), 0);
		glVertex3f(((*aLine)->posX/(SCREEN_WIDTH/(aspectRatio*2))), (SCREEN_HEIGHT - (*aLine)->posY)/(SCREEN_HEIGHT/2), 0);

		//Discard the temporary context
		glPopMatrix();
	}
	glEnd();

	glPointSize(2.0);

	//Draw points
	glBegin(GL_POINTS);
	//Set color values and opacity
	glColor4f(anEffect->colorR, anEffect->colorG, anEffect->colorB, anEffect->opacity);
	for (list<PointEffect *>::iterator aPoint = anEffect->pointEffects.begin(); aPoint != anEffect->pointEffects.end(); ++aPoint)
	{
		//Create a temporary context in case we perform specific transformation for that object
		glPushMatrix();

		//Draw a point
		glVertex3f(((*aPoint)->posX/(SCREEN_WIDTH/(aspectRatio*2))), (SCREEN_HEIGHT - (*aPoint)->posY)/(SCREEN_HEIGHT/2), 0);

		//Discard the temporary context
		glPopMatrix();
	}
	glEnd();

	//Restore default blending parameters
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

//Shake the camera
void GraphicEngine::shakeCamera(int aSense)
{
	if(!aSense)
	{
		glTranslatef(shakeValues.at(shakeCounter)/(SCREEN_WIDTH/(aspectRatio*2)), 0.0, 0.0);
	}
	else {
		glTranslatef(-shakeValues.at(shakeCounter)/(SCREEN_WIDTH/(aspectRatio*2)), 0.0, 0.0);
		shakeCounter++;
	}
}

//Load a font and store it in available fonts
int GraphicEngine::addFont(string aName, string path, int size)
{
	TTF_Font * font;
	font = TTF_OpenFont(path.c_str(), size);
	if (font == NULL)
    {
        cerr << "TTF_OpenFont() Failed: " << TTF_GetError() << endl;
        return 0;
    }
	availableFonts.insert(make_pair(aName, font));
	return 1;
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

//Initialize fading out
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
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

//Free all the loaded textures
void GraphicEngine::freeTextures()
{
	for (map<SDL_Surface *, GLuint>::iterator anOGLTexture = openGLTextures.begin() ; anOGLTexture != openGLTextures.end(); ++anOGLTexture)
	{
		glDeleteTextures(1, &((*anOGLTexture).second));
	}
	openGLTextures.clear();

	for (map<string, SDL_Surface *>::iterator aTexture = textures.begin() ; aTexture != textures.end(); ++aTexture)
	{
		SDL_FreeSurface((*aTexture).second);
	}
	textures.clear();
}

bool sortDisplayedElement(const Drawable * a, const Drawable * b)
{
	return (a->virtualDepth > b->virtualDepth);
}

