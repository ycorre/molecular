/*
 * Contains the menu function
 */

#include "menu.h"

Menu::Menu(GraphicEngine * aGe, SoundEngine * aSe)
{
	ge = aGe;
	soundEngine = aSe;
	menuColor = ge->availableColors.at("WHITE");
	menuFont = ge->availableFonts.at(0);
	currentMenu = MENU_INTRO;
	nextMenu = MENU_INTRO;
	selected = 0;
	menuInTransition = FALSE;
	startingGame = FALSE;
}

void Menu::loadMenu()
{
	loadMenuElements("conf/menu.conf");
	loadTextures();
	readHighScore("conf/highscore");

	bkgTitle = new Drawable();
	bkgTitle->width = 1200;
	bkgTitle->height = 800;
	bkgTitle->addTexture("bkgTitle");

	bkgLevel = new Drawable();
	bkgLevel->width = 1200;
	bkgLevel->height = 800;
	bkgLevel->addTexture("bkgLevel");

	bkgOption = new Drawable();
	bkgOption->width = 1200;
	bkgOption->height = 800;
	bkgOption->addTexture("bkgOption");

	logo = new Drawable();
	logo->width = 1200;
 	logo->height = 800;
	logo->addTexture("logo");

	title = new Drawable();
	title->width = 926;
	title->height = 152;
	title->addTexture("title");
	title->posX = 137;
	title->posY = 100;

	optionTitle = new Drawable();
	optionTitle->width = 1110;
	optionTitle->height = 70;
	optionTitle->addTexture("optionTitle");
	optionTitle->posX = 45;
	optionTitle->posY = 100;

	levelTitle = new Drawable();
	levelTitle->width = 892;
	levelTitle->height = 60;
	levelTitle->addTexture("levelTitle");
	levelTitle->posX = 154;
	levelTitle->posY = 100;

	bubble1 = new Drawable();
	bubble1->width = 1200;
	bubble1->height = 800;
	bubble1->addTexture("bubble1");

	bubble2 = new Drawable();
	bubble2->width = 1200;
	bubble2->height = 800;
	bubble2->addTexture("bubble2");

	bubbles = new CompositeDrawable();
	bubbles->width = 1200;
	bubbles->height = 800;
	bubbles->toMerge.push_back(bubble1);
	bubbles->toMerge.push_back(bubble2);
	bubbles->toMerge.push_back(bkgTitle);

	optionButton = new MultiTextureDrawable();
	optionButton->name = "option";
	optionButton->width = 384;
	optionButton->height = 64;
	optionButton->posX = 427;
	optionButton->posY = 450;
	optionButton->addTexture("optionOn");
	optionButton->addTexture("optionOff");
	optionButton->addTexture("optionHl");
	optionButton->textureState = "optionOff";

	quitButton = new MultiTextureDrawable();
	quitButton->name = "quit";
	quitButton->width = 384;
	quitButton->height = 64;
	quitButton->posX = 427;
	quitButton->posY = 600;
	quitButton->addTexture("quitOn");
	quitButton->addTexture("quitOff");
	quitButton->addTexture("quitHl");
	quitButton->textureState = "quitOff";

	levelSelectButton = new MultiTextureDrawable();
	levelSelectButton->name = "level";
	levelSelectButton->width = 384;
	levelSelectButton->height = 64;
	levelSelectButton->posX = 427;
	levelSelectButton->posY = 300;
	levelSelectButton->addTexture("levelOff");
	levelSelectButton->addTexture("levelOn");
	levelSelectButton->addTexture("levelHl");
	levelSelectButton->textureState = "levelHl";

	menuElements.push_back(levelSelectButton);
	menuElements.push_back(quitButton);
	menuElements.push_back(optionButton);

	sort(menuElements.begin(), menuElements.end(), sortElement);

	credit = new Drawable();
	credit->width = 640;
	credit->height = 480;
	credit->loadTexture("res/credits2.gif");
	credit->posX = 280;
	credit->posY = 60;

	success = new Drawable();
	success->width = 640;
	success->height = 480;
	success->loadTexture("res/end2.gif");
	success->posX = 280;
	success->posY = 60;

	gameOver = new Drawable();
	gameOver->width = 640;
	gameOver->height = 480;
	gameOver->loadTexture("res/gameover2.gif");
	gameOver->posX = 280;
	gameOver->posY = 60;

	soundEngine->addSound("sound/game_over.wav", "game_over");
	soundEngine->addSound("sound/start_ambient.wav", "start_ambient");
	soundEngine->addSound("sound/FractalZeptoReal.wav", "fractal");
	soundEngine->addMusic("sound/OptionsScreen.mp3", "musicMenu");

	soundEngine->addSound("sound/Valide.wav", "validated");
	soundEngine->addSound("sound/Buzz.wav", "buzz");
	soundEngine->addSound("sound/Vuit.wav", "selected");

 	soundEngine->playSound("fractal");

	loadIntro();
	setSelectedLevel(0);

 	startIntro = SDL_GetTicks();
 	endIntro = startIntro + introLength;
}

void Menu::loadIntro()
{
	string tmp= "Level";
	int levelNumber = 0;
	float stepY = 0;
	float stepX = 200;

	for(levelNumber = 0; levelNumber < 9; levelNumber++)
	{
		stringstream ss;
		ss << levelNumber+1;
		string numLevel = ss.str();
		Drawable * aLevelPic = new Drawable();
		aLevelPic->loadTexture("res/Level"+ numLevel +".png");
		aLevelPic->width = aLevelPic->getTexture()->w;
		aLevelPic->height = aLevelPic->getTexture()->h;
		stepX = stepX + 225;

		if(levelNumber % 3 == 0)
		{
			stepX = 300;
			stepY = stepY + 200;
		}
		aLevelPic->posX = stepX;
		aLevelPic->posY = stepY;
		aLevelPic->setAnimX(0);
		aLevelPic->setAnimY(0);

		levelSelectElements.push_back(aLevelPic);

		MultiTextureDrawable * levelHighlight = new MultiTextureDrawable();
		levelHighlight->name = "levelHighligt";
		levelHighlight->width = 192;
		levelHighlight->height = 192;
		levelHighlight->posX = stepX - 10;
		levelHighlight->posY = stepY - 10;
		levelHighlight->addTexture("levelHighligtOn");
		levelHighlight->addTexture("levelHighligtOff");
		levelHighlight->addTexture("levelHighligtHl");
		levelHighlight->textureState = "levelHighligtOff";

		levelSelectHalo.push_back(levelHighlight);

		Drawable * levelLocked = new Drawable();
		levelLocked->name = "levelHighligt";
		levelLocked->width = 176;
		levelLocked->height = 176;
		levelLocked->posX = stepX - 2;
		levelLocked->posY = stepY - 2;
		levelLocked->display = TRUE;
		levelLocked->addTexture("levelLock");

		levelLocks.push_back(levelLocked);
	}
}

void Menu::displayMenu()
{
	if(menuInTransition && !ge->isFading)
	{
		currentMenu = nextMenu;
		menuInTransition = FALSE;
		if(startingGame)
		{
			startingGame = FALSE;
			stringstream ss;
			ss << "level" << (selectedLevel%2) +1;
			game->launchLevel(ss.str());
		}
	}

	switch(currentMenu)
	{
		case MENU_MAIN:
			displayMainMenu();
			break;

		case MENU_LEVELSELECT:
			displayLevelSelection();
			break;

		case MENU_INTRO:
			displayIntro();
			break;

		case MENU_HIGHSCORE:
			displayHighScore();
			break;

		case MENU_NEWHIGHSCORE:
			enterHighScore();
			break;

		case MENU_CREDIT:
			displayCredit();
			break;

		case MENU_GAMEOVER:
			displayGameOver();
			break;

		case MENU_SUCCESS:
			displaySuccess();
			break;

		default:
			break;
	}
}

void Menu::displayMainMenu()
{
	ge->toDisplay.push_back(bubbles);
	ge->toDisplay.push_back(title);

	bubble1->setAnimX(bubble1->getAnimX()+0.3);
	bubble2->setAnimX(bubble2->getAnimX()+0.1);

	for (std::vector<MultiTextureDrawable *>::iterator anElement = menuElements.begin() ; anElement != menuElements.end(); ++anElement)
	{
		(*anElement)->animate();
		(*anElement)->processDisplay();
	}
}

void Menu::displayIntro()
{
	ge->toDisplay.push_back(logo);
	int diff = endIntro - SDL_GetTicks();

	//Start fading out during the last second
	if(diff<1000 && !menuInTransition)
	{
		soundEngine->playMusic("musicMenu");
		ge->startFadingOut(6);
		nextMenu = MENU_MAIN;
		menuInTransition = TRUE;
	}
}

void Menu::displayLevelSelection()
{
	ge->toDisplay.push_back(bkgLevel);
	ge->toDisplay.push_back(levelTitle);

	for (std::vector<MultiTextureDrawable *>::iterator anElement = levelSelectHalo.begin() ; anElement != levelSelectHalo.end(); ++anElement)
	{
		(*anElement)->animate();
		(*anElement)->processDisplay();
	}

	for (std::vector<Drawable *>::iterator anElement = levelSelectElements.begin() ; anElement != levelSelectElements.end(); ++anElement)
	{
		(*anElement)->animate();
		(*anElement)->processDisplay();
	}

	for (std::vector<Drawable *>::iterator anElement = levelLocks.begin() ; anElement != levelLocks.end(); ++anElement)
	{
		(*anElement)->animate();
		(*anElement)->processDisplay();
	}
}

void Menu::displayHighScore()
{
	ge->toDisplay.push_back(bkgOption);
	ge->toDisplay.push_back(optionTitle);

	for (std::vector<Text *>::iterator anElement = highScoreElements.begin() ; anElement != highScoreElements.end(); ++anElement)
	{
		(*anElement)->animate();
		if((*anElement)->display)
		{
			ge->toDisplay.push_back(*anElement);
		}
	}
}

void Menu::displayCredit()
{
	ge->toDisplay.push_back(credit);
}

void Menu::displayGameOver()
{
	ge->toDisplay.push_back(gameOver);
}

void Menu::displaySuccess()
{
	ge->toDisplay.push_back(success);
}

//State the transitions between the menus
void Menu::transition()
{
	switch(currentMenu)
	{
		case MENU_GAMEOVER:
			if (checkForNewHighScore())
			{
				nextMenu = MENU_NEWHIGHSCORE;
			}
			else
			{
				nextMenu =  MENU_MAIN;
			}
			break;

		case MENU_INTRO:
			nextMenu = MENU_MAIN;
			soundEngine->playMusic("musicMenu");
			break;

		case MENU_LEVELSELECT:
			{
				nextMenu = MENU_MAIN;
				menuElements.at(selected)->textureState = menuElements.at(selected)->name + "Hl";
			}
			break;

		case MENU_SUCCESS:
			nextMenu = MENU_CREDIT;
			break;

		case MENU_CREDIT:
			if (checkForNewHighScore())
			{
				nextMenu = MENU_NEWHIGHSCORE;
			}
			else
			{
				nextMenu = MENU_HIGHSCORE;
			}

			break;

		case MENU_HIGHSCORE:
			nextMenu = MENU_MAIN;
			menuElements.at(selected)->textureState = menuElements.at(selected)->name + "Hl";
			break;

		case MENU_NEWHIGHSCORE:
			nextMenu = MENU_HIGHSCORE;
			break;

		default:
			break;
	}

	menuInTransition = TRUE;
	ge->startFadingOut(6);

	return;
}

//Load the menu from the configuration file
void Menu::loadMenuElements(string path)
{
	//parse the configuration file
	ifstream file;
	string line;
	string token;
	string type;
	vector<string> confElements;
	string confElement;

	file.open(path.c_str());
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

	//Instantiate the corresponding drawable elements
/*	for (std::map<string, vector<string> >::iterator anElement = configurationElements.begin() ; anElement != configurationElements.end(); ++anElement)
	{
		Text * tmp = new Text(menuColor, menuFont);

		tmp->width = 300;
		tmp->height = 300;
		tmp->posX = atoi(((configurationElements.at(anElement->first)).at(0)).c_str());
		tmp->posY = atoi(((configurationElements.at(anElement->first)).at(1)).c_str());
		tmp->write(anElement->first);

		menuElements.push_back(tmp);
	}

	sort(menuElements.begin(), menuElements.end(), sortElement);*/
}

void Menu::selectionMove(int direction)
{
	soundEngine->playSound("selected");
	switch(currentMenu)
	{
		case MENU_MAIN:
			mainSelectionMove(direction);
			break;

		case MENU_LEVELSELECT:
			levelSelectionMove(direction);
			break;

		default:
			break;
	}
}

void Menu::mainSelectionMove(int direction)
{
	switch(direction)
	{
		case UP:
			setMainSelection((selected - 1 + menuElements.size()) % menuElements.size());
			break;

		case DOWN:
			setMainSelection((selected + 1) % menuElements.size());
			break;

		default:
				break;
	}
}

void  Menu::setMainSelection(int aValue)
{
	menuElements.at(selected)->textureState = menuElements.at(selected)->name + "Off";
	selected = aValue;
	menuElements.at(selected)->textureState = menuElements.at(selected)->name + "Hl";
}


void Menu::levelSelectionMove(int direction)
{
	switch(direction)
	{
		case RIGHT:
			setSelectedLevel((selectedLevel + 1) % levelSelectHalo.size());
			break;

		case LEFT:
			setSelectedLevel((selectedLevel - 1 + levelSelectHalo.size()) % levelSelectHalo.size());
			break;

		case DOWN:
			setSelectedLevel((selectedLevel + 3) % levelSelectHalo.size());
			break;

		case UP:
			setSelectedLevel((selectedLevel - 3 + levelSelectHalo.size()) % levelSelectHalo.size());
			break;

		default:
			break;
	}
}

void Menu::setSelectedLevel(int aValue)
{
	levelSelectHalo.at(selectedLevel)->textureState = levelSelectHalo.at(selectedLevel)->name + "Off";
	selectedLevel = aValue;
	levelSelectHalo.at(selectedLevel)->textureState = levelSelectHalo.at(selectedLevel)->name + "Hl";
}

void Menu::select()
{
	switch(currentMenu)
	{
		case MENU_MAIN:
			selectMainMenu();
			break;

		case MENU_LEVELSELECT:
			selectLevel();
			break;

		default:
			break;
	}
}

void Menu::selectMainMenu()
{
	menuElements.at(selected)->textureState = menuElements.at(selected)->name + "On";
	if(selected == 0)
	{
		soundEngine->playSound("validated");
		setSelectedLevel(selectedLevel);
		updateLocks();
		nextMenu = MENU_LEVELSELECT;
		transition();
		return;
	}

	if(selected == 1)
	{
		soundEngine->playSound("validated");
		nextMenu = MENU_HIGHSCORE;
		transition();
		return;
	}

	if(selected == 2)
	{
		game->quitGame();
		return;
	}
}

void Menu::updateLocks()
{
	int i;
    for(i = 1; i<10; i++)
    {
    	stringstream ss;
    	ss<< "level" << i;
    	levelLocks.at(i-1)->display = game->lockedLevel.at(ss.str());
    }
}

void Menu::selectLevel()
{
	levelSelectHalo.at(selectedLevel)->textureState = levelSelectHalo.at(selectedLevel)->name + "On";

	stringstream ss;
    ss<< "level" << selectedLevel + 1;
	if(game->lockedLevel.at(ss.str()))
	{
		soundEngine->playSound("buzz");
	}
	else
	{
		startingGame = TRUE;
		soundEngine->playSound("validated");
		transition();
	}
}

void Menu::loadTextures()
{
	for (map<string, vector<string> >::iterator anElement = configurationElements.begin(); anElement != configurationElements.end(); ++anElement)
	{
		ge->textures.insert(make_pair(anElement->first, ge->loadTexture((anElement->second).back())));
	}
}

/*
 * High Scores related functions
 */

//Load the menu from the configuration file
void Menu::readHighScore(string path)
{
	//parse the configuration file
	ifstream file;
	long int tmp;
	string line;
	string token;
	string type;
	vector<string> confElements;
	string confElement;

	file.open(path.c_str());
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
			//ignore unproperly formated lines
			if((!type.empty()) & (!confElements.front().empty()) & (!confElements.empty()))
			{
				tmp = strtol(confElements.front().c_str(),NULL,0);
				highScores.push_back(make_pair(type, tmp));
			}
			else
			{
				cout<< "Warning: Input not properly formated in the high scores file:\n" << type << ", " << confElements.front() << "endl";
			}
		}
	}
	sortHighScores();
	updateHighScore();
}


//Sort and trim the high scores and update the display
void Menu::updateHighScore()
{
	int ypos = 350;
	highScoreElements.clear();

	//Instantiate the scores as Text elements at position in accordance with their rank
	for (std::vector<pair<string, unsigned int> >::iterator anElement = highScores.begin(); anElement != highScores.end(); ++anElement)
	{
		Text * name = new Text(menuColor, menuFont, 350, ypos, 300, 300);
		Text * score = new Text(menuColor, menuFont, 750, ypos, 300, 300);

		name->write(anElement->first);
		highScoreElements.push_back(name);

		stringstream aScore;
		aScore << anElement->second;
		score->write(aScore.str());
		highScoreElements.push_back(score);

		ypos = ypos + 75;
	}
}

//Load the menu from the configuration file
void Menu::saveHighScore()
{
	  ofstream myfile ("conf/highscore");
	  if (myfile.is_open())
	  {
			for (std::vector<pair<string, unsigned int> >::iterator anElement = highScores.begin(); anElement != highScores.end(); ++anElement)
			{
				myfile << anElement->first << ";" << anElement->second <<"\n";
			}
		    myfile.close();
	  }
	  else cout << "Unable to open the high score file" << endl;

	  return;
}

//Allow the user to enter his name if he has a new highscore
void Menu::enterHighScore()
{
	Text * youWon =new Text(menuColor, menuFont, 480, 25, 300, 300);
	Text * instruction =new Text(menuColor, menuFont, 450, 75, 300, 300);

	youWon->write("New High Score!!!!");
	instruction->write("Please, enter your name:");
	ge->toDisplay.push_back(youWon);
	ge->toDisplay.push_back(instruction);

	for (std::vector<Text *>::iterator anElement = highScoreElements.begin() ; anElement != highScoreElements.end(); ++anElement)
	{
		(*anElement)->animate();
		if((*anElement)->display)
		{
			ge->toDisplay.push_back(*anElement);
		}
	}
}


//When entering a name in the high score, append a char
void Menu::addChar(char aChar)
{
	//Check that we don't exceed a number of characters
	if (newName.size() <= 18)
	{
		newName = newName + aChar;
		highScores.at(newHighScoreRank).first = newName;
		updateHighScore();
	}

	return;
}

//When entering a name in the high score, erase the last char
void Menu::eraseChar()
{
	if(!newName.empty())
	{
		newName.erase(newName.length() - 1);
		highScores.at(newHighScoreRank).first = newName;
		updateHighScore();
	}

	return;
}

void Menu::finishEnteringName()
{
	if(!newName.empty())
	{
		sortHighScores();
		updateHighScore();
		saveHighScore();
		transition();
		SDL_EnableUNICODE(SDL_DISABLE);
	}

	return;
}


int Menu::checkForNewHighScore()
{
	newHighScoreRank = 0;

	for (std::vector<pair<string, unsigned int> >::iterator anElement = highScores.begin(); anElement != highScores.end(); ++anElement)
	{
		if(Score > anElement->second)
		{
			//cout << Score << ", " << anElement->second << ", "  << anElement->first << endl;
			highScores.push_back(make_pair(" ", Score));
			newName = "";
			sortHighScores();
			updateHighScore();

			//Search for the position of the new high score
			//Useful when multiple high score have the same score value
			for (std::vector<pair<string, unsigned int> >::iterator anotherElement = highScores.begin(); anotherElement != highScores.end(); ++anotherElement)
			{
				if(anotherElement->first == " ")
				{
					break;
				}
				newHighScoreRank++;
			}
			if (newHighScoreRank > 5)
			{
				return FALSE;
			}
			SDL_EnableUNICODE(SDL_ENABLE);
			return TRUE;
		}
	}

	return FALSE;
}

//Sort scores in decreasing order
void Menu::sortHighScores()
{
	sort(highScores.begin(), highScores.end(), sortScores);

	//Make sure we have at most five values
	while(highScores.size() > 5)
	{
		//If not remove the lowest value(s)
		highScores.erase(highScores.end());
	}
}


bool sortElement(MultiTextureDrawable *a, MultiTextureDrawable* b)
{
	return (a->posY < b->posY);
}

bool sortScores(pair<string, int>  a, pair<string, int>  b)
{
	return (a.second > b.second);
}

