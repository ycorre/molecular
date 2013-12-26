/*
 * Main class of the game
 */

#include "game.h"

//TODO Use JSON for configuring all types of object ?
//Handle memory release
//Fix the music bug (repeating audio after music has been stop once (reload each time ?))
//Change menu & level loading methods:
//Modify the menu so that it is unloaded and reloaded each time we go back there
//Do a load level and a launch level method and separate both

//Timers: useful when pausing the game and for potential timing of the player
//Used as global variables and declared in common.h
Uint32 ProgramTimer;
Uint32 GameTimer;

Uint32 NextLoop, Interval, FPS = 60;
Uint32 Score = 0;

Game::Game()
{
	initSDL();
	initGame();
}

// Function to release/destroy the resources and go back to the desktop
void quit(int code)
{
    SDL_Quit();

    exit(code);
}


int main(int argc, char **argv)
{
    Game * game = new Game();

    //set the random generator seed
    srand(time(NULL));

    //Init loop timer
    NextLoop = 0;

    //Init interval in ms, i.e. the length of a loop
    //given by dividing 1 second by the number of targeted FPS
    Interval = 1000/FPS;

    //Launch the game
    game->mainLoop();
    
    return 1;
}

void Game::quitGame()
{
	quit(0);
}

int Game::mainLoop()
{
	int done = FALSE;
	SDL_Event event;

    //Init the program timer
    ProgramTimer = 0;
    GameTimer = 0;

    gameState = GAME_MENU;

    //Main loop
    while (!done)
	{
	    //Clear the elements to be displayed at the beginning of the loop
        graphicEngine.toDisplay.clear();
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glLoadIdentity();
    	//Process SDL events
	    while (SDL_PollEvent(&event))
		{
		    switch(event.type)
			{
		    	case SDL_KEYDOWN:
					//Handle key presses
					if(gameState == GAME_MENU)
					{
						keyboard->handleKeyPressMenu(&event.key.keysym, menu);
					}
					break;
			    
		    	case SDL_KEYUP:
					//Handle key presses
					keyboard->handleKeyUpHero(&event.key.keysym);
					break;

				case SDL_QUIT:
					//Handle quit requests
					done = TRUE;
					break;

				default:
					break;
			}
		}

	    keyboard->processKeyState();
	    keyboard->processKeyPress();

	    switch(gameState)
	    {
	    	case GAME_LOADMENU:
	    		delete menu;
	    		menu = new Menu(&graphicEngine, &soundEngine);
	    	    menu->loadMenu();
	    	    menu->game = this;
	    	    menu->currentMenu = MENU_MAIN;
	    	    menu->nextMenu = MENU_MAIN;
	    		gameState = GAME_MENU;
	    		break;

	    	case GAME_MENU:
	    		menu->displayMenu();
	    		break;

	    	case GAME_INGAME:
	    		keyboard->processKeyInGame(currentLevel->hero);
	    		if (gameState != GAME_INGAME)
	    			break;

	    		keyboard->processeMouseInGame(currentLevel->hero);
				currentLevel->drawLevel();
				if (currentLevel->levelState == LEVEL_WON)
				{
					launchNextLevel();
				}
				if (currentLevel->levelState == LEVEL_LOST)
				{
					currentLevel->cleanLevel();
					delete menu;
					menu = new Menu(&graphicEngine, &soundEngine);
					menu->loadMenu();
					menu->game = this;
					menu->currentMenu = MENU_MAIN;
				    menu->nextMenu = MENU_GAMEOVER;
					menu->menuInTransition = TRUE;
					gameState = GAME_MENU;
					  graphicEngine.toDisplay.clear();
				}
				break;

	    	default:
	    		break;
	    }

	    //Display the frame
		graphicEngine.drawFrame();
		
		//Control the speed of the game
		controlFPS();
		updateTimers();
	}
    //exit
    quit(0);

    //Should never get here
    return 0;
}

//Initialization of the game
int Game::initGame()
{
	loadConf();
	graphicEngine.init();
	graphicEngine.addFont("lCrystal", "res/fonts/LiquidCrystal.otf", 18);
	graphicEngine.addFont("lCrystal_16", "res/fonts/LiquidCrystal.otf", 16);
	graphicEngine.addFont("arial", "res/fonts/Arial.ttf", 18);
	graphicEngine.initColors();
	keyboard = new Keyboard();
	keyboard->game = this;
	physicEngine = *(new Pe());
	soundEngine = *(new SoundEngine());
	soundEngine.init();
	menu = new Menu(&graphicEngine, &soundEngine);

    Level1 * l1 = new Level1();
    Level2 * l2 = new Level2();

    l1->name = "level1";
    l2->name = "level2";

    levels.insert(make_pair("level1", l1));
    levels.insert(make_pair("level2", l2));

    int i;
    for(i = 1; i<10; i++)
    {
    	stringstream ss;
    	ss << "level" << i;
    	lockedLevel.insert(make_pair(ss.str(), TRUE));
    }

    lockedLevel.at("level1") = FALSE;
    lockedLevel.at("level2") = FALSE;

    //Init the menu
    menu->introLength = 2500;
    menu->loadMenu();
    menu->game = this;

	return 1;
}

//Initialization of SDL
int Game::initSDL()
{
	// Flags to pass to SDL_SetVideoMode
    int videoFlags;

    //This holds some info about our display
    const SDL_VideoInfo *videoInfo;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0)
	{
	    cerr <<  "Video initialization failed: " << SDL_GetError() << endl;
	    quit(1);
	}

    // Fetch the video info
    videoInfo = SDL_GetVideoInfo();

    if (!videoInfo)
	{
	    cerr << "Video query failed: " << SDL_GetError() << endl;
	    quit(1);
	}

    //The flags to pass to SDL_SetVideoMode
    videoFlags = SDL_DOUBLEBUF; 	   // Enable double buffering
#if USE_OPENGL
    videoFlags |= SDL_OPENGL;       // Store the palette in hardware
    videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */
#endif
    videoFlags |= SDL_HWPALETTE;


    // This checks to see if surfaces can be stored in memory
    if (videoInfo->hw_available)
    	videoFlags |= SDL_HWSURFACE;
    else
    	videoFlags |= SDL_SWSURFACE;

    // This checks if hardware blits can be done
    if (videoInfo->blit_hw)
    	videoFlags |= SDL_HWACCEL;

    // get a SDL surface from screen
#if USE_OPENGL

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    graphicEngine.screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, videoFlags);

#else
    graphicEngine.screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, videoFlags);
#endif

    //Verify there is a surface
    if (!graphicEngine.screen)
	{
	    cerr <<  "Video mode set failed: << " << SDL_GetError() << endl;
	    quit(1);
	}
    
    //Init font usage
    if (TTF_Init() != 0)
    {
    	cerr << "TTF_Init() Failed: " << TTF_GetError() << endl;
    	quit(1);
    }
    
    //Init Audio
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)  == -1)
	{
    	cout << "Warning: Audio_Init() Failed: " << SDL_GetError() << endl;
    	quit(1);
	}

    //Enable repetition of keyboard events
    SDL_EnableKeyRepeat(1, 250);//SDL_DEFAULT_REPEAT_INTERVAL);

    //Keep the mouse inside the game window
    SDL_WM_GrabInput(SDL_GRAB_ON);

    //Hide cursor
    SDL_ShowCursor(0);
    
    Drawable::ge = &graphicEngine;
    Sound::soundEngine = &soundEngine;
    graphicEngine.aspectRatio = (float) SCREEN_WIDTH / (float) SCREEN_HEIGHT;

#if USE_OPENGL
    initOpenGL();
#endif

    return 0;
}

void Game::pause()
{
	//If we are not in pause
	if (gameState != GAME_PAUSE)
	{
		//Then save current game state
		previousGameState = gameState;
		//and pause
		gameState = GAME_PAUSE;
	}
	else
	{
		//else revert to previous game state
		gameState = previousGameState;
	}
}

//Start a new game (beginning with level 1)
void Game::newGame()
{
	currentLevel = levels.at("level1");
	currentLevel->ge = &graphicEngine;
	currentLevel->pe = &physicEngine;
	currentLevel->soundEngine = &soundEngine;
	Drawable::lev = currentLevel;
	hero = new Hero();
    GameTimer = 0;
    Score = 0;

	currentLevel->loadLevel(hero);
	gameState = GAME_INGAME;
}

//Used to launch a specific level
//Should be used only in debug
void Game::launchLevel(string aLevel)
{
	currentLevel = levels.at(aLevel);
	currentLevel->ge = &graphicEngine;
	currentLevel->pe = &physicEngine;
	currentLevel->soundEngine = &soundEngine;
	Drawable::lev = currentLevel;
	hero = new Hero();
	GameTimer = 0;
	Score = 0;

	currentLevel->loadLevel(hero);
	gameState = GAME_INGAME;
}

//Launch the next level after a level has ended
void Game::launchNextLevel()
{
	unsigned int newLevel = 0;
	for (vector<string>::iterator aLevel = levelOrder.begin(); aLevel != levelOrder.end(); ++aLevel)
	{
		newLevel++;
		if (currentLevel->name == *aLevel)
			break;
	}

	if (newLevel >= 1)//levelOrder.size())
	{
		cout<<"Game won!!!!!!!!!!!\n";
		menu->currentMenu = MENU_SUCCESS;
		gameState = GAME_MENU;
		return;
	}

	currentLevel = levels.at(levelOrder.at(newLevel));

	currentLevel->ge = &graphicEngine;
	currentLevel->pe = &physicEngine;
	currentLevel->soundEngine = &soundEngine;
	Drawable::lev = currentLevel;

	currentLevel->loadLevel(hero);
	gameState = GAME_INGAME;
}

void Game::stopMusic()
{
	if(!soundEngine.musicMuted)
	{
		soundEngine.muteMusic();
	}
	else
	{
		soundEngine.musicMuted = FALSE;
		soundEngine.playMusic();
	}
}

void Game::muteAll()
{
	if(!soundEngine.mute)
	{
		soundEngine.muteAll();
	}
	else
	{
		soundEngine.mute = FALSE;
		soundEngine.playMusic();
	}
}

//Update Timers
void Game::updateTimers()
{
	ProgramTimer = ProgramTimer + Interval;

	if (gameState == GAME_INGAME)
		GameTimer = GameTimer + Interval;

	return;
}

//Control game speed
//If necessary, wait the required time until the next loop
void controlFPS()
{
	if(NextLoop > SDL_GetTicks())
		SDL_Delay(NextLoop - SDL_GetTicks());

	NextLoop = SDL_GetTicks() + Interval ;

	return;
}

//Load the game configuration file
void Game::loadConf()
{
	ifstream file;
	string line;
	string fileName = "conf/game.conf";
	string token;
	string anElement;
	vector<string> confElements;
	string confElement;

	file.open(fileName.c_str());
	while(getline(file, line))
	{
		if(line.size()!=0) //Ignore empty lines
		{
			confElements.clear();
			istringstream myLine(line);

			while(getline(myLine, token, ';'))
			{
				levelOrder.push_back(token);
			}
		}
	}
}

int initOpenGL()
{
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glEnable(GL_TEXTURE_2D);

	//Initialize clear color (black and opaque)
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	//glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float ratio = (GLfloat)SCREEN_WIDTH/(GLfloat)SCREEN_HEIGHT;

	//Set the view
	//90.0f since cotangent(45) = 1 which simplifies the coordinates computing on the y axis
	//the x coordinates are to be multiply by the ratio
	gluPerspective(90.0f, ratio, 1.0f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-0.5, -0.5 , -1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Check for error
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
	{
		cerr << "Error initializing OpenGL! " << gluErrorString(error) << endl;
		return FALSE;
	}

	return TRUE;
}
