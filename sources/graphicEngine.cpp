/*
 * Graphic Engine
 * Contains graphic-related functions (loading texture, drawing, etc.)
 */

#include "include/graphicEngine.h"

GraphicEngine::GraphicEngine() {
	alphaFading = 0;
	aspectRatio = 1.0;
	fadingSpeed = 1.0;
	screen = NULL;
	isFading = false;
	shakingEffect = false;
	shakeALot = false;
	shakeCounter = 0;
	xOffsetFactor = yOffsetFactor = 0;
}

void GraphicEngine::setAspectRatio(float aValue) {
	aspectRatio = aValue;

	xOffsetFactor = SCREEN_WIDTH / 2;
	yOffsetFactor = SCREEN_HEIGHT / 2;
}

void GraphicEngine::init() {
	initOpenGL();
	initColors();
	shakeValues = {14,-14,12,-12,10,-10,8,-8,6,-6,6,-6,4,-4,4,-4,2,-2,1,-1};
}

//Load a texture as an SDL_Surface
SDL_Surface * GraphicEngine::loadTexture(string path, bool clamp) {
	//Check if the texture is already loaded
	if (textures.find(path) == textures.end()) {
		SDL_Surface * aSurface;
		SDL_Surface * tmp = IMG_Load(path.c_str());
		if (tmp == NULL) {
			cerr << "Unable to load image: " << SDL_GetError() << endl;
			return NULL;
		}

		aSurface = SDL_DisplayFormatAlpha(tmp);
		SDL_FreeSurface(tmp);

		GLuint oglTex;
		createOGLTexture(aSurface, &oglTex, clamp);
		openGLTextures.insert(make_pair(aSurface, oglTex));

		//Save texture for later reuse
		textures.insert(make_pair(path, aSurface));
		return aSurface;
	} else {
		return textures.at(path);
	}
}

void GraphicEngine::drawFrame() {
	//Reset the view matrix
	glLoadIdentity();
	//Perform a translation so that we are starting our coordinates at point (0,0)
	glTranslatef(-xOffsetFactor, -yOffsetFactor, -yOffsetFactor);

	//Shift the camera in one direction
	if (shakingEffect) {
		shakeCamera(false);
	}

	if (!toDisplay.empty()) {
		//Sort toDisplay so that elements are displayed from back to front
		sort(toDisplay.begin(), toDisplay.end(), sortDisplayedElement);

		for (vector<Drawable *>::iterator aDrawable = toDisplay.begin();
				aDrawable != toDisplay.end(); ++aDrawable) {
			draw(*aDrawable);
		}
	} else {
		cout
				<< "Warning: GraphicEngine (drawFrame): The set of elements to display is empty\n";
	}

	for (list<ParticleEffect *>::iterator anEffect = particleEffects.begin();
			anEffect != particleEffects.end(); anEffect++) {
		drawEffect(*anEffect);
		(*anEffect)->animate();

		if ((*anEffect)->currentFrame >= (*anEffect)->animationLength - 1) {
			delete (*anEffect);
			particleEffects.remove(*anEffect--);
		}
	}

	//Check for effects to perform
	if (isFading) {
		fadeOut();
	}

	//Put the camera back in place to create the illusion of shaking
	if (shakingEffect) {
		shakeCamera(true);
		if (shakeCounter == shakeValues.size()) {
			shakingEffect = false;
			shakeALot = false;
			shakeCounter = 0;
		}
	}

	displayFrame();
}

//Draw an object on the screen
int GraphicEngine::draw(Drawable * sprite) {
	//If we need blending
	if (sprite->toBlend)
		glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);

	//Set colors and opacity
	glColor4f(sprite->colorR, sprite->colorG, sprite->colorB, sprite->opacity);

	//Load a new context in case we perform specific transformation for that object
	glPushMatrix();

	//If some scaling operations are required
	if (sprite->scaleX != 1 || sprite->scaleY != 1)
		performScaling(sprite);

	//If some rotation operations are required
	if (sprite->rotationAngle != 0)
		performRotation(sprite);

	//Use the object texture
	if (sprite->textured) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, sprite->getOpenGLTexture());
	} else {
		glDisable(GL_TEXTURE_2D);
	}

	if (sprite->isBlinkingWhite) {
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
		glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_ADD);

		//Add the texture with itself so that it is made brighter
		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE);
		glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_TEXTURE);
		glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
	}

	float oX = sprite->getPosX() - sprite->getWidth() / 2;
	float oY = sprite->getPosY() - sprite->getHeight() / 2;
	float cX = sprite->getPosX() + sprite->getWidth() / 2;
	float cY = sprite->getPosY() + sprite->getHeight() / 2;

	//Draw a quadrilateral using triangle_strip
	glBegin(GL_TRIANGLE_FAN);
	//Texture coordinates to display
	glTexCoord2f(sprite->ogl_Xorigin, sprite->ogl_Ycorner);
	//Coordinates of the quadrilateral
	glVertex3f(oX, oY, 0.0f);

	glTexCoord2f(sprite->ogl_Xcorner, sprite->ogl_Ycorner);
	glVertex3f(cX, oY, 0.0f);

	glTexCoord2f(sprite->ogl_Xcorner, sprite->ogl_Yorigin);
	glVertex3f(cX, cY, 0.0f);

	glTexCoord2f(sprite->ogl_Xorigin, sprite->ogl_Yorigin);
	glVertex3f(oX, cY, 0.0f);
	glEnd();

	//Discard the new context
	glPopMatrix();

	if (sprite->isBlinkingWhite)
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//Deactivate blending
	if (sprite->toBlend)
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return 1;
}

//Draw particle effects
void GraphicEngine::drawEffect(ParticleEffect * anEffect) {
	glDisable(GL_TEXTURE_2D);

	glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);

	//Set the width for the lines
	//glLineWidth(25.5f);

	//Draw Lines
	glBegin(GL_LINES);
	//Set color values and opacity
	glColor4f(anEffect->colorR, anEffect->colorG, anEffect->colorB,
			anEffect->opacity);

	for (list<LineEffect *>::iterator aLine = anEffect->lineEffects.begin();
			aLine != anEffect->lineEffects.end(); ++aLine) {
		//Set the width for the lines
		glLineWidth((*aLine)->lineWidth);

		//Create a temporary context in case we perform specific transformation for that object
		glPushMatrix();

		//Draw a Line
		glVertex3f((*aLine)->destX, (*aLine)->destY, 0);
		glVertex3f((*aLine)->posX, (*aLine)->posY, 0);

		//Discard the temporary context
		glPopMatrix();
	}
	glEnd();

	//Draw circles
	for (list<CircleEffect *>::iterator aCircle =
			anEffect->circleEffects.begin();
			aCircle != anEffect->circleEffects.end(); ++aCircle) {
		glBegin(GL_TRIANGLE_FAN);

		glColor4f(anEffect->colorR, anEffect->colorG, anEffect->colorB,
				anEffect->opacity);

		//Start at center
		glVertex2f((*aCircle)->center.first, (*aCircle)->center.second);

		for (float i = 0; i <= (*aCircle)->slices; i++) {
			float t = 2 * PI * i / (*aCircle)->slices;
			glVertex2f((*aCircle)->center.first + sin(t) * (*aCircle)->radius,
					(*aCircle)->center.second + cos(t) * (*aCircle)->radius);
		}

		glEnd();
	}

	//Draw points using vertex and color arrays
	if (!(anEffect->pointEffects.empty())) {
		glPointSize(anEffect->pointEffects.front()->size);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, &anEffect->pointCoordinateValues.at(0));

		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_FLOAT, 0, &anEffect->pointColorValues.at(0));

		//Draw points
		glDrawArrays(GL_POINTS, 0, anEffect->pointEffects.size());

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
	}

	//Restore default blending parameters
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);

	anEffect->currentFrame++;
}

void GraphicEngine::displayFrame() {
	glfwSwapBuffers(Window);
}

void GraphicEngine::performScaling(Drawable * sprite) {
	//Translate to 0,0 in order to perform the scaling
	glTranslatef(sprite->getPosX(), sprite->getPosY(), 0.0);
	//Scale
	glScalef(sprite->scaleX, sprite->scaleY, 1.0f);
	//Translate back to the normal position
	glTranslatef(-sprite->getPosX(), -sprite->getPosY(), 0.0);
}

void GraphicEngine::performRotation(Drawable * sprite) {
	//Translate to 0,0 in order to perform the rotation
	glTranslatef(sprite->getPosX(), sprite->getPosY(), 0.0);
	//Rotate
	glRotatef(sprite->rotationAngle, sprite->rotX, sprite->rotY, sprite->rotZ);
	//Translate back to the normal position
	glTranslatef(-sprite->getPosX(), -sprite->getPosY(), 0.0);
}

//Start a shaking camera effect
void GraphicEngine::startShaking(int aLength, bool aLot) {
	shakeCounter = max(0, (int) (shakeValues.size() - aLength));
	shakeALot = aLot;
	shakingEffect = true;
}

//Shake the camera
void GraphicEngine::shakeCamera(bool aSense) {
	static bool yMove;
	static bool xMove;

	if (!aSense) {
		if (shakeALot) {
			//xMove = rand() % 2;
			xMove = true;
			yMove = true;
			//if(xMove)
			//{
			glTranslatef(shakeValues.at(shakeCounter), 0.0, 0.0);
			//	yMove = rand() % 2;
			//	}
			//	else
			//{
			//		yMove = true;
			//	}

			if (yMove)
				glTranslatef(0.0, shakeValues.at(shakeCounter), 0.0);
		} else {
			glTranslatef(shakeValues.at(shakeCounter), 0.0, 0.0);
			xMove = true;
			yMove = false;
		}
	} else {
		if (xMove)
			glTranslatef(-shakeValues.at(shakeCounter), 0.0, 0.0);

		if (yMove)
			glTranslatef(0.0, -shakeValues.at(shakeCounter), 0.0);

		shakeCounter++;
	}
}

//Load a font and store it in available fonts
int GraphicEngine::addFont(string aName, string path, int size) {
	TTF_Font * font;
	font = TTF_OpenFont(path.c_str(), size);
	if (font == NULL) {
		cerr << "TTF_OpenFont() Failed: " << TTF_GetError() << endl;
		return 0;
	}
	availableFonts.insert(make_pair(aName, font));
	return 1;
}

void GraphicEngine::initColors() {
	SDL_Color white = { 255, 255, 255 };
	SDL_Color red = { 255, 0, 0 };
	SDL_Color blue = { 29, 210, 199 };

	availableColors.insert(make_pair("WHITE", white));
	availableColors.insert(make_pair("RED", red));
	availableColors.insert(make_pair("BLUE", blue));
}

//Initialize fading out
void GraphicEngine::startFadingOut(int aFadingSpeed) {
	fadingSpeed = aFadingSpeed;
	isFading = true;
	alphaFading = 0;
}

//Perform a fade out effect
void GraphicEngine::fadeOut() {
	alphaFading = min(255, alphaFading + fadingSpeed);

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

	if (alphaFading == 255) {
		isFading = false;
	}
}

void GraphicEngine::fadeIn() {
	if (!isFading) {
		isFading = true;
		alphaFading = 255;
	}

	alphaFading = max(0, alphaFading - 1);

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

	if (alphaFading <= 0) {
		isFading = false;
	}
}

void GraphicEngine::createOGLTexture(SDL_Surface * aSurface, GLuint * oglTex,
		bool clamp) {
	glGenTextures(1, oglTex);
	glBindTexture(GL_TEXTURE_2D, *oglTex);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, aSurface->w, aSurface->h, 0, GL_BGRA,
			GL_UNSIGNED_BYTE, aSurface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (clamp) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
}

//Free all the loaded textures
void GraphicEngine::freeTextures() {
	for (map<SDL_Surface *, GLuint>::iterator anOGLTexture =
			openGLTextures.begin(); anOGLTexture != openGLTextures.end();
			++anOGLTexture) {
		glDeleteTextures(1, &((*anOGLTexture).second));
	}
	openGLTextures.clear();

	for (map<string, SDL_Surface *>::iterator aTexture = textures.begin();
			aTexture != textures.end(); ++aTexture) {
		SDL_FreeSurface((*aTexture).second);
	}
	textures.clear();
}

void GraphicEngine::deleteParticleEffect() {
	for (list<ParticleEffect *>::iterator aParticleEffect =
			particleEffects.begin(); aParticleEffect != particleEffects.end();
			++aParticleEffect) {
		delete (*aParticleEffect);
	}
	particleEffects.clear();
}

bool sortDisplayedElement(const Drawable * a, const Drawable * b) {
	return (a->virtualDepth > b->virtualDepth);
}

bool initOpenGL()
{
	// Initialise GLFW
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	// Open a window and create its OpenGL context
	Window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Molecular", NULL, NULL);
	if (Window == NULL) {
		fprintf(stderr,
				"Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(Window); // Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	//Initialize clear color (black and opaque)
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	//glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float ratio = (GLfloat) SCREEN_WIDTH / (GLfloat) SCREEN_HEIGHT;

	//Set the view
	//90.0f since cotangent(45) = 1 which simplifies the coordinates computing on the y axis
	//the x coordinates are to be multiply by the ratio
	gluPerspective(90.0f, ratio, 1.0f, 1000.0f);
	glm::mat4 Projection = glm::perspective(90.0f, ratio, 1.0f, 1000.0f);

	// Camera matrix
	glm::mat4 View = glm::lookAt(glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
	glm::vec3(0, 0, 0), // and looks at the origin
	glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
			);

	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-0.5, -0.5, -1000);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);

	//Check for error
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		cerr << "Error initializing OpenGL! " << gluErrorString(error) << endl;
		return false;
	}

	return true;
}
