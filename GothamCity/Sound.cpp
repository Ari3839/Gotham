#include "Sound.h"
#include <iostream>
#include <bass.h>

bool Sound::audio_device = false;

Sound::Sound(const char* filename)
{
	if (!audio_device)
	{
		if (!BASS_Init(-1, 44100, 0, NULL, NULL))
		{
			printf("Error al cargar el archivo, no hay dispositivo de audio\n");
		}
		audio_device = true;
	}

	channel = BASS_StreamCreateFile(false, filename, 0, 0, BASS_SAMPLE_LOOP);

	if (!channel)
	{
		printf("No se puede reproducir el audio %s\n",filename);
	}
}

Sound::~Sound()
{
	BASS_Free();
}

void Sound::play()
{
	BASS_ChannelPlay(channel, false);
}

void Sound::pause()
{
	BASS_ChannelPause(channel);
}

void Sound::stop()
{
	BASS_ChannelStop(channel);
}