#include "SoundSorce.h"
#include <iostream>

SoundSorce::SoundSorce()
{
	alGenSources(1, &p_Source);
	alSourcef(p_Source, AL_PITCH, p_Pitch);
	alSourcef(p_Source, AL_GAIN, p_Gain);
	alSource3f(p_Source, AL_POSITION, p_Position[0], p_Position[1], p_Position[2]);
	alSource3f(p_Source, AL_VELOCITY, p_Velocity[0], p_Velocity[1], p_Velocity[2]);
	alSourcei(p_Source, AL_LOOPING, p_LoopSound);
	alSourcei(p_Source, AL_BUFFER, p_Buffer);

	alGenSources(1, &p_Source2);
	alSourcef(p_Source2, AL_PITCH, p_Pitch2);
	alSourcef(p_Source2, AL_GAIN, p_Gain2);
	alSource3f(p_Source2, AL_POSITION, p_Position2[0], p_Position2[1], p_Position2[2]);
	alSource3f(p_Source2, AL_VELOCITY, p_Velocity2[0], p_Velocity2[1], p_Velocity2[2]);
	alSourcei(p_Source2, AL_LOOPING, p_LoopSound2);
	alSourcei(p_Source2, AL_BUFFER, p_Buffer2);
}

SoundSorce::~SoundSorce()
{
	alDeleteSources(1, &p_Source);
	alDeleteSources(1, &p_Source2);
}

void SoundSorce::Play(const ALuint buffer_to_play, ALint bandera, const ALuint buffer_to_play2)
{
	if (buffer_to_play != p_Buffer)
	{
		p_Buffer = buffer_to_play;
		alSourcei(p_Source, AL_BUFFER, (ALuint)p_Buffer);
	}
	if (buffer_to_play2 != p_Buffer2)
	{
		p_Buffer2 = buffer_to_play2;
		alSourcei(p_Source2, AL_BUFFER, (ALuint)p_Buffer2);
	}

	//alSourcePlay(p_Source2);		//Audio asincrono

	if (bandera == 1)
		alSourcePlay(p_Source);
		alSourcePause(p_Source2);
	if (bandera == 2) {
		alSourcePause(p_Source);
		alSourcePlay(p_Source2);
	}
		
	

	ALint state = AL_PLAYING;
	/*std::cout << "Playing sound\n";
	while (state == AL_PLAYING && alGetError() == AL_NO_ERROR)
	{
		std::cout << "Reproduciendo sonido\n";
		alGetSourcei(p_Source, AL_SOURCE_STATE, &state);
	}
	std::cout << "Hecho sonido reproducido\n";*/
}