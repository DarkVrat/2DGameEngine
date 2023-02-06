#pragma once

#include <memory>
#include <vector>
#include "../Audio/SoundEffectsPlayer.h"

#define SOUND_MANAGER Audio::SoundManager::Get()

namespace Audio {
	class SoundManager {
	public:
		static SoundManager* Get();
		static void Terminate();

		void addPlayer(std::shared_ptr<Audio::SoundEffectsPlayer> player) { m_mapSoundPlayers.push_back(player); }
		void deletePlayer(std::shared_ptr<Audio::SoundEffectsPlayer> player);
		void UpdateSoundSystem();
		void UpdateGain();
	private:
		SoundManager() {};
		~SoundManager() { m_mapSoundPlayers.clear(); };

		std::vector<std::shared_ptr<Audio::SoundEffectsPlayer>> m_mapSoundPlayers;
	};
}