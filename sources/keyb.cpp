/*
 * Keyboard
 * Contains handling functions for keyboard events
 */

#include "include/keyb.h"

Keyboard::Keyboard() {
	p_pressed = false;
	firing = false;
	game = NULL;
}

void Keyboard::processKeyState(GLFWwindow* window, int key, int scancode,
		int action, int mods) {
	switch (action) {
	case GLFW_PRESS:
		//Handle key presses
		if (CurrentGame->gameState == GAME_MENU) {
			handleKeyPressMenu(key, CurrentGame->menu);
		}
		break;

	case GLFW_RELEASE:
		//Handle key release
		handleKeyUp(key);
		break;

	default:
		break;
	}

	processKeyPress();
}

void Keyboard::processKeyPress() {
	if (glfwGetKey(Window, PAUSE_KEY) == GLFW_PRESS) {
		if (!p_pressed) {
			game->pause();
			p_pressed = true;
		}
	}

	if (glfwGetKey(Window, MUTE_MUSIC_KEY) == GLFW_PRESS) {
		if (!p_pressed) {
			game->stopMusic();
			p_pressed = true;
		}
	}

	if (glfwGetKey(Window, MUTE_ALL_KEY) == GLFW_PRESS) {
		if (!p_pressed) {
			game->muteAll();
			p_pressed = true;
		}
	}
	return;
}

/**
 * Process in game keyboard events
 */
void Keyboard::processKeyInGame(Hero * hero) {
	if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		CurrentLevel->cleanLevel();
		game->gameState = GAME_LOADMENU;
	}

	//Cheat for those who find the game too hard
	if (glfwGetKey(Window, GLFW_KEY_KP_ADD) == GLFW_PRESS
			|| glfwGetKey(Window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
		hero->nbLife++;

	if (!hero->dontMove) {
		if (glfwGetKey(Window, UP_KEY) == GLFW_PRESS)
			hero->moveUp();

		if (glfwGetKey(Window, DOWN_KEY) == GLFW_PRESS)
			hero->moveDown();

		if (glfwGetKey(Window, RIGHT_KEY) == GLFW_PRESS)
			hero->moveRight();

		if (glfwGetKey(Window, LEFT_KEY) == GLFW_PRESS)
			hero->moveLeft();

		if (glfwGetKey(Window, TELEPORT_KEY) == GLFW_PRESS)
			hero->teleport();

		if (glfwGetKey(Window, GLFW_KEY_KP_1) == GLFW_PRESS)
			hero->upgradeWeaponTo(0);

		if (glfwGetKey(Window, GLFW_KEY_KP_2) == GLFW_PRESS)
			hero->upgradeWeaponTo(1);

		if (glfwGetKey(Window, GLFW_KEY_KP_3) == GLFW_PRESS)
			hero->upgradeWeaponTo(2);

		if (glfwGetKey(Window, PHOTON_KEY) == GLFW_PRESS) {
			hero->setWeapon(HadronGun);
			hero->fireWeapon(HadronGun);
		}

		if (glfwGetKey(Window, ELECTRON_KEY) == GLFW_PRESS) {
			hero->setWeapon(ElectronGun);
			hero->fireWeapon(ElectronGun);
		}

		if (glfwGetKey(Window, BARYON_KEY) == GLFW_PRESS) {
			hero->setWeapon(BaryonGun);
			hero->fireWeapon(BaryonGun);
		}

		if (glfwGetKey(Window, PLASMA_KEY) == GLFW_PRESS) {
			hero->setWeapon(PlasmaGun);
			hero->fireWeapon(PlasmaGun);
		}
	}

	return;
}

void Keyboard::processeMouseInGame(Hero * hero) {
	double xpos, ypos;
	glfwGetCursorPos(Window, &xpos, &ypos);

	if (!hero->dontMove) {
		hero->move(xpos, -ypos);

		if (glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			hero->fire();
			firing = true;
		} else if (!glfwGetKey(Window, ELECTRON_KEY) == GLFW_PRESS) {
			if (firing) {
				firing = false;
				hero->stopFiring();
			}
			hero->isFiring = false;
			hero->currentWeapon->isFiring = false;
		}
	}

	// Reset to 0 in order to have relative offset
	glfwSetCursorPos(Window, 0.0, 0.0);
	return;
}

/**
 * Process in game key release
 */
void Keyboard::handleKeyUp(int keysym) {
	switch (keysym) {
	case GLFW_KEY_B:
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

/**
 * Process in menu keyboard events
 */
void Keyboard::handleKeyPressMenu(int key, Menu * menu) {
	//If we are currently in transition from one menu to another: ignore all inputs
	if (menu->menuInTransition) {
		return;
	}

	//If we are entering a new name for the high score
	if (menu->currentMenu == MENU_NEWHIGHSCORE) {
		int validChar = false;

		if (key == GLFW_KEY_SPACE) {
			validChar = true;
		}
		//input is a number
		else if ((key >= GLFW_KEY_0) && (key <= GLFW_KEY_9)) {
			validChar = true;
		}
		//input is a uppercase letter
		else if ((key >= GLFW_KEY_A) && (key <= GLFW_KEY_Z)) {
			validChar = true;
		}

		if (validChar)
			menu->addChar((char) key);

		if ((key == GLFW_KEY_BACKSPACE)) {
			menu->eraseChar();
		}

		if ((key == GLFW_KEY_ENTER)) {
			menu->finishEnteringName();
		}

		return;
	}

	//If we are displaying one screen, then any touch take us to the next menu
	if (menu->currentMenu == MENU_SUCCESS || menu->currentMenu == MENU_CREDIT
			|| menu->currentMenu == MENU_GAMEOVER
			|| menu->currentMenu == MENU_HIGHSCORE
			|| menu->currentMenu == MENU_INTRO
			|| ((menu->currentMenu == MENU_OPTIONS
					|| menu->currentMenu == MENU_LEVELSELECT)
					&& key == GLFW_KEY_ESCAPE)) {
		menu->transition();
		return;
	}

	switch (key) {
	case GLFW_KEY_ESCAPE:
		game->done = true;
		break;

	case GLFW_KEY_UP:
		menu->selectionMove(UP);
		break;

	case GLFW_KEY_DOWN:
		menu->selectionMove(DOWN);
		break;

	case GLFW_KEY_RIGHT:
		menu->selectionMove(RIGHT);
		break;

	case GLFW_KEY_LEFT:
		menu->selectionMove(LEFT);
		break;

	case GLFW_KEY_SPACE:
	case GLFW_KEY_ENTER:
	case GLFW_KEY_KP_ENTER:
		menu->select();
		break;

	default:
		break;
	}

	return;
}
