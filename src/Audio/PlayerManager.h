#pragma once
#include "SoundEffectsPlayer.h"
#include "SampleSourse.h"
#include <vector>
#include <memory>

#define GETPLAYER Audio::PlayerManager::Get();

namespace Audio {
	class PlayerManager {
	public:
		static PlayerManager* Get();

		std::shared_ptr<SoundEffectsPlayer> add(SoundEffectsPlayer player);

		void PauseAll();
		void PlayAll();
		void DeleteStop();

		void Pause(std::shared_ptr<SoundEffectsPlayer> player);
		void Play(std::shared_ptr<SoundEffectsPlayer> player);
		void Delete(std::shared_ptr<SoundEffectsPlayer> player);

	private:
		PlayerManager();
		~PlayerManager();

		std::vector<std::shared_ptr<SoundEffectsPlayer>> m_vecPlayer;
	};
}