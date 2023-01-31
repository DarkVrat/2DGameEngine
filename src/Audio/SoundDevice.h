#pragma once

#include <AL\alc.h>
#include <glm/vec3.hpp>

#define SOUND_DEVICE Audio::SoundDevice::Get()

namespace Audio {
	class SoundDevice{
	public:
		static SoundDevice* Get();
		static void Terminate();

		void SetAttunation(int key);

		void GetPosition(glm::vec3& posit);
		void GetVelocity(glm::vec3& velos);
		void GetOrientation(glm::vec3& at, glm::vec3& up);
		void GetGain(float& gain);

		void SetPosition(const glm::vec3& posit);
		void SetVelocity(const glm::vec3& velos);
		void SetOrientation(const glm::vec3& at, const glm::vec3& up);
		void SetGain(const float& gain);

	private:
		SoundDevice();
		~SoundDevice();

		ALCdevice* p_ALCDevice;
		ALCcontext* p_ALCContext;

	};
}
