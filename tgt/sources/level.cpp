/*
 * Contains the level superclass functions
 */

#include "level.h"

void Level::loadLevel(Hero * aHero)
{

}

void Level::loadObject()
{
	loadBackGround();
	loadTextures();
}

//Load all textures used for the level at the beginning
//Texture and objects are specified in the level configuration file
void Level::loadTextures()
{
	for (map<string, vector<string> >::iterator anElement = configurationElements.begin(); anElement != configurationElements.end(); ++anElement)
	{
		ge->textures.insert(make_pair(anElement->first, ge->loadTexture((anElement->second).back())));
	}
}

void Level::loadBackGround()
{
#if USE_OPENGL
	displayBackGround = TRUE;

	bkg_distant.addTexture("bkg_distant");
	bkg_distant.width = SCREEN_WIDTH;
	bkg_distant.height = SCREEN_HEIGHT;
	bkg_distant.posX = -SCREEN_WIDTH/2.0;
	bkg_distant.posY =  SCREEN_HEIGHT/2.0;
	bkg_distant.state = 0;
	bkg_distant.setAnimX(0);
	bkg_distant.setAnimY(0);

	loadStarConf();
	generateStarfield();
#else


	background.toMerge.clear();
	bkg_near.width = SCREEN_WIDTH;
	bkg_near.height = SCREEN_HEIGHT;
	bkg_near.addTexture("bkg_near");
	bkg_near.posX = 0;
	bkg_near.posY = 0;
	bkg_near.state = 0;
	bkg_near.setAnimX(0);
	bkg_near.setAnimY(0);

	bkg_mid.width = SCREEN_WIDTH;
	bkg_mid.height = SCREEN_HEIGHT;
	bkg_mid.addTexture("bkg_mid");
	bkg_mid.posX = 0;
	bkg_mid.posY = 0;
	bkg_mid.state = 0;
	bkg_mid.setAnimX(0);
	bkg_mid.setAnimY(0);


	bkg_distant.addTexture("bkg_distant");
	bkg_distant.width = SCREEN_WIDTH;
	bkg_distant.height = SCREEN_HEIGHT;
	bkg_distant.posX = -SCREEN_WIDTH/2.0;
	bkg_distant.posY =  SCREEN_HEIGHT/2.0;
	bkg_distant.state = 0;
	bkg_distant.setAnimX(0);
	bkg_distant.setAnimY(0);

	background.toMerge.push_back(&bkg_distant);
	background.toMerge.push_back(&bkg_mid);
	background.toMerge.push_back(&bkg_near);
	activeElements.push_back(&background);
#endif
}

void Level::moveBackGround()
{
#if USE_OPENGL
	displayStarField();
#else
	//Move the backgrounds
	bkg_distant.setAnimX(bkg_distant.getAnimX() + 0.1);
	bkg_mid.setAnimX(bkg_mid.getAnimX() + 0.2);
	bkg_near.setAnimX(bkg_near.getAnimX() + 0.4);
#endif
}

void Level::drawLevel()
{
	checkEvent();

	pe->stayOnScreen(hero, make_pair(SCREEN_WIDTH, GAMEZONE_HEIGHT));

	for (std::list<Drawable *>::iterator anElement = activeElements.begin() ; anElement != activeElements.end(); ++anElement)
	{
		(*anElement)->animate();
		if((*anElement)->display)
		{
			ge->toDisplay.push_back(*anElement);
		}
	}
	hero->animate();
	background.setAnimX(background.getAnimX() + cameraSpeed);
}


//function that handle the events (enemies apparitions, collision checks,  etc...)
void Level::checkEvent()
{

	for (std::list<Drawable *>::iterator anElement = activeElements.begin() ; anElement != activeElements.end(); ++anElement)
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
				Enemy * anEnemy = static_cast<Enemy *>(*anElement);
				anEnemy->fire();
			}
			if((*anElement)->isBonus() ||(*anElement)->isLaser())
			{
				checkCollision(*anElement);
			}
		}
	}
}

int Level::checkEnemyCollision(Drawable * anElement)
{
	if(pe->collisionDetection(hero, anElement))
	{
		anElement->processCollisionWith(hero);
		hero->processCollisionWith(anElement);
		return 1;
	}

	for (list<Laser*>::iterator aLaser = hero->lasers.begin(); aLaser != hero->lasers.end(); ++aLaser)
	{
		Laser * aL = *aLaser;
		if(pe->collisionDetection(aL, anElement))
		{
			anElement->processCollisionWith(aL);
			aL->toRemove = TRUE;
			return 1;
		}
	}
	return 0;
}

int Level::checkCollision(Drawable * anElement)
{
	if(pe->collisionDetection(hero, anElement))
	{
		anElement->processCollisionWith(hero);
		hero->processCollisionWith(anElement);
		return 1;
	}
	return 0;
}

void Level::createExplosion(int x, int y, int type)
{
	activeElements.push_back(new Explosion(x, y, type));
}

void Level::createBonus(int x, int y, int type)
{
	activeElements.push_back(new Bonus(x, y, type));
}

//Load all the configuration elements from a text file
//Format: elementName;width;height;listOfAnimationStatus$nbOfFramesInAnAnimation;pathToTextureFile
void Level::loadConf()
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
}

void Level::heroLoseLife()
{
	hero->loseLife();
}

int Level::isOnScreen(Drawable * aDrawable)
{
	return pe->isOnScreen(aDrawable);
}

void Level::cleanLevel()
{
	for (std::list<Drawable *>::iterator anElement = activeElements.begin() ; anElement != activeElements.end(); ++anElement)
	{
		(*anElement)->clean();
	}
	activeElements.clear();
}

void Level::endLevel()
{
	cleanLevel();
	levelState = LEVEL_WON;
}

void Level::finishLevel()
{

}

//Generate a random set of stars
//Parameters for generation are set in conf/star.conf
void Level::generateStarfield()
{
	//Clear potential previous sets of stars
	stars.clear();
	starPoints.clear();
	starDrawable.clear();

	int i;
	float x, y, z;
	int size;

	//Generate stars that are represented as GL_POINT
	//Only 3 coordinates are needed (x, y, z)
	for (i=0; i<numberOfStarPoints; i++)
	{
		vector<float> starVector;
		z = 1.21 + (float) rand() / ((float) RAND_MAX/maxDepth);
		x = (float) (rand() / ((float) RAND_MAX/SCREEN_WIDTH)) - SCREEN_WIDTH/2;
		y = (float) (rand() / ((float) RAND_MAX/GAMEZONE_HEIGHT)) - GAMEZONE_HEIGHT/2;

		starVector.push_back((x / SCREEN_WIDTH)*z);
		starVector.push_back((y / GAMEZONE_HEIGHT)*z);
		starVector.push_back(z);

		starPoints.push_back(starVector);
	}

	//Generate stars that are represented as GL_QUAD
	//Need two more coordinates for the corner of the square
	for (i=0; i<numberOfStars; i++)
	{
		vector<float> starVector;
		z = 1.21 + (float) rand() / ((float) RAND_MAX/maxDepth);
		x = (float) (rand() / ((float) RAND_MAX/SCREEN_WIDTH)) - SCREEN_WIDTH/2;
		y = (float) (rand() / ((float) RAND_MAX/GAMEZONE_HEIGHT)) - GAMEZONE_HEIGHT/2;
		size = 1 + rand() % 2;

		starVector.push_back((x/ SCREEN_WIDTH)*z);
		starVector.push_back((y/ GAMEZONE_HEIGHT)*z);
		starVector.push_back(z);
		starVector.push_back(((x +size)/SCREEN_WIDTH)*z);
		starVector.push_back(((y +size)/GAMEZONE_HEIGHT)*z);

		stars.push_back(starVector);
	}

/*	for (i=0; i<numberOfStars; i++)
	{
		Drawable aStarDraw;
		aStarDraw.getTexture("star");
		aStarDraw.state = 0;

		aStarDraw.posZ =  1.21 + (float) rand() / ((float) RAND_MAX/maxDepth);
		aStarDraw.posX = x = (float) (rand() / ((float) RAND_MAX/SCREEN_WIDTH)) - SCREEN_WIDTH/2;
		aStarDraw.posY = y = (float) (rand() / ((float) RAND_MAX/GAMEZONE_HEIGHT)) - GAMEZONE_HEIGHT/2;
		aStarDraw.width = aStarDraw.height = 1 + rand() % 2;

		aStarDraw.ogl_Xorigin = ((aStarDraw.posX/ SCREEN_WIDTH)*aStarDraw.posZ);
		aStarDraw.ogl_Yorigin = ((aStarDraw.posY/ GAMEZONE_HEIGHT)*aStarDraw.posZ);
		aStarDraw.ogl_Xcorner = (((x + aStarDraw.width)/SCREEN_WIDTH)*aStarDraw.posZ);
		aStarDraw.ogl_Ycorner = (((y + aStarDraw.height)/GAMEZONE_HEIGHT)*aStarDraw.posZ);

		//cout << aStarDraw.ogl_Xorigin << ", " << aStarDraw.ogl_Xcorner << endl;

		starDrawable.push_back(&aStarDraw);
	}*/
}

//Display and move the star field
void Level::displayStarField()
{
	glDisable(GL_TEXTURE_2D);
	glColor4f(1., 1., 1., 1.0);

	//Draw squared stars
	for (std::vector<vector<float> >::iterator aStar = stars.begin() ; aStar != stars.end(); ++aStar)
	{
		glBegin(GL_QUADS);
			glVertex3f((*aStar).at(0), (*aStar).at(1), -(*aStar).at(2));
			glVertex3f((*aStar).at(3), (*aStar).at(1), -(*aStar).at(2));
			glVertex3f((*aStar).at(3), aStar->at(4),-(*aStar).at(2));
			glVertex3f((*aStar).at(0), (*aStar).at(4), -(*aStar).at(2));
		glEnd();

		//Move the stars
		aStar->at(0)= aStar->at(0) - (1.0/1200.0);
		aStar->at(3)= aStar->at(3) - (1.0/1200.0);

		//If a star moves out of the screen, teleport it to the other
		//to create an infinite starfield
		if(aStar->at(3) <= (float) -SCREEN_WIDTH*((*aStar).at(2)/2400.0))
		{
			float size = aStar->at(3) - aStar->at(0);
			aStar->at(0) = (float) (SCREEN_WIDTH*(*aStar).at(2))/2400.0;
			aStar->at(3) = aStar->at(0) + size;
		}
	}

	//Draw star points
	for (std::vector<vector<float> >::iterator aStar = starPoints.begin() ; aStar != starPoints.end(); ++aStar)
	{
		glBegin(GL_POINTS);
			glVertex3f((*aStar).at(0), (*aStar).at(1), -(*aStar).at(2));
		glEnd();

		aStar->at(0)= aStar->at(0) - (1.0/1200.0);

		if(aStar->at(0) <= (float) -SCREEN_WIDTH*((*aStar).at(2)/2400.0))
		{
			aStar->at(0) = (float) (SCREEN_WIDTH*(*aStar).at(2))/2400.0;
		}
	}

	glColor4f(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);

/*	glBindTexture(GL_TEXTURE_2D, (*starDrawable.begin())->oglTexture);
	for (std::vector<Drawable* >::iterator aStar = starDrawable.begin() ; aStar != starDrawable.end(); ++aStar)
	{
		glBindTexture(GL_TEXTURE_2D, (*aStar)->oglTexture);
		glBegin(GL_QUADS);
			glTexCoord2f(0., 0.);
			glVertex3f((*aStar)->ogl_Xorigin, (*aStar)->ogl_Yorigin, (*aStar)->posZ);

			glTexCoord2f(1., 0.);
			glVertex3f((*aStar)->ogl_Xcorner, (*aStar)->ogl_Yorigin, (*aStar)->posZ);

			glTexCoord2f(1.,1.);
			glVertex3f((*aStar)->ogl_Xcorner, (*aStar)->ogl_Ycorner, (*aStar)->posZ);

			glTexCoord2f(0., 1.);
			glVertex3f((*aStar)->ogl_Xorigin, (*aStar)->ogl_Ycorner, (*aStar)->posZ);

		glEnd();

		/*aStar->at(0)= aStar->at(0) - (5.0/1200.0);
		aStar->at(3)= aStar->at(3) - (5.0/1200.0);

		float size = aStar->at(3) - aStar->at(0);

		if(aStar->at(3) <= (float) -SCREEN_WIDTH*((*aStar).at(2)/2400.0))
		{
			aStar->at(0) = (float) (SCREEN_WIDTH*(*aStar).at(2))/2400.0;
			aStar->at(3) = aStar->at(0) + size;
		}
	}*/

	//Display the background
	if(displayBackGround)
	{
		glTranslatef(0.0f, 0.0f, -maxDepth);
		glScalef(maxDepth, maxDepth, 1.0f);

		glBindTexture(GL_TEXTURE_2D, bkg_distant.oglTexture);
		glBegin(GL_QUADS);
			glTexCoord2f(bkg_distant.ogl_Xorigin, bkg_distant.ogl_Yorigin);
			glVertex3f((bkg_distant.posX)/1200.0, ((600- bkg_distant.posY))/600.0, 0);

			glTexCoord2f(bkg_distant.ogl_Xcorner, bkg_distant.ogl_Yorigin);
			glVertex3f(((bkg_distant.posX) + bkg_distant.width)/1200.0, ((600-bkg_distant.posY))/600, 0);

			glTexCoord2f(bkg_distant.ogl_Xcorner, bkg_distant.ogl_Ycorner);
			glVertex3f(((bkg_distant.posX ) + bkg_distant.width)/1200.0, ((600-(bkg_distant.posY + bkg_distant.height)))/600, 0);

			glTexCoord2f(bkg_distant.ogl_Xorigin, bkg_distant.ogl_Ycorner);
			glVertex3f((bkg_distant.posX)/1200.0, ((600-(bkg_distant.posY + bkg_distant.height)))/600, 0);
		glEnd();

		bkg_distant.setAnimX(bkg_distant.getAnimX() + 0.2);
	}
}

//Load the star configuration parameters
void Level::loadStarConf()
{
	ifstream file;
	string line;
	string fileName = "conf/star.conf";
	string token;
	string type;
	vector<string> confElements;
	string confElement;

	file.open(fileName.c_str());
	while(getline(file, line))
	{
		if(line.size()!=0) //Ignore empty lines
		{
			istringstream myLine(line);
			getline(myLine, type, '#');

			confElements.push_back(type);
		}
	}

	numberOfStarPoints = atoi(confElements.at(0).c_str());
	numberOfStars = atoi(confElements.at(1).c_str());
	maxDepth = atoi(confElements.at(2).c_str());
}
