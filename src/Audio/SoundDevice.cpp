#include "SoundDevice.h"
#include <AL\al.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include "OpenALErrorCheck.h"

static Audio::SoundDevice* soundDevice = nullptr;

namespace Audio {
	SoundDevice* SoundDevice::Get(){
		if (soundDevice == nullptr)
			soundDevice = new SoundDevice();
		return soundDevice;
	}

	void SoundDevice::SetAttunation(int key){
		if (key < 0xD001 || key > 0xD006)
			std::cerr << "(!) bad attunation key" << std::endl;

		alDistanceModel(key);
		AL_CheckAndThrow();
	}

	void SoundDevice::GetPosition(glm::vec3& posit) {
		alGetListener3f(AL_POSITION, &posit.x, &posit.y, &posit.z);
		AL_CheckAndThrow();
	}
	void SoundDevice::GetVelocity(glm::vec3& velos){
		alGetListener3f(AL_VELOCITY, &velos.x, &velos.y, &velos.z);
		AL_CheckAndThrow();
	}
	void SoundDevice::GetOrientation(glm::vec3& at, glm::vec3& up){
		float ori[6];
		alGetListenerfv(AL_ORIENTATION, ori);
		at.x = ori[0];
		at.y = ori[1];
		at.z = ori[2];
		up.x = ori[3];
		up.y = ori[4];
		up.z = ori[5];
		AL_CheckAndThrow();
	}
	void SoundDevice::GetGain(float& gain){
		alGetListenerf(AL_GAIN, &gain);
		AL_CheckAndThrow();
	}

	void SoundDevice::SetPosition(const glm::vec3& posit){
		alListener3f(AL_POSITION, posit.x, posit.y, posit.z);
		AL_CheckAndThrow();
	}
	void SoundDevice::SetVelocity(const glm::vec3& velos) {
		alListener3f(AL_VELOCITY, velos.x, velos.y, velos.z);
		AL_CheckAndThrow();
	}
	void SoundDevice::SetOrientation(const glm::vec3& at, const glm::vec3& up){
		float ori[] = { at.x,at.y,at.z,up.x,up.y,up.z };
		alListenerfv(AL_ORIENTATION, ori);
		AL_CheckAndThrow();
	}
	void SoundDevice::SetGain(const float& val){
		alListenerf(AL_GAIN, val);
		AL_CheckAndThrow();
	}

	SoundDevice::SoundDevice(){
		p_ALCDevice = alcOpenDevice(nullptr);
		if (!p_ALCDevice)
			std::cerr << "(!) failed to get sound device" << std::endl;

		p_ALCContext = alcCreateContext(p_ALCDevice, nullptr);
		if (!p_ALCContext)
			std::cerr << "(!) Failed to set sound context" << std::endl;

		if (!alcMakeContextCurrent(p_ALCContext))
			std::cerr << "(!) failed to make context current" << std::endl;

		const ALCchar* name = nullptr;
		if (alcIsExtensionPresent(p_ALCDevice, "ALC_ENUMERATE_ALL_EXT"))
			name = alcGetString(p_ALCDevice, ALC_ALL_DEVICES_SPECIFIER);
		if (!name || alcGetError(p_ALCDevice) != AL_NO_ERROR)
			name = alcGetString(p_ALCDevice, ALC_DEVICE_SPECIFIER);
		std::cout << "Opened " <<name<< std::endl;
	}

	SoundDevice::~SoundDevice(){
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(p_ALCContext);
		alcCloseDevice(p_ALCDevice);
	}
}
