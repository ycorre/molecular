#ifndef COMMON_H
#define COMMON_H
#include <SDL.h>

using namespace std;

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

extern Uint32 programTimer;
extern Uint32 gameTimer;

#endif

