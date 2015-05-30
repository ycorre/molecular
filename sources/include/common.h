#ifndef COMMON_H
#define COMMON_H
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#define GLM_FORCE_RADIANS
#include <GL/glew.h>

#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>

#include "../json/json/reader.h"
#include "../json/json/value.h"
#include "types.h"
#include "utility.h"

using namespace std;
class Game;
class Level;

// screen width, height, and bit depth
#define SCREEN_WIDTH  1200
#define SCREEN_HEIGHT 800
#define GAMEZONE_LIMIT 80
#define GAMEZONE_HEIGHT 720
#define SCREEN_BPP     32

//Do we use OpenGL rendering ?
//TODO check that the SDL one is not broken
#define USE_OPENGL 1

//Set up directions
#define RIGHT 0
#define LEFT 2
#define UP 1
#define DOWN 3

//Constant
#define PI 3.14159265

//Global variables (declared in game.cpp)
extern Uint32 ProgramTimer;
extern Uint32 GameTimer;
extern Uint32 Score;
extern GLFWwindow * Window;
extern Level * CurrentLevel;
extern Game * CurrentGame;

#endif

