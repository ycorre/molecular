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
		Music * currentMusic;
		vector<Sound *> playingSounds;
		map<int, Sound *> playingLayers;
		
		int soundVolume;
		int musicVolume;
		int numberOfChannel;
		bool pauseMusic, pauseSound;
		bool mute;
		bool soundMuted, musicMuted;

		SoundEngine();
		void init();
		void loadSoundFrom(Json::Value aConfig);
		void loadMusicFrom(Json::Value aConfig);
		void muteAll();
		void clearSounds();
		void stopAllSounds();
		void finishedPlaying(int aChannel);

		int loadSound(string pathToASound, Sound * aSound);
		void addSound(string pathToASound, string aSoundId);
		void addSound(Sound * aSound);
		void playSound(string aSound);
		void playSound(Sound * aSound);
		void stopSound(string aSound);
		void stopSound(Sound * aSound);
		void playSoundLoop(string aSound, int loop);
		void setSoundVolume(int soundVolume);
		void setVolumeFor(Sound * aSound);

		int loadMusic(string pathToASound, Music * aMusic);
		void addMusic(string pathTotASound, string aSoundId);
		void addMusic(Music * aMusic);
		void playMusic(string aMusic);
		void playMusic(Music * aMusic);
		void playMusic();
		void stopMusic(string aMusic);
		void stopMusic(Music * aMusic);
		void stopMusic();
		void fadeMusic(int timeMs);
		void fadeMusic(string aMusic, int ms);
		void muteMusic();
		void setMusicVolume(int musicVolume);
};

void channelDone(int channel);

#endif /* SOUNDENGINE_H */
