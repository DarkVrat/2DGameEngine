#pragma once

#include <AL\alc.h>
#include <glm/vec3.hpp>

#define SOUND_DEVICE Audio::SoundDevice

 //(RUS) Устройство для воспроизведения звука
//(ENG) Audio playback device

namespace Audio {
	class SoundDevice{
	public:
		static void init();
		static void terminate();

		static void setAttunation(int key);

		static void getPosition(glm::vec3& posit);
		static void getVelocity(glm::vec3& velos);
		static void getOrientation(glm::vec3& at, glm::vec3& up);
		static void getGain(float& gain);

		static void setPosition(const glm::vec3& posit);
		static void setVelocity(const glm::vec3& velos);
		static void setOrientation(const glm::vec3& at, const glm::vec3& up);
		static void setGain(const float& gain);

		static void AL_CheckAndThrow();
	private:

		static ALCdevice* m_ALCDevice;
		static ALCcontext* m_ALCContext;

	};
}
