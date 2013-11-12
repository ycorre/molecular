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
#include <map>
#include <SDL.h>
#include <SDL_audio.h>
#include <SDL_mixer.h>
#include "common.h"

class SoundEngine
{
	public:
		map<string, Mix_Chunk*> sounds;
		
		void playSound(string aSound);
		void addSound(string pathTotASound, string aSoundId);
		//void mixaudio(void *unused, Uint8 *stream, unsigned int len);
};

#endif /* SOUNDENGINE_H */
