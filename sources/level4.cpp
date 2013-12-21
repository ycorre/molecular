/*
 * Contains level 4 functions
 * Level 4 is a dummy level use to test experimental features
 */

#include "level4.h"

Level4::Level4()
{

}

void Level4::loadLevel(Hero * aHero)
{
	activeElements.clear();
	toMerge.clear();
	loadLevelConfiguration("conf/Level1.json");

	hud = new HUD(ge);
	hud->loadHUDElements("conf/hud.conf");
	cameraSpeed = 1;
	hero = aHero;
	//hero->addTexture("tie", atomDead);
	hero->width = atoi(((configurationElements.at("tie")).at(0)).c_str());
	hero->height = atoi(((configurationElements.at("tie")).at(1)).c_str());
	hero->resetHero();
	levelState = LEVEL_PLAYING;
	ending = fading = exiting = FALSE;
}

void Level4::loadObject()
{
	loadTextures();
	loadBackGround();
	generateStarfield();
}

//Load all textures used for the level at the beginning
//Texture and objects are specified in the level configuration file
void Level4::loadTextures()
{
	for (map<string, vector<string> >::iterator anElement = configurationElements.begin(); anElement != configurationElements.end(); ++anElement)
	{
		ge->textures.insert(make_pair(anElement->first, ge->loadTexture((anElement->second).back())));
	}
}

void Level4::loadBackGround()
{
	Level::loadBackGround();
	//string path = "res/decor.png";
	bigStars.loadTexture("res/bkg_distant.png");
	bigStars.width = SCREEN_WIDTH;
	bigStars.height = SCREEN_HEIGHT;
	bigStars.posX = -SCREEN_WIDTH/2.0;
	bigStars.posY = SCREEN_HEIGHT/2.0;
	bigStars.setAnimX(0);
	bigStars.setAnimY(0);
	//activeElements.push_back(&bigStars);

	/*smallStars.loadTexture("res/PetitesEtoiles.png");
	smallStars.width = SCREEN_WIDTH;
	smallStars.height = SCREEN_HEIGHT;
	smallStars.posX = 0;
	smallStars.posY = 0;
	smallStars.state = 0;
	smallStars.setAnimX(0);
	smallStars.setAnimY(0);
	//activeElements.push_back(&smallStars);

	nebulae.loadTexture("res/NebuleuseEtoiles.png");
	nebulae.width = SCREEN_WIDTH;
	nebulae.height = SCREEN_HEIGHT;
	nebulae.posX = 0;
	nebulae.posY = 0;
	nebulae.state = 0;
	nebulae.setAnimX(0);
	nebulae.setAnimY(0);

	//fullBg.texture = SDL_CreateRGBSurface(SDL_SRCALPHA, 200, 200, 32, 0x000000ff,0x0000ff00,0x00ff0000, 0xff000000);
	///fullBg.texture = SDL_CreateRGBSurface(SDL_SRCALPHA, 200, 200, 32,0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
	//fullBg.texture = SDL_CreateRGBSurface(0, 200, 200, 32, 0, 0, 0, 0);//SCREEN_WIDTH, GAMEZONE_HEIGHT, 32, 0, 0, 0, 0);
	fullBg.texture = SDL_CreateRGBSurface(0, SCREEN_WIDTH, GAMEZONE_HEIGHT, 32, 0, 0, 0, 0);
	fullBg.width = SCREEN_WIDTH;
	fullBg.height = GAMEZONE_HEIGHT;
	fullBg.posX = 0;
	fullBg.posY = 0;
	fullBg.state = 0;
	fullBg.setAnimX(0);
	fullBg.setAnimY(0);/


	// toMerge.push_back(&bigStars);
	 //toMerge.push_back(&nebulae);
	// toMerge.push_back(&smallStars);

	 //toMerge.push_back(&rayonH);
	 //toMerge.push_back(&rayonV);

//cout<<"bpp "<< bpp << endl;
	//mergeBg();
	// ge->mergeImages(toMerge, &fullBg);

	//activeElements.push_back(&fullBg);
	/*int x,y;
	SDL_LockSurface(nebulae.texture);
	SDL_LockSurface(bigStars.texture);
	SDL_LockSurface(fullBg.texture);

	for(y=0; y<GAMEZONE_HEIGHT; y++)
	{
		for(x=0; x<SCREEN_WIDTH; x++)
		{
			//Here p is the address to the pixel we want to retrieve
			Uint8 *p1 = (Uint8 *) nebulae.texture->pixels + (y * nebulae.texture->pitch) + (x * 4);
			Uint8 *p2 = (Uint8 *) bigStars.texture->pixels + (y * bigStars.texture->pitch) + (x * 4);
			Uint8 *p3 = (Uint8 *) fullBg.texture->pixels + (y * fullBg.texture->pitch) + (x * 4);
			p3[0] = min(255, p2[0] + p1[0]);
			p3[1] = min(255, p2[1] + p1[1]);
			p3[2] = min(255, p2[2] + p1[2]);
			p3[3] = min(255, p2[3] + p1[3]);
		}
	}
	SDL_UnlockSurface(nebulae.texture);
	SDL_UnlockSurface(fullBg.texture);
	SDL_UnlockSurface(bigStars.texture);
	activeElements.push_back(&fullBg);*/
	soundEngine->addSound("sound/xwing_explode.wav", "xwing_explode");
	soundEngine->addSound("sound/xwing_fire.wav", "xwing_fire");
	soundEngine->addSound("sound/tie_fire.wav", "tie_fire");
	soundEngine->addSound("sound/tie_explode.wav", "tie_explode");
	soundEngine->addSound("sound/tie_hit.wav", "tie_hit");

}


void Level4::drawLevel()
{
	checkEvent();

	pe->stayOnScreen(hero, make_pair(SCREEN_WIDTH, GAMEZONE_HEIGHT));

	for (list<Drawable *>::iterator anElement = activeElements.begin() ; anElement != activeElements.end(); ++anElement)
	{
		(*anElement)->animate();
		(*anElement)->processDisplay();
	}
	hud->displayUI(hero);

	hero->animate();

	//bg.animX = bg.animX + cameraSpeed;
/*	nebulae.setAnimX(nebulae.getAnimX() + 0.5);
	bigStars.setAnimX(bigStars.getAnimX() + 1.5);
	smallStars.setAnimX(smallStars.getAnimX() + 1);*/

	//mergeBg();
/*	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
//	glBlendEquation(GL_FUNC_ADD);
	ge->draw(&nebulae);
	ge->draw(&bigStars);
	ge->draw(&smallStars);
	//ge->draw(toMerge, &fullBg);
	//activeElements.push_back(&rayonH);
	//activeElements.push_back(&fullBg);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendEquation(GL_FUNC_ADD);*/

	displayStarField();
//	testBackGround();

	if(ending)
	{
		finishLevel();
	}
}

void Level4::testBackGround()
{

	//  glLoadIdentity( );
   // glTranslatef( 0.0f, .0f, -5.5f );

	glDisable(GL_TEXTURE_2D);
	glColor4f(1., 1., 1., 1.0);

	for (std::vector<vector<float> >::iterator aStar = stars.begin() ; aStar != stars.end(); ++aStar)
	{
		glBegin(GL_QUADS);
			glVertex3f((*aStar).at(0), (*aStar).at(1), -(*aStar).at(2));
			glVertex3f((*aStar).at(3), (*aStar).at(1), -(*aStar).at(2));
			glVertex3f((*aStar).at(3), aStar->at(4),-(*aStar).at(2));
			glVertex3f((*aStar).at(0), (*aStar).at(4), -(*aStar).at(2));
		glEnd();

		/*glBegin(GL_POINTS);
			glVertex3f((*aStar).at(0), (*aStar).at(1), -(*aStar).at(2));

		glEnd();*/

		aStar->at(0)= aStar->at(0) - (1.0/1200.0);
		aStar->at(3)= aStar->at(3) - (1.0/1200.0);
	}

	for (std::vector<vector<float> >::iterator aStar = starPoints.begin() ; aStar != starPoints.end(); ++aStar)
	{
		glBegin(GL_POINTS);
			glVertex3f((*aStar).at(0), (*aStar).at(1), -(*aStar).at(2));
		glEnd();

		aStar->at(0)= aStar->at(0) - (1.0/1200.0);
	}
/*	float x0 = 0;
	glColor4f(1., 0., 0., 1.0);
	glBegin(GL_QUADS);
		glVertex3f(0, SCREEN_HEIGHT/600.0, 0);
		glVertex3f(SCREEN_WIDTH/2400.0, SCREEN_HEIGHT/600.0, 0.);
		glVertex3f(SCREEN_WIDTH/2400.0, SCREEN_HEIGHT/1200.0,0.);
		glVertex3f(0, SCREEN_HEIGHT/1200.0, 0.);
	glEnd();

	glColor4f(0., 0., 1., 1.0);
	//glTranslatef(0.0f,0.0f,-2.0f);
	glBegin(GL_QUADS);

		glVertex3f(x0 , 0.0f, 0);
		glVertex3f(SCREEN_WIDTH/2400.0 , 0.0f, 0.);
		glVertex3f(SCREEN_WIDTH/2400.0 , SCREEN_HEIGHT/1200.0,0.);
		glVertex3f(x0 , SCREEN_HEIGHT/1200.0, 0.);
	glEnd();

	glColor4f(0., 1., 0., 1.0);
	//glTranslatef(0.0f,0.0f,-2.0f);
	glBegin(GL_QUADS);
		glVertex3f(SCREEN_WIDTH/1200 , 0.0f, -1);
		glVertex3f(SCREEN_WIDTH/600 , 0.0f, -1.);
		glVertex3f(SCREEN_WIDTH/600 , SCREEN_HEIGHT/600,-1.);
		glVertex3f(SCREEN_WIDTH/1200 , SCREEN_HEIGHT/600, -1.);
	glEnd();

	glColor4f(1., 0., 1., 1.0);
	//glTranslatef(0.0f,0.0f,-2.0f);
	glBegin(GL_QUADS);
		glVertex3f(SCREEN_WIDTH/1200 , SCREEN_HEIGHT/600, -2);
		glVertex3f(SCREEN_WIDTH/600 , SCREEN_HEIGHT/600, -2.);
		glVertex3f(SCREEN_WIDTH/600 , SCREEN_HEIGHT/300,-2.);
		glVertex3f(SCREEN_WIDTH/1200 , SCREEN_HEIGHT/300, -2.);
	glEnd();
//
//	glTranslatef( 0.0f, 0.0f, 0.5f );*/
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glTranslatef( 0.0f, 0.0f, -maxDepth );
	glScalef(maxDepth, maxDepth, 1.0f);

	glBindTexture(GL_TEXTURE_2D, bigStars.getOpenGLTexture());
	glBegin(GL_QUADS);
		glTexCoord2f(bigStars.ogl_Xorigin, bigStars.ogl_Yorigin);
		glVertex3f((bigStars.posX)/1200.0, ((600- bigStars.posY))/600.0, 0);

		glTexCoord2f(bigStars.ogl_Xcorner, bigStars.ogl_Yorigin);
		glVertex3f(((bigStars.posX) + bigStars.width)/1200.0, ((600-bigStars.posY))/600, 0);

		glTexCoord2f(bigStars.ogl_Xcorner, bigStars.ogl_Ycorner);
		glVertex3f(((bigStars.posX ) + bigStars.width)/1200.0, ((600-(bigStars.posY + bigStars.height)))/600, 0);

		glTexCoord2f(bigStars.ogl_Xorigin, bigStars.ogl_Ycorner);
		glVertex3f((bigStars.posX)/1200.0, ((600-(bigStars.posY + bigStars.height)))/600, 0);
	glEnd();

		bigStars.setAnimX(bigStars.getAnimX() + 0.2);
}

#if 0
float xrot, yrot, zrot = 0;
void Level4::testBackGround()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	 glLoadIdentity( );
	    glTranslatef( 0., 0., -5.5f );


	    glRotatef( xrot, 1.0f, 0.0f, 0.0f); /* Rotate On The X Axis */
	       glRotatef( yrot, 0.0f, 1.0f, 0.0f); /* Rotate On The Y Axis */
	       glRotatef( zrot, 0.0f, 0.0f, 1.0f); /* Rotate On The Z Axis */

	       glBindTexture(GL_TEXTURE_2D, bigStars.oglTexture);

	   //	glDisable(GL_TEXTURE_2D);
	  // 	glColor4f(1., 0., 0., 1.0);
 glBegin(GL_QUADS);
   /* Front Face */
   /* Bottom Left Of The Texture and Quad */
   glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -1.0f, -1.0f, 1.0f );
   /* Bottom Right Of The Texture and Quad */
   glTexCoord2f( 1.0f, 1.0f ); glVertex3f(  1.0f, -1.0f, 1.0f );
   /* Top Right Of The Texture and Quad */
   glTexCoord2f( 1.0f, 0.0f ); glVertex3f(  1.0f,  1.0f, 1.0f );
   /* Top Left Of The Texture and Quad */
   glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -1.0f,  1.0f, 1.0f );

   /* Back Face */
   /* Bottom Right Of The Texture and Quad */
   glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -1.0f, -1.0f, -1.0f );
   /* Top Right Of The Texture and Quad */
   glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -1.0f,  1.0f, -1.0f );
   /* Top Left Of The Texture and Quad */
   glTexCoord2f( 1.0f, 1.0f ); glVertex3f(  1.0f,  1.0f, -1.0f );
   /* Bottom Left Of The Texture and Quad */
   glTexCoord2f( 1.0f, 0.0f ); glVertex3f(  1.0f, -1.0f, -1.0f );

   /* Top Face */
   /* Top Left Of The Texture and Quad */
   glTexCoord2f( 1.0f, 1.0f ); glVertex3f( -1.0f,  1.0f, -1.0f );
   /* Bottom Left Of The Texture and Quad */
   glTexCoord2f( 1.0f, 0.0f ); glVertex3f( -1.0f,  1.0f,  1.0f );
   /* Bottom Right Of The Texture and Quad */
   glTexCoord2f( 0.0f, 0.0f ); glVertex3f(  1.0f,  1.0f,  1.0f );
   /* Top Right Of The Texture and Quad */
   glTexCoord2f( 0.0f, 1.0f ); glVertex3f(  1.0f,  1.0f, -1.0f );

   /* Bottom Face */
   /* Top Right Of The Texture and Quad */
   glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -1.0f, -1.0f, -1.0f );
   /* Top Left Of The Texture and Quad */
   glTexCoord2f( 1.0f, 1.0f ); glVertex3f(  1.0f, -1.0f, -1.0f );
   /* Bottom Left Of The Texture and Quad */
   glTexCoord2f( 1.0f, 0.0f ); glVertex3f(  1.0f, -1.0f,  1.0f );
   /* Bottom Right Of The Texture and Quad */
   glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -1.0f, -1.0f,  1.0f );

   /* Right face */
   /* Bottom Right Of The Texture and Quad */
   glTexCoord2f( 0.0f, 0.0f ); glVertex3f( 1.0f, -1.0f, -1.0f );
   /* Top Right Of The Texture and Quad */
   glTexCoord2f( 0.0f, 1.0f ); glVertex3f( 1.0f,  1.0f, -1.0f );
   /* Top Left Of The Texture and Quad */
   glTexCoord2f( 1.0f, 1.0f ); glVertex3f( 1.0f,  1.0f,  1.0f );
   /* Bottom Left Of The Texture and Quad */
   glTexCoord2f( 1.0f, 0.0f ); glVertex3f( 1.0f, -1.0f,  1.0f );

   /* Left Face */
   /* Bottom Left Of The Texture and Quad */
   glTexCoord2f( 1.0f, 0.0f ); glVertex3f( -1.0f, -1.0f, -1.0f );
   /* Bottom Right Of The Texture and Quad */
   glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -1.0f, -1.0f,  1.0f );
   /* Top Right Of The Texture and Quad */
   glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -1.0f,  1.0f,  1.0f );
   /* Top Left Of The Texture and Quad */
   glTexCoord2f( 1.0f, 1.0f ); glVertex3f( -1.0f,  1.0f, -1.0f );
 glEnd( );
// glTranslatef( 0.0f, 0.0f, -2.5f );

 xrot += 0.3f; /* X Axis Rotation */
 yrot += 0.2f; /* Y Axis Rotation */
 zrot += 0.4f;
 //SDL_GL_SwapBuffers();
	//glColor4f(1.0, 1.0, 1.0, 1.0);
	//glEnable(GL_TEXTURE_2D);
}
#endif

//function that handle the events (enemies apparitions, collision checks,  etc...)
void Level4::checkEvent()
{
	for (list<Drawable *>::iterator anElement = activeElements.begin() ; anElement != activeElements.end(); ++anElement)
	{
		if((*anElement)->toRemove)
		{
			activeElements.erase(anElement++);
		}
		else
		{
			if((*anElement)->isEnemy())
			{
				checkEnemyCollision(*anElement);
				Enemy * anEnemy = dynamic_cast<Enemy *>(*anElement);
				anEnemy->fire();
			}
			if((*anElement)->isBonus() ||(*anElement)->isLaser())
			{
				checkCollision(*anElement);
			}
		}
	}
}


int Level4::checkEnemyCollision(Drawable * anElement)
{

	if(pe->collisionDetection(hero, anElement))
	{
		if(!(hero->invincible || hero->state == DEAD))
		{
			hero->processCollisionWith(anElement);
			anElement->processCollisionWith(hero);
			return 1;
		}
	}

	/*for (list<Laser*>::iterator aLaser = hero->getLasers()->begin(); aLaser != hero->getLasers()->end(); ++aLaser)
	{
		Laser * aL = *aLaser;
		if(pe->collisionDetection(aL, anElement))
		{
			anElement->processCollisionWith(aL);
			aL->toRemove = TRUE;
			hero->getLasers()->erase(aLaser++);
			return 1;
		}
	}*/
	return 0;
}

int Level4::checkCollision(Drawable * anElement)
{
	if (hero->state == DEAD)
	{
		return 0;
	}

	if(pe->collisionDetection(hero, anElement))
	{
		anElement->processCollisionWith(hero);
		hero->processCollisionWith(anElement);
		return 1;
	}
	return 0;
}

void Level4::createExplosion(int x, int y, int type)
{
	activeElements.push_back(new Explosion(x, y, type));
}

void Level4::createBonus(int x, int y, int type)
{
	activeElements.push_back(new Bonus(x, y, type));
}

void Level4::finishLevel()
{
	//First call: init
	if(!ending)
	{
		exiting = TRUE;
		hero->setState(EXITING);
		fading = FALSE;
	}

	//Play Victory sound


	//Get hero out of the screen to the right
	if(exiting)
	{
		//Start fading out
		if (hero->posX >= SCREEN_WIDTH - 100)
		{
			ge->startFadingOut(3);
			fading = TRUE;
			exiting=FALSE;
		}
	}

	//Fade out
	if(fading)
	{
		if (ge->isFading == FALSE)
		{
			fading = FALSE;
		}
	}

	ending = TRUE;

	//Make sure all the events have taken place
	if(!fading && !exiting)
	{
		//Aaaand we are out!
		endLevel();
	}
}

//Load all the configuration elements from a text file
//Format: elementName;width;height;listOfAnimationStatus$nbOfFramesInAnAnimation;pathToTextureFile
void Level4::loadConf()
{
	ifstream file;
	string line;
	string fileName = "conf/l1.conf";
	string token;
	string type;
	vector<string> confElements;
	string confElement;

	file.open(fileName.c_str());
	while(getline(file, line))
	{
		if(line.size()!=0) //Ignore empty lines
		{
			confElements.clear();
			istringstream myLine(line);
			getline(myLine, type, ';');

			while(getline(myLine, token, ';'))
			{
				confElements.push_back(token);
			}

			configurationElements.insert(make_pair(type, confElements));
		}
	}

loadStarConf();
}


