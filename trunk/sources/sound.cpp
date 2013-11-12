/*
 * sound.cpp
 *
 *  Created on: 18 mai 2013
 *      Author: youenn
 */
#include "sound.h"

SoundEngine * Sound::soundEngine;

Sound::Sound()
{
	playingChannel = -1;
	isPlaying = FALSE;
	volume = 0;
	looped = FALSE;
	numberOfLoops = 0;
	soundData = NULL;
}

Sound::Sound(string aName)
{
	name = aName;
	playingChannel = -1;
	isPlaying = FALSE;
	volume = 0;
	looped = FALSE;
	numberOfLoops = 0;
	soundData = NULL;
}

Sound::Sound(string aPath, string aName)
{
	name = aName;
	soundData = NULL;
	this->load(aPath);
	playingChannel = -1;
	isPlaying = FALSE;
	volume = 0;
	looped = FALSE;
	numberOfLoops = 0;
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
		looped = TRUE;
	}
	else
	{
		looped = FALSE;
	}
}

Music::Music()
{
	playingChannel = -1;
	isPlaying = FALSE;
	volume = 0;
	looped = TRUE;
	numberOfLoops = -1;
	soundData = NULL;
	musicData = NULL;
}

Music::Music(string aPath, string aName)
{
	name = aName;
	soundData = NULL;
	this->load(aPath);
	playingChannel = -1;
	isPlaying = FALSE;
	volume = 0;
	looped = FALSE;
	numberOfLoops = 0;
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
}
