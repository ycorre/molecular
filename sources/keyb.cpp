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

	if(keyStates[STOP_MUSIC_KEY])
	{
		if (!p_pressed)
		{
			game->stopMusic();
			p_pressed = TRUE;
		}
	}
    return;
}

//Process in game keyboard events
void Keyboard::processKeyInGame(Hero* hero)
{
	if(keyStates[SDLK_ESCAPE])
	{
		game->currentLevel->cleanLevel();
		game->gameState = MENU;
	}

	if(keyStates[SDLK_b])
	{
		if (!p_pressed)
		{
			game->currentLevel->displayBackGround = (game->currentLevel->displayBackGround + 1) % 2;
			p_pressed = TRUE;
		}
	}

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
			hero->teleport();
	}

    return;
}


void Keyboard::processeMouseInGame(Hero * hero)
{
	int x, y, buttonMask;
	buttonMask = SDL_GetRelativeMouseState(&x, &y);

	if (!hero->dontMove)
	{
		hero->move(x, y);

		if(buttonMask & SDL_BUTTON(1))
		{
			hero->fire();
		}
		else if (!keyStates[FIRE_KEY])
		{
			hero->isFiring = FALSE;
		}
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

		case FIRE_KEY:
			hero->isFiring = FALSE;
			break;

		case SDLK_b:
		case STOP_MUSIC_KEY:
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
	//If we are currently in transition from one menu to another: ignore all inputs
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
			menu->currentMenu == MENU_INTRO || (menu->currentMenu == MENU_LEVELSELECT && keysym->sym == SDLK_ESCAPE))
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
			menu->selectionMove(UP);
			break;

		case SDLK_DOWN:
			menu->selectionMove(DOWN);
			break;

		case SDLK_RIGHT:
			menu->selectionMove(RIGHT);
			break;

		case SDLK_LEFT:
			menu->selectionMove(LEFT);
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

