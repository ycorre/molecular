/*
 * Contains the menu function
 */

#include "menu.h"

Menu::Menu(GraphicEngine * aGe, SoundEngine * aSe)
{
	ge = aGe;
	soundEngine = aSe;
	menuColor = ge->availableColors.at("BLUE");
	menuFont = ge->availableFonts.at("lCrystal");
	currentMenu = MENU_INTRO;
	nextMenu = MENU_INTRO;
	selected = 0;
	menuInTransition = false;
	startingGame = false;
	endIntro = 0;
	introLength = 0;
	game = NULL;
	selectedLevel = 0;
	newHighScoreRank = 0;
	startIntro = 0;
}

void Menu::loadMenu()
{
	loadMenuElements("conf/menus.json");
	readHighScore("conf/highscore");

	bkgTitle = loadedMenuElements.at("bkgTitle");
	bkgLevel = loadedMenuElements.at("bkgLevel");
	bkgOption = loadedMenuElements.at("bkgOption");
	logo = loadedMenuElements.at("logo");
	title  = loadedMenuElements.at("title");
	optionTitle = loadedMenuElements.at("optionTitle");
	levelTitle = loadedMenuElements.at("levelTitle");
	bubble1 = loadedMenuElements.at("bubble1");
	bubble2 = loadedMenuElements.at("bubble2");

	optionButton = loadedAnimMenuElements.at("btnOption");
	quitButton = loadedAnimMenuElements.at("btnQuit");
	levelSelectButton = loadedAnimMenuElements.at("btnLevel");

	menuElements.push_back(levelSelectButton);
	menuElements.push_back(quitButton);
	menuElements.push_back(optionButton);

	sort(menuElements.begin(), menuElements.end(), sortElement);

	credit.width = 640;
	credit.height = 480;
	credit.loadTexture("res/interface/credits2.gif");
	credit.posX = 280;
	credit.posY = 60;

	success.width = 640;
	success.height = 480;
	success.loadTexture("res/interface/end2.gif");
	success.posX = 280;
	success.posY = 60;

	gameOver.width = 640;
	gameOver.height = 480;
	gameOver.loadTexture("res/interface/gameover2.gif");
	gameOver.posX = 280;
	gameOver.posY = 60;

 	setMainSelection(0);

 	loadSelectedLevel();
	setSelectedLevel(0);

 	soundEngine->playSound("fractal");

 	startIntro = SDL_GetTicks();
 	endIntro = startIntro + introLength;
}

void Menu::loadSelectedLevel()
{
	string tmp = "Level";
	int levelNumber = 0;
	float stepY = 0;
	float stepX = 200;

	for(levelNumber = 0; levelNumber < 9; levelNumber++)
	{
		stringstream ss;
		ss << levelNumber+1;
		string numLevel = ss.str();
		Drawable aLevelPic;
		aLevelPic.loadTexture("res/interface/Level"+ numLevel +".png");
		aLevelPic.width = aLevelPic.getTexture()->w;
		aLevelPic.height = aLevelPic.getTexture()->h;
		stepX = stepX + 225;

		if(levelNumber % 3 == 0)
		{
			stepX = 300;
			stepY = stepY + 200;
		}
		aLevelPic.posX = stepX;
		aLevelPic.posY = stepY;
		aLevelPic.setAnimX(0);
		aLevelPic.setAnimY(0);
		aLevelPic.virtualDepth = 50;

		levelSelectElements.push_back(aLevelPic);

		AnimatedDrawable levelHighlight = loadedAnimMenuElements.at("levelHighlight");
		levelHighlight.posX = stepX - 10;
		levelHighlight.posY = stepY - 10;
		levelHighlight.virtualDepth = 51;

		levelSelectHalo.push_back(levelHighlight);

		Drawable levelLocked = loadedMenuElements.at("levelLock");
		levelLocked.posX = stepX - 2;
		levelLocked.posY = stepY - 2;
		levelLocked.virtualDepth = 48;

		levelLocks.push_back(levelLocked);
	}
}

void Menu::displayMenu()
{
	if(menuInTransition && !ge->isFading)
	{
		currentMenu = nextMenu;
		menuInTransition = false;
		if(startingGame)
		{
			startingGame = false;
			stringstream ss;
			ss << "level" << (selectedLevel) + 1;
			game->launchLevel(ss.str());
			return;
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
	bkgTitle.processDisplay();
	bubble1.processDisplay();
	bubble2.processDisplay();
	title.processDisplay();

	bubble1.setAnimX(bubble1.getAnimX() + 0.3);
	bubble2.setAnimX(bubble2.getAnimX() + 0.1);

	soundEngine->playMusic("musicMenu");

	for (vector<AnimatedDrawable>::iterator anElement = menuElements.begin() ; anElement != menuElements.end(); ++anElement)
	{
		(*anElement).processDisplay();
	}
}

void Menu::displayIntro()
{
	ge->toDisplay.push_back(&logo);
	int diff = endIntro - SDL_GetTicks();

	//Start fading out during the last second
	if(diff<1000 && !menuInTransition)
	{
		ge->startFadingOut(6);
		nextMenu = MENU_MAIN;
		menuInTransition = true;
	}
}

void Menu::displayLevelSelection()
{
	bkgLevel.processDisplay();
	levelTitle.processDisplay();

	for (vector<AnimatedDrawable>::iterator anElement = levelSelectHalo.begin() ; anElement != levelSelectHalo.end(); ++anElement)
	{
		(*anElement).animate();
		(*anElement).processDisplay();
	}

	for (list<Drawable>::iterator anElement = levelSelectElements.begin() ; anElement != levelSelectElements.end(); ++anElement)
	{
		(*anElement).animate();
		(*anElement).processDisplay();
	}

	for (vector<Drawable>::iterator anElement = levelLocks.begin() ; anElement != levelLocks.end(); ++anElement)
	{
		(*anElement).animate();
		(*anElement).processDisplay();
	}
}

void Menu::displayHighScore()
{
	bkgOption.processDisplay();
	optionTitle.processDisplay();

	for (list<Text *>::iterator anElement = highScoreElements.begin() ; anElement != highScoreElements.end(); ++anElement)
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
	credit.processDisplay();
}

void Menu::displayGameOver()
{
	gameOver.processDisplay();
}

void Menu::displaySuccess()
{
	success.processDisplay();
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

		case MENU_LEVELSELECT:
			{
				nextMenu = MENU_MAIN;
				menuElements.at(selected).setAnimation("hl");
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
			menuElements.at(selected).setAnimation("hl");
			break;

		case MENU_NEWHIGHSCORE:
			nextMenu = MENU_HIGHSCORE;
			break;

		default:
			break;
	}

	menuInTransition = true;
	ge->startFadingOut(6);

	return;
}

void Menu::loadMenuElements(string path)
{
	unsigned int index;

	//parse the configuration file
	ifstream file;
	Json::Value root;
	Json::Reader reader;

	file.open(path.c_str());
	int parsingSuccessful = reader.parse(file, root);
	if (!parsingSuccessful)
	{
		//report to the user the failure and their locations in the document.
		cout << "Failed to parse configuration:" << endl << reader.getFormattedErrorMessages();
	}

	//instantiate the corresponding drawable elements
	const Json::Value mainMenu = root["mainMenu"];
	const Json::Value drawable = mainMenu["drawable"];

	for (index = 0; index < drawable.size(); ++index)
	{
		Drawable tmp = Drawable(drawable[index]);
		loadedMenuElements.insert(make_pair(tmp.name, tmp));
	}

	const Json::Value animDrawable = mainMenu["animatedDrawable"];
	for (index = 0; index < animDrawable.size(); ++index)
	{
		AnimatedDrawable tmp = AnimatedDrawable(animDrawable[index]);
		loadedAnimMenuElements.insert(make_pair(tmp.name, tmp));
	}

	const Json::Value options = root["options"];
	const Json::Value optionDrawable = options["drawable"];
	for (index = 0; index < optionDrawable.size(); ++index)
	{
		Drawable tmp = Drawable(optionDrawable[index]);
		loadedMenuElements.insert(make_pair(tmp.name, tmp));
	}

	const Json::Value levelSelect = root["levelSelect"];
	const Json::Value  levelDrawable = levelSelect["drawable"];
	for (index = 0; index < levelDrawable.size(); ++index)
	{
		Drawable tmp = Drawable(levelDrawable[index]);
		loadedMenuElements.insert(make_pair(tmp.name, tmp));
	}

	const Json::Value lsAnimDrawable = levelSelect["animatedDrawable"];
	for (index = 0; index < lsAnimDrawable.size(); ++index)
	{
		AnimatedDrawable tmp = AnimatedDrawable(lsAnimDrawable[index]);
		loadedAnimMenuElements.insert(make_pair(tmp.name, tmp));
	}

	//Load Sounds
	const Json::Value sounds = root["sounds"];;
	for (index = 0; index < sounds.size(); ++index)
	{
		soundEngine->loadSoundFrom(sounds[index]);
	}

	//Load Music
	const Json::Value music = root["music"];;
	for (index = 0; index < music.size(); ++index)
	{
		soundEngine->loadMusicFrom(music[index]);
	}

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
	menuElements.at(selected).setAnimation("off");
	selected = aValue;
	menuElements.at(selected).setAnimation("hl");
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
	levelSelectHalo.at(selectedLevel).setAnimation("off");
	selectedLevel = aValue;
	levelSelectHalo.at(selectedLevel).setAnimation("hl");
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
	menuElements.at(selected).setAnimation("on");
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
    	levelLocks.at(i-1).display = game->lockedLevel.at(ss.str());
    }
}

void Menu::selectLevel()
{
	levelSelectHalo.at(selectedLevel).setAnimation("on");

	stringstream ss;
    ss << "level" << selectedLevel + 1;
	if(game->lockedLevel.at(ss.str()))
	{
		soundEngine->playSound("buzz");
	}
	else
	{
		startingGame = true;
		soundEngine->playSound("validated");
		soundEngine->fadeMusic("musicMenu", 1500);
		transition();
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
	for (vector<pair<string, unsigned int> >::iterator anElement = highScores.begin(); anElement != highScores.end(); ++anElement)
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
			for (vector<pair<string, unsigned int> >::iterator anElement = highScores.begin(); anElement != highScores.end(); ++anElement)
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

	for (list<Text *>::iterator anElement = highScoreElements.begin() ; anElement != highScoreElements.end(); ++anElement)
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


bool Menu::checkForNewHighScore()
{
	newHighScoreRank = 0;

	for (vector<pair<string, unsigned int> >::iterator anElement = highScores.begin(); anElement != highScores.end(); ++anElement)
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
			for (vector<pair<string, unsigned int> >::iterator anotherElement = highScores.begin(); anotherElement != highScores.end(); ++anotherElement)
			{
				if(anotherElement->first == " ")
				{
					break;
				}
				newHighScoreRank++;
			}
			if (newHighScoreRank > 5)
			{
				return false;
			}
			SDL_EnableUNICODE(SDL_ENABLE);
			return true;
		}
	}
	return false;
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

/*
 * Utility functions
 */
bool sortElement(AnimatedDrawable a, AnimatedDrawable b)
{
	return (a.posY < b.posY);
}

bool sortScores(pair<string, int>  a, pair<string, int>  b)
{
	return (a.second > b.second);
}

