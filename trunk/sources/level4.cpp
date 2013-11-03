/*
 * Contains level 4 functions
 */

#include "level4.h"

void Level4::loadLevel(Hero * aHero)
{
	activeElements.clear();
	toMerge.clear();
	loadConf();
	loadObject();
	hud = new HUD(ge);
	hud->loadHUDElements("conf/hud.conf");
	cameraSpeed = 1;
	hero = aHero;
	hero->getTexture("tie");
	hero->width = atoi(((configurationElements.at("tie")).at(0)).c_str());
	hero->height = atoi(((configurationElements.at("tie")).at(1)).c_str());
	hero->nbFrames = parseAnimationState((configurationElements.at("tie")).at(2));
	hero->resetHero();
	levelState = LEVEL_PLAYING;
	ending = fading = exiting = FALSE;
}

void Level4::loadObject()
{
	loadBackGround();
	loadTextures();
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
	string path = "res/decor.png";
	bigStars.loadTexture("res/GrandesEtoiles.png");
	bigStars.width = SCREEN_WIDTH;
	bigStars.height = SCREEN_HEIGHT;
	bigStars.posX = 0;
	bigStars.posY = 0;
	bigStars.state = 0;
	bigStars.animX = 0;
	bigStars.animY = 0;
	//activeElements.push_back(&bigStars);

	smallStars.loadTexture("res/PetitesEtoiles.png");
	smallStars.width = SCREEN_WIDTH;
	smallStars.height = SCREEN_HEIGHT;
	smallStars.posX = 0;
	smallStars.posY = 0;
	smallStars.state = 0;
	smallStars.animX = 0;
	smallStars.animY = 0;
	//activeElements.push_back(&smallStars);

	nebulae.loadTexture("res/NebuleuseEtoiles.png");
	nebulae.width = SCREEN_WIDTH;
	nebulae.height = SCREEN_HEIGHT;
	nebulae.posX = 0;
	nebulae.posY = 0;
	nebulae.state = 0;
	nebulae.animX = 0;
	nebulae.animY = 0;

	rayonH.texture = ge->loadTexture("res/rayonH.png");
	rayonH.width = 200;
	rayonH.height = 200;
	rayonH.posX = 200;
	rayonH.posY = 0;
	rayonH.state = 0;
	rayonH.animX = 0;
	rayonH.animY = 0;

	rayonV.texture = ge->loadTexture("res/rayonV.png");
	rayonV.width = 200;
	rayonV.height = 200;
	rayonV.posX = 400;
	rayonV.posY = 0;
	rayonV.state = 0;
	rayonV.animX = 0;
	rayonV.animY = 0;
	//activeElements.push_back(&nebulae);

	//fullBg.texture = SDL_CreateRGBSurface(SDL_SRCALPHA, 200, 200, 32, 0x000000ff,0x0000ff00,0x00ff0000, 0xff000000);
	///fullBg.texture = SDL_CreateRGBSurface(SDL_SRCALPHA, 200, 200, 32,0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
	//fullBg.texture = SDL_CreateRGBSurface(0, 200, 200, 32, 0, 0, 0, 0);//SCREEN_WIDTH, GAMEZONE_HEIGHT, 32, 0, 0, 0, 0);
	fullBg.texture = SDL_CreateRGBSurface(0, SCREEN_WIDTH, GAMEZONE_HEIGHT, 32, 0, 0, 0, 0);
	fullBg.width = SCREEN_WIDTH;
	fullBg.height = GAMEZONE_HEIGHT;
	fullBg.posX = 0;
	fullBg.posY = 0;
	fullBg.state = 0;
	fullBg.animX = 0;
	fullBg.animY = 0;


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

void Level4::mergeBg()
{
	int x,y;
	SDL_LockSurface(bigStars.texture);
	SDL_LockSurface(nebulae.texture);
	SDL_LockSurface(smallStars.texture);
	SDL_LockSurface(fullBg.texture);
	/*Uint8 *p1 = (Uint8 *) rayonV.texture->pixels; //+ (y * rayonV.texture->pitch) + (x * 4);
	Uint8 *p2 = (Uint8 *) rayonH.texture->pixels; //+ (y * rayonH.texture->pitch) + (x * 4);
	Uint8 *p3 = (Uint8 *) fullBg.texture->pixels; //;+ (y * fullBg.texture->pitch) + (x * 4);*/

	for(y=0; y<GAMEZONE_HEIGHT; y++)
	{
		for(x=0; x<SCREEN_WIDTH; x++)
		{
			//Here p is the address to the pixel we want to retrieve
/*			Uint8 *p1 = (Uint8 *) bigStars.texture->pixels + (y * bigStars.texture->pitch) + ((bigStars.animX + x) * 4);
			Uint8 *p2 = (Uint8 *) nebulae.texture->pixels + (y * nebulae.texture->pitch) + ((nebulae.animX + x) * 4);
			Uint8 *p3 = (Uint8 *) smallStars.texture->pixels + (y * smallStars.texture->pitch) + ((smallStars.animX + x) * 4);
			//Uint8 *p2 = (Uint8 *) rayonH.texture->pixels; //+ (y * rayonH.texture->pitch) + (x * 4);
			Uint8 *p4 = (Uint8 *) fullBg.texture->pixels + (y * fullBg.texture->pitch) + (x * 4);*/
		//	Uint8 *p1 = (Uint8 *) nebulae.texture->pixels + (y * nebulae.texture->pitch) + (x * 4);

		//	Uint8 pix1= p1[0];
		/*	p3[0] =  p2[0]; //+ p1[0];
			p3[1] =  p2[1];//+ p1[1];
			p3[2] =  p2[2];//+ p1[2];
			p3[3] =  p2[3];//+ p1[3];*/
		//	cout << p2[0] << ", " << pix1 << ", " << p3[0] <<endl;
/*			p4[0] = min(255, p2[0] + p1[0] + p3[0]);
			p4[1] = min(255, p2[1] + p1[1] + p3[1]);
			p4[2] = min(255, p2[2] + p1[2] + p3[2]);
			p4[3] = min(255, p2[3] + p1[3] + p3[3]);
*/
		}
	}
	SDL_UnlockSurface(bigStars.texture);
	SDL_UnlockSurface(nebulae.texture);
	SDL_UnlockSurface(fullBg.texture);
	SDL_UnlockSurface(smallStars.texture);
}


void Level4::drawLevel()
{
	checkEvent();

	pe->stayOnScreen(hero, make_pair(SCREEN_WIDTH, GAMEZONE_HEIGHT));

	for (std::list<Drawable *>::iterator anElement = activeElements.begin() ; anElement != activeElements.end(); ++anElement)
	{
		(*anElement)->animate();
		(*anElement)->processDisplay();
	}
	hud->displayUI();
	hud->displayHealth(hero->health);
	hud->displayLife(hero->nbLife);
	hud->displayScore(Score);

	hero->animate();
	//bg.animX = bg.animX + cameraSpeed;
	nebulae.animX = nebulae.animX + 0.5;
	bigStars.animX = bigStars.animX + 1.5;
	smallStars.animX = smallStars.animX + 1;

	//mergeBg();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
//	glBlendEquation(GL_FUNC_ADD);
	ge->draw(&nebulae);
	ge->draw(&bigStars);
	ge->draw(&smallStars);
	//ge->draw(toMerge, &fullBg);
	//activeElements.push_back(&rayonH);
	//activeElements.push_back(&fullBg);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendEquation(GL_FUNC_ADD);

	if(ending)
	{
		finishLevel();
	}
}

//function that handle the events (enemies apparitions, collision checks,  etc...)
void Level4::checkEvent()
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

	for (list<Laser*>::iterator aLaser = hero->lasers.begin(); aLaser != hero->lasers.end(); ++aLaser)
	{
		Laser * aL = *aLaser;
		if(pe->collisionDetection(aL, anElement))
		{
			anElement->processCollisionWith(aL);
			aL->toRemove = TRUE;
			hero->lasers.erase(aLaser++);
			return 1;
		}
	}
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
			fading = TRUE;
			exiting=FALSE;
		}
	}

	//Fade out
	if(fading)
	{
		ge->fadeOut();
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
}


