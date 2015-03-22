/*
 * sound.cpp
 *
 *  Created on: 18 mai 2013
 *      Author: youenn
 */
#include "include/sound.h"

SoundEngine * Sound::soundEngine;

Sound::Sound()
{
	playingChannel = -1;
	isPlaying = false;
	volume = 100;
	looped = false;
	numberOfLoops = 0;
	soundLayer = 0;
	soundData = NULL;
	posX = -1;
}

Sound::Sound(string aPath, string aName)
{
	name = aName;
	soundData = NULL;
	load(aPath);
	playingChannel = -1;
	isPlaying = false;
	volume = 100;
	looped = false;
	soundLayer = 0;
	numberOfLoops = 0;
	posX = -1;
}

Sound::Sound(Json::Value aConfig)
{
	soundData = NULL;
	playingChannel = -1;
	isPlaying = false;
	name = aConfig.get("name", "soundDefaut").asString();
	setLoop(aConfig.get("loop", 0).asInt());
	volume = aConfig.get("volume", 100).asInt();
	soundLayer = aConfig.get("soundLayer", 0).asInt();
	load(aConfig.get("dataPath", "").asString());
	soundEngine->setVolumeFor(this);
	posX = -1;
}

void Sound::load(string aPath)
{
	soundEngine->loadSound(aPath, this);
}

void Sound::play()
{
	if(looped)
	{
		if (!isPlaying)
			soundEngine->playSound(this);
	}
	else
	{
		soundEngine->playSound(this);
	}
}

void Sound::stop()
{
	if(looped)
	{
		if(isPlaying)
			soundEngine->stopSound(this);
	}
	else
	{
		soundEngine->stopSound(this);
	}
}

//nTimes = -1 means infinite loop
void Sound::setLoop(int nTimes)
{
	numberOfLoops = nTimes;
	if(numberOfLoops != 0)
	{
		looped = true;
	}
	else
	{
		looped = false;
	}
}

void Sound::setPosition(float aPosX)
{
	posX = aPosX;
	posX = max(0.0f, posX);
	posX = min((float)SCREEN_WIDTH, posX);
}

/*
 * Music functions
 */
Music::Music()
{
	playingChannel = -1;
	isPlaying = false;
	volume = 100;
	looped = true;
	numberOfLoops = -1;
	soundData = NULL;
	musicData = NULL;
}

Music::Music(string aPath, string aName)
{
	name = aName;
	soundData = NULL;
	musicData = NULL;
	load(aPath);
	playingChannel = -1;
	isPlaying = false;
	volume = 100;
	looped = true;
	numberOfLoops = -1;
}

Music::Music(Json::Value aConfig)
{
	soundData = NULL;
	musicData = NULL;
	playingChannel = -1;
	name = aConfig.get("name", "musicDefaut").asString();
	setLoop(aConfig.get("loop", 0).asInt());
	volume = aConfig.get("volume", 100).asInt();
	load(aConfig.get("dataPath", "").asString());
	isPlaying = false;

	soundEngine->setMusicVolume(volume);
}

void Music::load(string aPath)
{
	soundEngine->loadMusic(aPath, this);
}

void Music::play()
{
	if(looped)
	{
		if (!isPlaying)
			soundEngine->playMusic(this);
	}
	else
	{
		soundEngine->playMusic(this);
	}
}

void Music::stop()
{
	if(looped)
	{
		if(isPlaying)
			soundEngine->stopMusic(this);
	}
	else
	{
		soundEngine->stopMusic(this);
	}
	isPlaying = false;
}

void Music::fadeOut(int aTime)
{
	if(looped)
	{
		if(isPlaying)
			soundEngine->fadeMusic(aTime);
	}
	else
	{
		soundEngine->fadeMusic(aTime);
	}
	isPlaying = false;
}
