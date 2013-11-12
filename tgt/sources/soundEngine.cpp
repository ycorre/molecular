/*
 * soundEngine.cpp
 *
 *  Created on: 18 mai 2013
 *      Author: youenn
 */
#include "soundEngine.h"


void SoundEngine::playSound(string aSound)
{
	 Mix_PlayChannel(-1, sounds.at(aSound), 0);
}
	
void SoundEngine::addSound(string pathToASound, string aSoundId)
{
	sounds.insert(make_pair(aSoundId, Mix_LoadWAV(pathToASound.c_str())));
}

