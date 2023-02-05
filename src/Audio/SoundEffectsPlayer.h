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
		static std::shared_ptr<Audio::SoundEffectsPlayer> MakeSoundEffectPlayer(const std::string soundEffect, const std::string sampleName="default");
		SoundEffectsPlayer(const std::string soundEffect, const std::string sampleName );
		~SoundEffectsPlayer();

		void Play();
		void Stop();
		void Pause();
		void Rewind();

		bool isStopped();

		void SetVec3Param(ALenum param, const glm::vec3& value);
		void SetSampleSourse();

		void UpdateGain();

		glm::vec3 GetVec3Param(ALenum param);
		ALuint GetSource() { return p_Source; }

		void DeleteSourse();
		void CreateEffect();

	private:
		std::string p_soundEffect;
		Audio::SampleSourse p_sampleSource;
		ALuint p_Source;
	};
}
