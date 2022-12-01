#include "PlayerManager.h"

static Audio::PlayerManager* playManager = nullptr;

namespace Audio {
	PlayerManager* PlayerManager::Get(){
		if (playManager == nullptr)
			playManager = new PlayerManager();
		return playManager;
	}

	std::shared_ptr<SoundEffectsPlayer> PlayerManager::add(SoundEffectsPlayer player){
		std::shared_ptr<SoundEffectsPlayer> added = std::make_shared<SoundEffectsPlayer>(std::move(player));
		m_vecPlayer.push_back(added);
		return added;
	}

	void PlayerManager::PauseAll(){
		for (auto It : m_vecPlayer) 
			It->Pause();
	}
	void PlayerManager::PlayAll(){
		for (auto It : m_vecPlayer)
			It->Play();
	}
	void PlayerManager::DeleteStop(){
		for (int i = 0; i < m_vecPlayer.size(); i++) 
			if (m_vecPlayer[i]->isStopped()) 
				m_vecPlayer.erase(m_vecPlayer.begin()+i);
	}

	void PlayerManager::Pause(std::shared_ptr<SoundEffectsPlayer> player){
		player->Pause();
	}
	void PlayerManager::Play(std::shared_ptr<SoundEffectsPlayer> player){
		player->Play();
	}
	void PlayerManager::Delete(std::shared_ptr<SoundEffectsPlayer> player){
		for (int i = 0; i < m_vecPlayer.size(); i++)
			if (m_vecPlayer[i]==player)
				m_vecPlayer.erase(m_vecPlayer.begin() + i);
	}


	PlayerManager::PlayerManager() {
		m_vecPlayer.clear();
	}
	PlayerManager::~PlayerManager(){
		m_vecPlayer.clear();
	}
}