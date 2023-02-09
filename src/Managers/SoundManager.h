#pragma once

#include <memory>
#include <vector>
#include "../Audio/SoundEffectsPlayer.h"

#define SOUND_MANAGER Audio::SoundManager

 //(RUS) Менеджер звукового сопровождения, отвечает за обновление громкости, и перезагрузку звуков
//(ENG) Sound manager, responsible for updating the volume, and reloading sounds

namespace Audio {
	class SoundManager {
	public:
		static void addPlayer(std::shared_ptr<Audio::SoundEffectsPlayer> player);
		static void deletePlayer(std::shared_ptr<Audio::SoundEffectsPlayer> player);
		static void updateSoundSystem();
		static void updateGain();

		static void terminate();
	private:
		SoundManager() {};
		~SoundManager() {};

		static std::vector<std::shared_ptr<Audio::SoundEffectsPlayer>> m_vectorSoundPlayers;
	};
}