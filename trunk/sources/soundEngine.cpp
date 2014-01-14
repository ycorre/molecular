/*
 * soundEngine.cpp
 *
 *  Created on: 18 mai 2013
 *      Author: youenn
 */
#include "soundEngine.h"

static SoundEngine * aSe;

SoundEngine::SoundEngine()
{
	numberOfChannel = 24;
	currentMusic = NULL;
	pauseMusic = pauseSound = mute = musicMuted = soundMuted = false;
	soundVolume = musicVolume = 100;
}

void SoundEngine::init()
{
	int i;

    //Allocate more channels than the default parameter (8)
    Mix_AllocateChannels(numberOfChannel);
    Mix_Init(MIX_INIT_MP3);
    Mix_Init(MIX_INIT_OGG|MIX_INIT_MOD);

	Mix_ChannelFinished(channelDone);
	playingSounds.resize(numberOfChannel);

	//Init the layers (16 layers for now)
	for (i= 0; i < 16; i++)
	{
		playingLayers[i] = NULL;
	}

	aSe = this;
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
	if((!mute) && (!soundMuted))
	{
		//if the sound layer !=0 then check the sound layer
		if(aSound->soundLayer)
		{
			//If a sound of this layer is already playing
			if(playingLayers.at(aSound->soundLayer))
			{
				//Interrupt the current sound
				playingLayers.at(aSound->soundLayer)->stop();
				playingLayers.at(aSound->soundLayer) = aSound;
			}
			else
			{
				playingLayers.at(aSound->soundLayer) = aSound;
			}
		}

		aSound->playingChannel = Mix_PlayChannel(-1, aSound->soundData, aSound->numberOfLoops);
		if (aSound->playingChannel > -1)
		{
			aSound->isPlaying = true;
			playingSounds.at(aSound->playingChannel) = aSound;
		}
		else
		{
			playingLayers.at(aSound->soundLayer) = NULL;
			cerr << "Error could not play sound" << aSound->name << endl;
		}
	}
}

void SoundEngine::stopSound(Sound * aSound)
{
	if((!mute) && (!soundMuted))
	{
		Mix_HaltChannel(aSound->playingChannel);

		if (aSound->playingChannel!= -1)
		{
			aSound->isPlaying = false;
		}
		else
		{
			aSound->isPlaying = false;
			cerr << "Error could not play sound" << aSound->name << endl;
		}
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

void SoundEngine::addSound(Sound * aSound)
{
	//Check if the sound is already loaded
	if(sounds.find(aSound->name) == sounds.end())
	{
		sounds.insert(make_pair(aSound->name, aSound));
	}
	else
	{
		Mix_FreeChunk(aSound->soundData);
		cerr << "SoundEngine Error: The sound " << aSound->name << " has already been loaded " << endl;
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

void SoundEngine::setVolumeFor(Sound * aSound)
{
	//normalize value on 128 (which correspond to the max volume (MIX_MAX_VOLUME))
	int aVolume = (int) (aSound->volume * 1.28);
	Mix_VolumeChunk(aSound->soundData, aVolume);
}

void SoundEngine::clearSounds()
{
	sounds.clear();
}

void SoundEngine::stopAllSounds()
{
	Mix_HaltChannel(-1);
}

void SoundEngine::loadSoundFrom(Json::Value aConfig)
{
	Sound * aSound = new Sound(aConfig);
	addSound(aSound);
}

//Call when a channel has finished playing a sound
void channelDone(int channel)
{
	aSe->finishedPlaying(channel);
}


void SoundEngine::finishedPlaying(int aChannel)
{
	//Set the playing flag back to false
	playingSounds.at(aChannel)->isPlaying = false;

	//Free the sound layer
	playingLayers.at(playingSounds.at(aChannel)->soundLayer) = NULL;
}

//Set the volume for all channel
void SoundEngine::setSoundVolume(int aSoundVolume)
{
	//Sound volume goes up to 128 but we use value up to 100
	soundVolume = (int)(aSoundVolume * 1.28);
	Mix_Volume(-1, soundVolume);
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
	currentMusic = aMusic;
	if(!mute && !musicMuted)
	{
		Mix_SetMusicPosition(0.0);

		aMusic->playingChannel = Mix_PlayMusic(aMusic->musicData, aMusic->numberOfLoops);
		if (aMusic->playingChannel!= -1)
		{
			aMusic->isPlaying = true;
		}
		else
		{
			cerr << "Error could not play music" << aMusic->name << endl;
		}
	}
}

void SoundEngine::playMusic()
{
	playMusic(currentMusic);
}

void SoundEngine::stopMusic(Music * aMusic)
{
	Mix_HaltMusic();
	Mix_RewindMusic();
	aMusic->isPlaying = false;
}

void SoundEngine::stopMusic()
{
	Mix_HaltMusic();
	Mix_RewindMusic();
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

void SoundEngine::addMusic(Music * aMusic)
{
	//Check if the music is already loaded
	if(musics.find(aMusic->name) == musics.end())
	{
		musics.insert(make_pair(aMusic->name, aMusic));
	}
	else
	{
		Mix_FreeChunk(aMusic->soundData);
		cerr << "SoundEngine Error: The music " << aMusic->name << " has already been loaded " << endl;
	}
}

void SoundEngine::fadeMusic(string aMusic, int ms)
{
	musics.at(aMusic)->fadeOut(ms);
}

void SoundEngine::fadeMusic(int ms)
{
	//Check if the sound is already loaded
	Mix_FadeOutMusic(ms);
}

void SoundEngine::muteMusic()
{
	musicMuted = true;
	stopMusic();
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

void SoundEngine::setMusicVolume(int aMusicVolume)
{
	musicVolume = (int)(aMusicVolume * 1.28);
	Mix_VolumeMusic(musicVolume);
}

void SoundEngine::muteAll()
{
	mute = true;
	stopMusic();
	stopAllSounds();
}

void SoundEngine::loadMusicFrom(Json::Value aConfig)
{
	Music * aMusic = new Music(aConfig);
	addMusic(aMusic);
}
