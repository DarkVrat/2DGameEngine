#pragma once
#include <AL\al.h>
#include <glm/vec3.hpp>
#include "SampleSourse.h"
#include <iostream>
#include <memory>

#define MAKE_SOUND_PLAYER Audio::SoundEffectsPlayer::MakeSoundEffectPlayer

namespace Audio {
	class SoundEffectsPlayer{
	public:
		static std::shared_ptr<Audio::SoundEffectsPlayer> MakeSoundEffectPlayer(const std::string soundEffect);
		SoundEffectsPlayer(const std::string soundEffect);
		~SoundEffectsPlayer();

		void Play();
		void Stop();
		void Pause();
		void Rewind();

		void SetFloatParam(ALenum param, const float& value);
		void SetVec3Param(ALenum param, const glm::vec3& value);
		void SetIntParam(ALenum param, const int& value);
		void SetSampleSourse(const SampleSourse sample);

		float GetFloatParam(ALenum param);
		glm::vec3 GetVec3Param(ALenum param);
		int GetIntParam(ALenum param);
		Audio::SampleSourse GetSampleSourse();

		void DeleteSourse();
		void CreateEffect();

		bool isStopped();

		ALuint GetSource() { return p_Source; }

	private:
		std::string p_soundEffect;
		ALuint p_Source;
	};
}
