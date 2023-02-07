#pragma once

#include <memory>
#include <vector>
#include "../Audio/SoundEffectsPlayer.h"

#define SOUND_MANAGER Audio::SoundManager

namespace Audio {
	class SoundManager {
	public:
		static void addPlayer(std::shared_ptr<Audio::SoundEffectsPlayer> player) { m_mapSoundPlayers.push_back(player); }
		static void deletePlayer(std::shared_ptr<Audio::SoundEffectsPlayer> player);
		static void updateSoundSystem();
		static void updateGain();
	private:
		SoundManager() {};
		~SoundManager() { m_mapSoundPlayers.clear(); };

		static std::vector<std::shared_ptr<Audio::SoundEffectsPlayer>> m_mapSoundPlayers;
	};
}