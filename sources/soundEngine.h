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
#include "sound.h"

class Sound;
class Music;

class SoundEngine
{
	public:
		map<string, Sound *> sounds;
		map<string, Music *> musics;
		
		int soundVolume;
		int musicVolume;
		int numberOfChannel;

		SoundEngine();
		void init();
		void playSound(string aSound);
		void playSound(Sound * aSound);
		void stopSound(Sound * aSound);
		int loadSound(string pathToASound, Sound * aSound);
		int loadMusic(string pathToASound, Music * aMusic);
		void playSoundLoop(string aSound, int loop);
		void stopSound(string aSound);
		void playMusic(string aMusic);
		void playMusic(Music * aMusic);
		void stopMusic(string aMusic);
		void stopMusic(Music * aMusic);
		void addSound(string pathTotASound, string aSoundId);
		void addMusic(string pathTotASound, string aSoundId);
		void clearSounds();
		void stopAllSounds();
		//void mixaudio(void *unused, Uint8 *stream, unsigned int len);

};

void channelDone(int channel);

#endif /* SOUNDENGINE_H */
