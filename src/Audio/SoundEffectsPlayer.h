#pragma once
#include <AL\al.h>
#include <glm/vec3.hpp>
#include "SampleSourse.h"

namespace Audio {
	class SoundEffectsPlayer{
	public:
		SoundEffectsPlayer();
		SoundEffectsPlayer(const ALuint& buffer_to_play);
		SoundEffectsPlayer(const ALuint& buffer_to_play, const SampleSourse sample);
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
		ALuint p_Source;
	};
}
