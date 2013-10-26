/*
 * Keyboard
 * Contains handling functions for keyboard events
 */
 
#include "keyb.h"

Keyboard::Keyboard()
{
	p_pressed = FALSE;
}

//Update the key states
void Keyboard::processKeyState()
{
	keyStates = SDL_GetKeyState(NULL);
}


void Keyboard::processKeyPress()
{
	if(keyStates[PAUSE_KEY])
	{
		if (!p_pressed)
		{
			game->pause();
			p_pressed = TRUE;
		}
	}
    return;
}

//Process in game keyboard events
void Keyboard::processeyInGame(Hero* hero)
{
	if(keyStates[SDLK_ESCAPE])
		game->gameState = MENU;

	//Cheat for those who find the game too hard
	if(keyStates[SDLK_KP_PLUS]||keyStates[SDLK_PLUS])
		hero->nbLife++;

	if (!hero->dontMove)
	{
		if(keyStates[UP_KEY])
			hero->moveUp();

		if(keyStates[DOWN_KEY])
			hero->moveDown();

		if(keyStates[RIGHT_KEY])
			hero->moveRight();

		if(keyStates[LEFT_KEY])
			hero->moveLeft();

		if(keyStates[FIRE_KEY])
			hero->fire();
	}

    return;
}

//Process in game key release
void Keyboard::handleKeyUpHero(SDL_keysym *keysym, Hero *hero)
{
    switch (keysym->sym)
	{
		case UP_KEY:
			hero->heroMovingUpOrDown = 0;
			break;
			
		case DOWN_KEY:
			hero->heroMovingUpOrDown = 0;
			break;

		case PAUSE_KEY:
			p_pressed = FALSE;
			break;

		default:
			break;
	}
    return;
}

//Process in menu keyboard events
void Keyboard::handleKeyPressMenu(SDL_keysym *keysym, Menu * menu)
{

	//If we are actually in transition from one menu to another: ignore all inputs
	if(menu->menuInTransition)
	{
		return;
	}

	//If we are entering a new name for the high score
	if(menu->currentMenu == MENU_NEWHIGHSCORE)
	{
		int validChar = FALSE;

		if(keysym->unicode == (Uint16)' ')
		{
			validChar = TRUE;
		}
		//input is a number
		else if((keysym->unicode >= (Uint16)'0') && (keysym->unicode <= (Uint16)'9'))
		{
			validChar = TRUE;
		}
		//input is a uppercase letter
		else if((keysym->unicode >= (Uint16)'A') && (keysym->unicode <= (Uint16)'Z'))
		{
			validChar = TRUE;
		}
		//input is a lowercase letter
		else if((keysym->unicode >= (Uint16)'a') && (keysym->unicode <= (Uint16)'z'))
		{
			validChar = TRUE;
		}

		if (validChar)
			menu->addChar((char)keysym->unicode);

		if((keysym->sym == SDLK_BACKSPACE))
		{
		 	menu->eraseChar();
		}

		if((keysym->sym == SDLK_RETURN))
		{
		 	menu->finishEnteringName();
		}

		return;
	}

	//If we are displaying one screen, then any touch take us to the next menu
	if(menu->currentMenu == MENU_SUCCESS || menu->currentMenu == MENU_CREDIT ||
			menu->currentMenu == MENU_GAMEOVER || menu->currentMenu == MENU_HIGHSCORE ||
			menu->currentMenu == MENU_INTRO)
	{
		menu->transition();
		return;
	}

	switch (keysym->sym)
	{
		case SDLK_ESCAPE:
			quit(0);
			break;

		case SDLK_UP:
			menu->selectUp();
			break;

		case SDLK_DOWN:
			menu->selectDown();
			break;

		case SDLK_SPACE:
		case SDLK_RETURN:
			menu->select();
			break;

		default:
			break;
	}

    return;
}

