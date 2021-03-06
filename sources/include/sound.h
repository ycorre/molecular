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
#include <SDL/SDL.h>
#include <SDL/SDL_audio.h>
#include <SDL/SDL_mixer.h>
#include "common.h"
#include "soundEngine.h"

class SoundEngine;

class Sound
{
	public:
		string name;
		int playingChannel;
		bool isPlaying;
		int volume;
		bool looped;
		int numberOfLoops;
		int soundLayer;
		float posX;
		Mix_Chunk * soundData;

		static SoundEngine * soundEngine;
	    map<string, int> soundConfParameters;

		Sound();
		virtual ~Sound() {};
		Sound(string path, string name);
		Sound(Json::Value aConfig);
		virtual void load(string aPath);
		virtual void play();
		virtual void stop();
		void setPosition(float posX);
		void setLoop(int nTimes);
};

class Music: public Sound
{
	public:
		Mix_Music * musicData;

		Music();
		Music(string path, string name);
		Music(Json::Value aConfig);
		void load(string aPath);
		void play();
		void stop();
		void fadeOut(int aTime);
};

#endif /* SOUND_H */
