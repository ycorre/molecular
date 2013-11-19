/*
 * soundEngine.cpp
 *
 *  Created on: 18 mai 2013
 *      Author: youenn
 */
#include "soundEngine.h"

SoundEngine::SoundEngine()
{
	numberOfChannel = 24;
	currentMusic = "";
	pauseMusic = pauseSound = mute = FALSE;
}

void SoundEngine::init()
{
    //Allocate more channels than the default parameter (8)
    Mix_AllocateChannels(numberOfChannel);
    Mix_Init(MIX_INIT_MP3);
	Mix_ChannelFinished(channelDone);
}

void SoundEngine::playSound(string aSound)
{
	sounds.at(aSound)->play();
}

void SoundEngine::stopSound(string aSound)
{
	sounds.at(aSound)->stop();
}

void SoundEngine::playSound(Sound * aSound)
{
	aSound->playingChannel = Mix_PlayChannel(-1, aSound->soundData, aSound->numberOfLoops);
	if (aSound->playingChannel!= -1)
	{
		aSound->isPlaying = TRUE;
	}
	else
	{
		cerr << "Error could not play sound" << aSound->name << endl;
	}
}

void SoundEngine::stopSound(Sound * aSound)
{
	Mix_HaltChannel(aSound->playingChannel);

	if (aSound->playingChannel!= -1)
	{
		aSound->isPlaying = FALSE;
	}
	else
	{
		aSound->isPlaying = FALSE;
		cerr << "Error could not play sound" << aSound->name << endl;
	}
}

void SoundEngine::addSound(string pathToASound, string aSoundId)
{
	//Check if the sound is already loaded
	if(sounds.find(aSoundId) == sounds.end())
	{
		Sound * aSound = new Sound(pathToASound, aSoundId);

		sounds.insert(make_pair(aSoundId, aSound));
	}
	else
	{
		cerr << "SoundEngine Error: The sound " << aSoundId << " has already been loaded " << endl;
	}
}

int SoundEngine::loadSound(string pathToASound, Sound * aSound)
{
	aSound->soundData = Mix_LoadWAV(pathToASound.c_str());

	if(aSound->soundData == NULL)
	{
		cerr << "SoundEngine Error: Could not load " << pathToASound << endl;
		return 0;
	}

	return 1;
}


void SoundEngine::clearSounds()
{
	sounds.clear();
}

void SoundEngine::stopAllSounds()
{
	Mix_HaltChannel(-1);
}

void channelDone(int channel)
{
  // cout<< "channel " << channel << " finished playback" << endl;
}



/*
 * Music functions
 */

void SoundEngine::playMusic(string aMusic)
{
	musics.at(aMusic)->play();
}

void SoundEngine::stopMusic(string aMusic)
{
	musics.at(aMusic)->stop();
}

void SoundEngine::playMusic(Music * aMusic)
{
	//currentMusic = aMusic->name;
	aMusic->playingChannel = Mix_PlayMusic(aMusic->musicData, aMusic->numberOfLoops);
	if (aMusic->playingChannel!= -1)
	{
		aMusic->isPlaying = TRUE;
	}
	else
	{
		cerr << "Error could not play sound" << aMusic->name << endl;
	}
}

void SoundEngine::playMusic()
{
	playMusic(currentMusic);
}

void SoundEngine::stopMusic(Music * aMusic)
{
	Mix_HaltMusic();
	aMusic->isPlaying = FALSE;
}

void SoundEngine::stopMusic()
{
	Mix_HaltMusic();
}

void SoundEngine::addMusic(string pathToASound, string aMusicId)
{
	//Check if the sound is already loaded
	if(musics.find(aMusicId) == musics.end())
	{
		Music * aMusic = new Music(pathToASound, aMusicId);

		musics.insert(make_pair(aMusicId, aMusic));
	}
	else
	{
		cerr << "SoundEngine Error: The sound " << aMusicId << " has already been loaded " << endl;
	}
}

int SoundEngine::loadMusic(string pathToASound, Music * aMusic)
{
	aMusic->musicData = Mix_LoadMUS(pathToASound.c_str());

	if(aMusic->musicData == NULL)
	{
		cerr << "SoundEngine Error: Could not load " << pathToASound << endl;
		return 0;
	}

	return 1;
}
