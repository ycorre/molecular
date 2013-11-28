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
	volume = 100;
	looped = FALSE;
	numberOfLoops = 0;
	soundData = NULL;

	soundConfParameters["name"] = sName;
	soundConfParameters["volume"] = sVolume;
	soundConfParameters["loop"] = sLoop;
	soundConfParameters["sound"] = sData;
}

Sound::Sound(string aName)
{
	name = aName;
	playingChannel = -1;
	isPlaying = FALSE;
	volume = 100;
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
	volume = 100;
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

void Sound::loadASound(string confString)
{
	string token;
	istringstream aConf(confString);

	while(getline(aConf, token, ';'))
	{
		istringstream aParam(token);
		string paramType;
		string paramValue;
		getline(aParam, paramType, ':');
		getline(aParam, paramValue, ':');

		switch(soundConfParameters.at(paramType))
		{
			case sName:
				name = paramValue;
				break;

			case sVolume:
				volume = atoi(paramValue.c_str());
				if(soundData != NULL)
					soundEngine->setVolumeFor(this);
				break;

			case sLoop:
				setLoop(atoi(paramValue.c_str()));
				break;

			case sData:
				load(paramValue.c_str());
				soundEngine->setVolumeFor(this);
				break;

			default:
				cerr << "Sound loadFrom(): Unknown configuration parameter: " << paramType << endl;
				break;
		}
	}
}

/*
 * Music function
 */

Music::Music()
{
	playingChannel = -1;
	isPlaying = FALSE;
	volume = 100;
	looped = TRUE;
	numberOfLoops = -1;
	soundData = NULL;
	musicData = NULL;
}

Music::Music(string aPath, string aName)
{
	name = aName;
	soundData = NULL;
	musicData = NULL;
	this->load(aPath);
	playingChannel = -1;
	isPlaying = FALSE;
	volume = 100;
	looped = TRUE;
	numberOfLoops = -1;
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
	isPlaying = FALSE;
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
	isPlaying = FALSE;
}
