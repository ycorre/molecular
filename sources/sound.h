/*
 * sound.h
 *
 *  Created on: 18 mai 2013
 *      Author: youenn
 */

#ifndef SOUND_H
#define SOUND_H

#include <iostream>
#include <string>
#include <utility>
#include <map>
#include <sstream>
#include <SDL.h>
#include <SDL_audio.h>
#include <SDL_mixer.h>
#include "common.h"
#include "soundEngine.h"

class SoundEngine;

class Sound
{
	public:
		string name;
		int playingChannel;
		int isPlaying;
		int volume;
		int looped;
		int numberOfLoops;
		Mix_Chunk * soundData;

		static SoundEngine * soundEngine;
	    map<string, int> soundConfParameters;

		Sound();
		Sound(string aName);
		Sound(string path, string name);
		Sound(Json::Value aConfig);
		virtual void load(string aPath);
		virtual void play();
		virtual void stop();
		void setLoop(int nTimes);
};

class Music: public Sound
{
	public:
		Mix_Music * musicData;

		Music();
		Music(string path, string name);
		Music(Json::Value aConfig);
		virtual void load(string aPath);
		virtual void play();
		virtual void stop();
		virtual void fadeOut(int aTime);
};

#endif /* SOUND_H */
