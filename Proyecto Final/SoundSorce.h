#pragma once
#include <AL\al.h>
class SoundSorce
{
public:
	SoundSorce();
	~SoundSorce();

	void Play(const ALuint buffer_to_play, ALint bandera, const ALuint buffer_to_play2);

private:
	ALuint p_Source;
	float p_Pitch = 1.0f;
	float p_Gain = 0.5f;			//Volumen
	float p_Position[3] = { 0,0,0 };
	float p_Velocity[3] = { 0,0,0 };
	bool p_LoopSound = false;
	ALuint p_Buffer = 0;
	
	ALuint p_Source2;
	float p_Pitch2 = 1.0f;
	float p_Gain2 = 1.0f;
	float p_Position2[3] = { 0,0,0 };
	float p_Velocity2[3] = { 0,0,0 };
	bool p_LoopSound2 = true;
	ALuint p_Buffer2 = 0;
};

