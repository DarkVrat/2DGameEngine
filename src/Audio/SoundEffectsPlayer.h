#pragma once

#include <iostream>
#include <memory>
#include <AL\al.h>
#include <glm/vec3.hpp>
#include "SampleSourse.h"

#define MAKE_SOUND_PLAYER Audio::SoundEffectsPlayer::MakeSoundEffectPlayer

 //(RUS) Плеер для звукового эффекта, управление параметрами источника
//(ENG) Sound effect player, source control

namespace Audio {
	class SoundEffectsPlayer{
	public:
		static std::shared_ptr<Audio::SoundEffectsPlayer> MakeSoundEffectPlayer(const std::string& soundEffect, const std::string& sampleName="default");
		SoundEffectsPlayer(const std::string& soundEffect, const std::string& sampleName);
		~SoundEffectsPlayer();

		void play();
		void stop();
		void pause();
		void rewind();

		bool isStopped();

		void setVec3Param(const ALenum& param, const glm::vec3& value);
		void setSampleSourse();

		void updateGain();

		void getVec3Param(const ALenum& param, glm::vec3& value);

		void deleteSourse();
		void createEffect();

	private:
		std::string m_soundEffect;
		std::shared_ptr<Audio::SampleSourse> m_sampleSource;
		ALuint m_source;
	};
}
