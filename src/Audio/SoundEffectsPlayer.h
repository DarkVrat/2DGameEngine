#pragma once
#include <AL\al.h>
#include <glm/vec3.hpp>

namespace Audio {
	class SoundEffectsPlayer{
	public:
		SoundEffectsPlayer();
		SoundEffectsPlayer(const ALuint& buffer_to_play);
		~SoundEffectsPlayer();

		void Play();
		void Stop();
		void Pause();
		void Rewind();

		void SetFloatParam(ALenum param, const float& value);
		void SetVec3Param(ALenum param, const glm::vec3& value);
		void SetIntParam(ALenum param, const int& value);

		bool isPlaying();

	private:
		ALuint p_Source;
	};
}
