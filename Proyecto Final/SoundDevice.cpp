#include <AL\alc.h>
#include <AL\al.h>
#include <iostream>
#include "SoundDevice.h"

SoundDevice* SoundDevice::get()
{
	static SoundDevice* snd_device = new SoundDevice();
	return snd_device;
}

SoundDevice::SoundDevice() 
{
	p_ALCDevice = alcOpenDevice(nullptr);
	if (!p_ALCDevice)
		throw("Falla al obtener el dispositivo de sonido");

	p_ALCContext = alcCreateContext(p_ALCDevice, nullptr);
	if (!p_ALCContext)
		throw("Falla al establecer el contexto de sonido");

	if (!alcMakeContextCurrent(p_ALCContext))
		throw("Fallo al hacer el contexto actual");

	const ALCchar* name = nullptr;
	if (alcIsExtensionPresent(p_ALCDevice, "ALC_ENUMERATE_ALL_EXT"))
		name = alcGetString(p_ALCDevice, ALC_ALL_DEVICES_SPECIFIER);
	if (!name || alcGetError(p_ALCDevice) != AL_NO_ERROR)
		name = alcGetString(p_ALCDevice, ALC_DEVICE_SPECIFIER);
	printf("Opened   \"%s\"\n", name);
}

SoundDevice::~SoundDevice()
{
	
}
