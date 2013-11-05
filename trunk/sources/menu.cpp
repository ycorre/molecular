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
}

void Menu::loadMenu()
{
	mainBg = new Drawable();
	mainBg->width = 1200;
	mainBg->height = 600;
	mainBg->loadTexture("res/titre_wide.png");


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

 	arrow = new Drawable();
 	arrow->width = 48;
 	arrow->height = 52;
 	arrow->loadTexture("res/zombix2.png");
 	arrow->nbFrames.clear();
 	arrow->nbFrames.push_back(4);

	loadMenuElements("conf/menu.conf");
	readHighScore("conf/highscore");

	arrow->posX = menuElements.at(0)->posX - 70;
	arrow->posY = menuElements.at(0)->posY - 10;

	soundEngine->addSound("sound/game_over.wav", "game_over");
	soundEngine->addSound("sound/start_ambient.wav", "start_ambient");
}

void Menu::loadIntro()
{
	logo = new Drawable();
	logo->width = 1200;
 	logo->height = 480;
	logo->loadTexture("res/logo.png");


 	bbox = new Drawable();
 	bbox->width = 1200;
 	bbox->height = 600;
 	bbox->loadTexture("res/bbox.png");
	bbox->texture = SDL_DisplayFormat(bbox->texture);
 	bbox->setAnimX(0);
 	bbox->setAnimY(0);

 	startIntro = SDL_GetTicks();
 	endIntro = startIntro + introLength;
}

void Menu::displayMenu()
{
	if(menuInTransition && !ge->isFading)
	{
		currentMenu = nextMenu;
		menuInTransition = FALSE;
	}

	switch(currentMenu)
	{
		case MENU_MAIN:
			displayMainMenu();
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

	/*if(ge->isFading)
	{
		ge->fadeOut(6);
		return;
	}*/
}

void Menu::displayMainMenu()
{
	ge->toDisplay.push_back(mainBg);
	for (std::vector<Text *>::iterator anElement = menuElements.begin() ; anElement != menuElements.end(); ++anElement)
	{
		(*anElement)->animate();
		if((*anElement)->display)
		{
			ge->toDisplay.push_back(*anElement);
		}
	}
	arrow->updateAnimationFrame();
	ge->toDisplay.push_back(arrow);
}

void Menu::displayIntro()
{
	ge->toDisplay.push_back(bbox);
	ge->toDisplay.push_back(logo);
	int diff = endIntro - SDL_GetTicks();

	//Start fading out during the last second
	if(diff<1000 && !menuInTransition)
	{
		ge->startFadingOut(6);
		nextMenu = MENU_MAIN;
		menuInTransition = TRUE;
	}
}

void Menu::displayHighScore()
{
	ge->toDisplay.push_back(bbox);
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
	ge->toDisplay.push_back(bbox);
	ge->toDisplay.push_back(credit);
}

void Menu::displayGameOver()
{
	ge->toDisplay.push_back(bbox);
	ge->toDisplay.push_back(gameOver);
}

void Menu::displaySuccess()
{
	ge->toDisplay.push_back(bbox);
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
	for (std::map<string, vector<string> >::iterator anElement = configurationElements.begin() ; anElement != configurationElements.end(); ++anElement)
	{
		Text * tmp = new Text(menuColor, menuFont);

		tmp->width = 300;
		tmp->height = 300;
		tmp->posX = atoi(((configurationElements.at(anElement->first)).at(0)).c_str());
		tmp->posY = atoi(((configurationElements.at(anElement->first)).at(1)).c_str());
		tmp->write(anElement->first);

		menuElements.push_back(tmp);
	}

	sort(menuElements.begin(), menuElements.end(), sortElement);
}

void Menu::selectUp()
{
	menuElements.at(selected)->setColor(ge->availableColors.at("WHITE"));
	selected = (selected - 1 + menuElements.size()) % menuElements.size();
	arrow->posY = menuElements.at(selected)->posY - 10;
	menuElements.at(selected)->setColor(ge->availableColors.at("RED"));
}

void Menu::selectDown()
{
	menuElements.at(selected)->setColor(ge->availableColors.at("WHITE"));
	selected = (selected + 1) % menuElements.size();
	arrow->posY = menuElements.at(selected)->posY - 10;
	menuElements.at(selected)->setColor(ge->availableColors.at("RED"));
}

void Menu::select()
{
	if(selected == 0)
	{
		game->newGame();
		return;
	}

	if(selected == 5)
	{
		currentMenu = MENU_HIGHSCORE;
		nextMenu = MENU_HIGHSCORE;
		return;
	}

	if(selected == 6)
	{
		game->quitGame();
		return;
	}

	stringstream ss;
	ss << "level" << selected;
	game->launchLevel(ss.str());
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
	int ypos = 150;
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
	ge->toDisplay.push_back(bbox);
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
			//Useful when multiple high score have the same score
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


bool sortElement(Text *a, Text* b)
{
	return (a->posY < b->posY);
}

bool sortScores(pair<string, int>  a, pair<string, int>  b)
{
	return (a.second > b.second);
}

