#pragma once
#include <AL\al.h>
#include <glm/vec3.hpp>
#include "SampleSourse.h"
#include <iostream>

namespace Audio {
	class SoundEffectsPlayer{
	public:
		SoundEffectsPlayer();
		SoundEffectsPlayer(const std::string soundEffect);
		SoundEffectsPlayer(const std::string soundEffect, const SampleSourse sample);
		~SoundEffectsPlayer();

		void Play();
		void Stop();
		void Pause();
		void Rewind();

		void SetFloatParam(ALenum param, const float& value);
		void SetVec3Param(ALenum param, const glm::vec3& value);
		void SetIntParam(ALenum param, const int& value);
		void SetSampleSourse(const SampleSourse sample);

		bool isStopped();

		ALuint GetSource() { return p_Source; }

	private:
		std::string p_soundEffect;
		ALuint p_Source;
	};
}
