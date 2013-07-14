/*
 * soundEngine.h
 *
 *  Created on: 18 mai 2013
 *      Author: youenn
 */

#ifndef SOUNDENGINE_H
#define SOUNDENGINE_H

#include <iostream>
#include <string>
#include <utility>
#include <SDL.h>
#include <SDL_audio.h>
#include "common.h"

class SoundEngine
{
	public:
		void PlaySound(string file);
		void mixaudio(void *unused, Uint8 *stream, unsigned int len);
};

#endif /* SOUNDENGINE_H */
