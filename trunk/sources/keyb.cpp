/*
 * Keyboard
 * Contains handling functions for keyboard events
 */
 
#include "keyb.h"

Keyboard::Keyboard()
{
	p_pressed = false;
	firing = false;
	keyStates = NULL;
	game = NULL;
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
			p_pressed = true;
		}
	}

	if(keyStates[MUTE_MUSIC_KEY])
	{
		if (!p_pressed)
		{
			game->stopMusic();
			p_pressed = true;
		}
	}

	if(keyStates[MUTE_ALL_KEY])
	{
		if (!p_pressed)
		{
			game->muteAll();
			p_pressed = true;
		}
	}
    return;
}

//Process in game keyboard events
void Keyboard::processKeyInGame(Hero * hero)
{
	if(keyStates[SDLK_ESCAPE])
	{
		CurrentLevel->cleanLevel();
		game->gameState = GAME_LOADMENU;
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

		if(keyStates[TELEPORT_KEY])
			hero->teleport();

		if(keyStates[SDLK_KP1])
			hero->upgradeWeaponTo(0);

		if(keyStates[SDLK_KP2])
			hero->upgradeWeaponTo(1);

		if(keyStates[SDLK_KP3])
			hero->upgradeWeaponTo(2);

		if(keyStates[PHOTON_KEY])
		{
			hero->setWeapon(HadronGun);
			hero->fireWeapon(HadronGun);
		}

		if(keyStates[ELECTRON_KEY])
		{
			hero->setWeapon(ElectronGun);
			hero->fireWeapon(ElectronGun);
		}

		if(keyStates[BARYON_KEY])
		{
			hero->setWeapon(BaryonGun);
			hero->fireWeapon(BaryonGun);
		}

		if(keyStates[PLASMA_KEY])
		{
			hero->setWeapon(PlasmaGun);
			hero->fireWeapon(PlasmaGun);
		}
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
			firing = true;
		}
		else if (!keyStates[ELECTRON_KEY])
		{
			if(firing)
			{
				firing = false;
				hero->stopFiring();
			}
			hero->isFiring = false;
			hero->currentWeapon->isFiring = false;
		}
	}

    return;
}

//Process in game key release
void Keyboard::handleKeyUp(SDL_keysym * keysym)
{
    switch (keysym->sym)
	{
		case SDLK_b:
		case MUTE_MUSIC_KEY:
		case MUTE_ALL_KEY:
		case PAUSE_KEY:
			p_pressed = false;
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
		int validChar = false;

		if(keysym->unicode == (Uint16)' ')
		{
			validChar = true;
		}
		//input is a number
		else if((keysym->unicode >= (Uint16)'0') && (keysym->unicode <= (Uint16)'9'))
		{
			validChar = true;
		}
		//input is a uppercase letter
		else if((keysym->unicode >= (Uint16)'A') && (keysym->unicode <= (Uint16)'Z'))
		{
			validChar = true;
		}
		//input is a lowercase letter
		else if((keysym->unicode >= (Uint16)'a') && (keysym->unicode <= (Uint16)'z'))
		{
			validChar = true;
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
		case SDLK_KP_ENTER:
			menu->select();
			break;

		default:
			break;
	}

    return;
}

