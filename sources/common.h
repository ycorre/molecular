#ifndef COMMON_H
#define COMMON_H
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_opengl.h>
#include "json/json/reader.h"
#include "json/json/value.h"
//#include <minmax.h>

using namespace std;
//class Game;
class Level;

//Do we use OpenGL rendering ?
//TODO check that the SDL one is not broken
#define USE_OPENGL 1

//Set up boolean values
#define TRUE  1
#define FALSE 0

//Set up directions
#define RIGHT 0
#define LEFT 2
#define UP 1
#define DOWN 3

//Debug trace
#ifdef DEBUG
    #define printd(x) printf x
#else
    #define printd(x)
#endif

//Constant
#define PI 3.14159265

//Global variables (declared in game.cpp)
extern Uint32 ProgramTimer;
extern Uint32 GameTimer;
extern Uint32 Score;
extern Level * CurrentLevel;
//Game * CurrentGame;

#endif

