#include "SoundDevice.h"

#include <AL\al.h>
#include <iostream>
#include "OpenALErrorCheck.h"
#include "SoundEffectsLibrary.h"

ALCdevice* Audio::SoundDevice::m_ALCDevice;
ALCcontext* Audio::SoundDevice::m_ALCContext;

namespace Audio {
	void SoundDevice::setAttunation(int key){
		if (key < 0xD001 || key > 0xD006)
			std::cerr << "(!) bad attunation key" << std::endl;

		alDistanceModel(key);
		AL_CheckAndThrow();
	}

	void SoundDevice::getPosition(glm::vec3& posit) {
		alGetListener3f(AL_POSITION, &posit.x, &posit.y, &posit.z);
		AL_CheckAndThrow();
	}
	void SoundDevice::getVelocity(glm::vec3& velos){
		alGetListener3f(AL_VELOCITY, &velos.x, &velos.y, &velos.z);
		AL_CheckAndThrow();
	}
	void SoundDevice::getOrientation(glm::vec3& at, glm::vec3& up){
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
	void SoundDevice::getGain(float& gain){
		alGetListenerf(AL_GAIN, &gain);
		AL_CheckAndThrow();
	}

	void SoundDevice::setPosition(const glm::vec3& posit){
		alListener3f(AL_POSITION, posit.x, posit.y, posit.z);
		AL_CheckAndThrow();
	}
	void SoundDevice::setVelocity(const glm::vec3& velos) {
		alListener3f(AL_VELOCITY, velos.x, velos.y, velos.z);
		AL_CheckAndThrow();
	}
	void SoundDevice::setOrientation(const glm::vec3& at, const glm::vec3& up){
		float ori[] = { at.x,at.y,at.z,up.x,up.y,up.z };
		alListenerfv(AL_ORIENTATION, ori);
		AL_CheckAndThrow();
	}
	void SoundDevice::setGain(const float& val){
		alListenerf(AL_GAIN, val);
		AL_CheckAndThrow();
	}

	void SoundDevice::init(){
		m_ALCDevice = alcOpenDevice(nullptr);
		if (!m_ALCDevice)
			std::cerr << "(!) failed to get sound device" << std::endl;

		m_ALCContext = alcCreateContext(m_ALCDevice, nullptr);
		if (!m_ALCContext)
			std::cerr << "(!) Failed to set sound context" << std::endl;

		if (!alcMakeContextCurrent(m_ALCContext))
			std::cerr << "(!) failed to make context current" << std::endl;

		const ALCchar* name = nullptr;
		if (alcIsExtensionPresent(m_ALCDevice, "ALC_ENUMERATE_ALL_EXT"))
			name = alcGetString(m_ALCDevice, ALC_ALL_DEVICES_SPECIFIER);
		if (!name || alcGetError(m_ALCDevice) != AL_NO_ERROR)
			name = alcGetString(m_ALCDevice, ALC_DEVICE_SPECIFIER);
		std::cout << "Opened " <<name<< std::endl;
	}

	void SoundDevice::terminate(){
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(m_ALCContext);
		alcCloseDevice(m_ALCDevice);
	}
}
