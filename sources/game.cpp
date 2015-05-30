/*
 * Main class of the game
 */

#include "include/game.h"

//TODO Use JSON for configuring all types of object ?

//Timers: useful when pausing the game and for potential timing of the player
//Used as global variables and declared in common.h
Uint32 ProgramTimer;
Uint32 GameTimer;

Uint32 NextLoop, Interval, FPS = 60;
Uint32 Score = 0;

//Global variables
GLFWwindow * Window;
Level * CurrentLevel;
Game * CurrentGame;

Game::Game() {
	initSDL();
	initGame();
}

// Function to release/destroy the resources and go back to the desktop
void quit(int code) {
	glfwTerminate();
	SDL_Quit();
}


int main(int argc, char **argv)
{
    Game * game = new Game();
    CurrentGame = game;

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
	done = false;

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

    	//Process SDL events (Inputs, etc.)
        processEvents();

	    //Main processing
        mainProcessing();

	    //Display the frame
		graphicEngine.drawFrame();
		
		//Control the speed of the game
		controlFPS();
		updateTimers();
	}
    //exit
    quitGame();

    return 0;
}

void Game::processEvents() {
	glfwPollEvents();
}

void Game::mainProcessing()
{
    switch(gameState)
    {
    	case GAME_LOADMENU:
    		loadMenu(MENU_MAIN);
    		gameState = GAME_MENU;
    		break;

    	case GAME_MENU:
    		menu->displayMenu();
    		break;

    	case GAME_INGAME:
    		keyboard->processKeyInGame(CurrentLevel->hero);

    		//If we are no longer in the game then stop there
    		if (gameState != GAME_INGAME)
    			break;

    		keyboard->processeMouseInGame(CurrentLevel->hero);
			CurrentLevel->drawLevel();

			if (CurrentLevel->levelState == LEVEL_WON)
			{
				launchNextLevel();
			}
			if (CurrentLevel->levelState == LEVEL_LOST)
			{
				CurrentLevel->cleanLevel();
				delete CurrentLevel;
				loadMenu(MENU_GAMEOVER);
				gameState = GAME_MENU;
				graphicEngine.toDisplay.clear();
			}
			break;

    	default:
    		break;
    }
}

void Game::loadMenu(MenuState aMenu)
{
	menu = new Menu(&graphicEngine, &soundEngine);
	menu->currentMenu = MENU_MAIN;
	menu->nextMenu = aMenu;
	menu->loadMenu();
	menu->game = this;
	menu->menuInTransition = true;
}

void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods) {
	CurrentGame->keyboard->processKeyState(window, key, scancode, action, mods);
}

//Initialization of the game
int Game::initGame()
{
	graphicEngine.init();
	graphicEngine.addFont("lCrystal", "res/fonts/LiquidCrystal.otf", 18);
	graphicEngine.addFont("lCrystal_16", "res/fonts/LiquidCrystal.otf", 16);
	graphicEngine.initColors();
    graphicEngine.setAspectRatio((float) SCREEN_WIDTH / (float) SCREEN_HEIGHT);
	keyboard = new Keyboard();
	keyboard->game = this;
	soundEngine.init();

	glfwSetKeyCallback(Window, keyCallback);
    //Hide cursor
    glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    Drawable::graphicEngine = &graphicEngine;
    Sound::soundEngine = &soundEngine;

	menu = new Menu(&graphicEngine, &soundEngine);

	int i;
	for (i = 1; i < 10; i++) {
		stringstream ss;
		ss << "level" << i;
		lockedLevel.insert(make_pair(ss.str(), true));
	}

    lockedLevel.at("level1") = false;
    lockedLevel.at("level2") = false;

    //Init the menu
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
    const SDL_VideoInfo * videoInfo;

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
    videoFlags |= SDL_OPENGL;       // Store the palette in hardware
    videoFlags |= SDL_GL_DOUBLEBUFFER; // Enable double buffering
    videoFlags |= SDL_HWPALETTE;


    // This checks to see if surfaces can be stored in memory
    if (videoInfo->hw_available)
    	videoFlags |= SDL_HWSURFACE;
    else
    	videoFlags |= SDL_SWSURFACE;

    // This checks if hardware blits can be done
    if (videoInfo->blit_hw)
    	videoFlags |= SDL_HWACCEL;

    //get a SDL surface from screen
    //SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    graphicEngine.screen = SDL_SetVideoMode(10, 10, SCREEN_BPP, videoFlags);

	//Verify there is a surface
	if (!graphicEngine.screen) {
		cerr << "Video mode set failed: << " << SDL_GetError() << endl;
		quit(1);
	}

	//Init font usage
	if (TTF_Init() != 0) {
		cerr << "TTF_Init() Failed: " << TTF_GetError() << endl;
		quit(1);
	}

	//Init Audio
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		cerr << "Warning: Audio_Init() Failed: " << SDL_GetError() << endl;
		quit(1);
	}

    return 0;
}

void Game::pause() {
	//If we are not in pause
	if (gameState != GAME_PAUSE) {
		//Then save current game state
		previousGameState = gameState;
		//and pause
		gameState = GAME_PAUSE;
	} else {
		//else revert to previous game state
		gameState = previousGameState;
	}
}

//Start a new game (beginning with level 1)
void Game::newGame()
{
	delete menu;
	CurrentLevel = new Level1();
	CurrentLevel->graphicEngine = &graphicEngine;
	CurrentLevel->physicEngine = &physicEngine;
	CurrentLevel->soundEngine = &soundEngine;
	hero = new Hero();
    GameTimer = 0;
    Score = 0;

	CurrentLevel->loadLevel(hero);
	gameState = GAME_INGAME;
}

//Used to launch a specific level
//Should be used only in debug
void Game::launchLevel(string aLevelName)
{
	delete menu;

	int last_index = aLevelName.find_last_not_of("0123456789");
	int aLevelIndex = atoi(aLevelName.substr(last_index + 1).c_str());
	switch(aLevelIndex)
	{
		case 1:
			CurrentLevel = new Level1();
			break;

		case 2:
			CurrentLevel = new Level2();
			break;

		default:
			cerr << "Trying to load unknown level " << aLevelName << endl;
	}

	CurrentLevel->graphicEngine = &graphicEngine;
	CurrentLevel->physicEngine = &physicEngine;
	CurrentLevel->soundEngine = &soundEngine;
	if(hero)
		delete hero;

	hero = new Hero();
	GameTimer = 0;
	Score = 0;

	CurrentLevel->loadLevel(hero);
	gameState = GAME_INGAME;
}

//Launch the next level after a level has ended
void Game::launchNextLevel()
{
	unsigned int newLevel = 0;

	for (newLevel = 0; newLevel < levelOrder.size(); newLevel++) {
		if (CurrentLevel->name == levelOrder.at(newLevel))
			break;
	}

	if (newLevel >= 1) {   //levelOrder.size())
		cout << "Game won!!!!!!!!!!!\n";
		menu->currentMenu = MENU_SUCCESS;
		gameState = GAME_MENU;
		return;
	}

	CurrentLevel->graphicEngine = &graphicEngine;
	CurrentLevel->physicEngine = &physicEngine;
	CurrentLevel->soundEngine = &soundEngine;
	CurrentLevel->loadLevel(hero);
	gameState = GAME_INGAME;
}

void Game::stopMusic() {
	if (!soundEngine.musicMuted) {
		soundEngine.muteMusic();
	} else {
		soundEngine.musicMuted = false;
		soundEngine.playMusic();
	}
}

void Game::muteAll() {
	if (!soundEngine.mute) {
		soundEngine.muteAll();
	} else {
		soundEngine.mute = false;
		soundEngine.playMusic();
	}
}

//Update Timers
void Game::updateTimers() {
	ProgramTimer = ProgramTimer + Interval;

	if (gameState == GAME_INGAME)
		GameTimer = GameTimer + Interval;

	return;
}

//Control game speed
//If necessary, wait the required time until the next loop
void controlFPS() {
	if (NextLoop > SDL_GetTicks())
		SDL_Delay(NextLoop - SDL_GetTicks());

	NextLoop = SDL_GetTicks() + Interval;

	return;
}
