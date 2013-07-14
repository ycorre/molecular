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

// Function to handle key press events during active game phase
void Keyboard::handleKeyPress(SDL_keysym *keysym)
{
    switch (keysym->sym)
	{
		case SDLK_ESCAPE:
			/* ESC key was pressed */
			Quit(0);
			break;
		case SDLK_UP:

			break;
		case SDLK_DOWN:

			break;
		case SDLK_RIGHT:
			break;
		case SDLK_LEFT:

			break;
		case SDLK_SPACE:

			break;
		case SDLK_F1:

			break;
		default:
			break;
	}
    return;
}

void Keyboard::processKeyPress()
{
	if(keyStates[SDLK_p])
		{
		if (!p_pressed)
			{
				game->pause();
				p_pressed = TRUE;
			}
		}
    return;
}

void Keyboard::processKeyPressHero(Hero* hero)
{
	if(keyStates[SDLK_ESCAPE])
		Quit(0);

	if(keyStates[SDLK_o])
		{game->launchLevel("level1");

		cout<<"1 pressed\n";
		}

	if(keyStates[SDLK_i])
		game->launchLevel("level2");

	if (!hero->dontMove)
	{
		if(keyStates[SDLK_UP])
			hero->moveUp();

		if(keyStates[SDLK_DOWN])
			hero->moveDown();

		if(keyStates[SDLK_RIGHT])
			hero->moveRight();

		if(keyStates[SDLK_LEFT])
			hero->moveLeft();

		if(keyStates[SDLK_SPACE])
			hero->fire();
	}

	if(keyStates[SDLK_F1])
		{}
    return;
}

void Keyboard::handleKeyUpHero(SDL_keysym *keysym, Hero *hero)
{
    switch (keysym->sym)
	{
		case SDLK_UP:
			hero->heroMovingUpOrDown = 0;
			break;
			
		case SDLK_DOWN:
			hero->heroMovingUpOrDown = 0;
			break;
			
		case SDLK_p:
			p_pressed = FALSE;
			break;

		default:
			break;
	}
    return;
}

void Keyboard::handleKeyPressMenu(SDL_keysym *keysym, Menu * menu)
{
	switch (keysym->sym)
	{
		case SDLK_ESCAPE:
			Quit(0);
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


void Keyboard::processKeyPressMenu(Menu * menu)
{
	if(keyStates[SDLK_ESCAPE])
		Quit(0);

	if(keyStates[SDLK_UP])
		menu->selectUp();

	if(keyStates[SDLK_DOWN])
		menu->selectDown();

	if(keyStates[SDLK_SPACE] || keyStates[SDLK_RETURN])
		menu->select();

    return;
}

