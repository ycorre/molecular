/*
 * Main class of the game
 *
 * Part of this code was created by Jeff Molofee '99
 * (ported to Linux/SDL by Ti Leggett '01)
 *
 * If you've found this code useful, please let me know.
 *
 * Visit Jeff at http://nehe.gamedev.net/
 *
 * or for port-specific comments, questions, bugreports etc.
 * email to leggett@eecs.tulane.edu
 */

#include "game.h"

//Timer useful when pausing the game and eventually for timing the player
//Used as global variables and declared in common.h
Uint32 programTimer;
Uint32 gameTimer;

Game::Game()
{
	initSDL();
	initGame();
}

// Function to release/destroy our resources and restoring the old desktop
void Quit(int returnCode)
{
    //Close the SDL session
    SDL_Quit();

    exit(returnCode);
}


int main(int argc, char **argv)
{
    Game * game = new Game();

    //set the random generator seed
    srand(time(NULL));

    //Launch the game
    game->mainLoop();
}

int Game::mainLoop()
{
	int done = FALSE;
	SDL_Event event;

    gameState = LOGO;
   // Drawable::lev = currentLevel;

    menu->introLength = 2500;
    menu->loadIntro();
    menu->loadMenu();
    menu->game = this;

    //Main loop
    while (!done)
	{
	    // Clear the elements to be displayed at the beginning of the loop
    	graphicEngine.toDisplay.clear();
	    while (SDL_PollEvent(&event))
		{
		    switch(event.type)
			{
			case SDL_KEYDOWN:
			    // handle key presses
				// handleKeyPressHero(&event.key.keysym, &(game->hero));
		    	if(gameState == MENU)
		    	{
		    		keyboard->handleKeyPressMenu(&event.key.keysym, menu);
		    	}
			    break;
			    
			case SDL_KEYUP:
			    // handle key presses
				if(gameState == INGAME||gameState == PAUSE)
				{
					keyboard->handleKeyUpHero(&event.key.keysym, currentLevel->hero);
				}
			    break;
			case SDL_QUIT:
			    // handle quit requests
			    done = TRUE;
			    break;
			default:
			    break;
			}
		}

	    keyboard->processKeyState();
	    keyboard->processKeyPress();

    	if(gameState == LOGO)
    	{
    		menu->playIntro();
   		 	gameState = MENU;
    	}

    	if(gameState == MENU)
    	{
    		menu->displayMenu();
    	}

	    if(gameState == INGAME)
	    {
	    	keyboard->processKeyPressHero(currentLevel->hero);
	    	currentLevel->drawLevel();
	    }

		graphicEngine.drawFrame();
		
		SDL_Delay(10);
	}

    //Clean ourselves up and exit
    Quit(0);

    //Should never get here
    return(0);
}


int Game::initGame()
{
	graphicEngine.addFont("res/Arial.ttf");
	graphicEngine.initColors();
	keyboard = new Keyboard();
	keyboard->game = this;
	physicEngine = *(new Pe());
	soundEngine = *(new SoundEngine());
	menu = new Menu(&graphicEngine, &soundEngine);

    Level1 * l1 = new Level1();
    Level2 * l2 = new Level2();

    levels.insert(make_pair("level1", l1));
    levels.insert(make_pair("level2", l2));

	return 1;
}

int Game::initSDL()
{
	// Flags to pass to SDL_SetVideoMode
    int videoFlags;
    //This holds some info about our display
    const SDL_VideoInfo *videoInfo;
	
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0)
	{
	    fprintf(stderr, "Video initialization failed: %s\n", SDL_GetError());
	    Quit(1);
	}

    // Fetch the video info
    videoInfo = SDL_GetVideoInfo();

    if (!videoInfo)
	{
	    fprintf(stderr, "Video query failed: %s\n", SDL_GetError());
	    Quit(1);
	}

    //The flags to pass to SDL_SetVideoMode
    videoFlags = SDL_DOUBLEBUF; 	// Enable double buffering
    videoFlags |= SDL_HWPALETTE;       // Store the palette in hardware
    videoFlags |= SDL_RESIZABLE;       // Enable window resizing

    // This checks to see if surfaces can be stored in memory
    if (videoInfo->hw_available)
	videoFlags |= SDL_HWSURFACE;
    else
	videoFlags |= SDL_SWSURFACE;

    // This checks if hardware blits can be done
    if (videoInfo->blit_hw)
	videoFlags |= SDL_HWACCEL;

    // get a SDL surface from screen
    graphicEngine.screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
				videoFlags);

    // Verify there is a surface
    if (!graphicEngine.screen)
	{
	    fprintf(stderr,  "Video mode set failed: %s\n", SDL_GetError());
	    Quit(1);
	}
    
    //Init font usage
    if (TTF_Init() != 0)
    {
    	cerr << "TTF_Init() Failed: " << TTF_GetError() << endl;
    	SDL_Quit();
    	exit(1);
    }

    // Load a font
     TTF_Font *font;
     font = TTF_OpenFont("res/Arial.ttf", 24);
     if (font == NULL)
     {
        cerr << "TTF_OpenFont() Failed: " << TTF_GetError() << endl;
        TTF_Quit();
        SDL_Quit();
        exit(1);
     }

    //Enable repetition of keyboard events
    SDL_EnableKeyRepeat(1, 250);//SDL_DEFAULT_REPEAT_INTERVAL);
    
    Drawable::ge = &graphicEngine;

    return 0;
}

void Game::pause()
{
	if (gameState != PAUSE)
	{
		previousGameState = gameState;
		gameState = PAUSE;
	}
	else
	{
		gameState = previousGameState;
	}
}

void Game::launchLevel(string aLevel)
{
	currentLevel = levels.at(aLevel);
	currentLevel->ge = &graphicEngine;
	currentLevel->pe = &physicEngine;
	Drawable::lev = currentLevel;

	currentLevel->loadLevel();
	gameState = INGAME;
}


