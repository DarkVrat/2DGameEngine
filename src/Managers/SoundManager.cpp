#include "SoundManager.h"

#include <glm/vec3.hpp>
#include "../Audio/SoundDevice.h"
#include "../Audio/SoundEffectsLibrary.h"
#include "../Managers/ConfigManager.h"

Audio::SoundManager* soundManager = nullptr;

namespace Audio {
	SoundManager* SoundManager::Get() {
		if (soundManager == nullptr)
			soundManager = new SoundManager();
		return soundManager;
	}
	void SoundManager::Terminate() {
		delete soundManager;
	}

	void SoundManager::deletePlayer(std::shared_ptr<Audio::SoundEffectsPlayer> player){
		for (int i = 0; i < m_mapSoundPlayers.size(); i++) {
			if (m_mapSoundPlayers.at(i) == player) {
				m_mapSoundPlayers.erase(m_mapSoundPlayers.begin()+i);
			}
		}
	}

	void SoundManager::UpdateSoundSystem(){
		std::map<std::shared_ptr<Audio::SoundEffectsPlayer>, glm::vec3> position;
		for (int i = 0; i < m_mapSoundPlayers.size();i++) {
			std::shared_ptr<Audio::SoundEffectsPlayer> current = m_mapSoundPlayers.at(i);
			position.emplace(current, current->GetVec3Param(AL_POSITION));
			current->DeleteSourse();
		}

		Audio::SoundEffectsLibrary::Terminate();

		glm::vec3 positionDevice;
		glm::vec3 OrientationAt;
		glm::vec3 OrientationUp;
		SOUND_DEVICE->GetPosition(positionDevice);
		SOUND_DEVICE->GetOrientation(OrientationAt, OrientationUp);
		Audio::SoundDevice::Terminate();

		SOUND_DEVICE->SetGain(CONFIG_MANAGER->getVolumeSounde());
		SOUND_DEVICE->SetAttunation(AL_INVERSE_DISTANCE_CLAMPED);
		SOUND_DEVICE->SetPosition(positionDevice);
		SOUND_DEVICE->SetOrientation(OrientationAt, OrientationUp);

		SOUND_LIBRARY;

		for (int i = 0; i < m_mapSoundPlayers.size(); i++) {
			std::shared_ptr<Audio::SoundEffectsPlayer> current = m_mapSoundPlayers.at(i);
			current->CreateEffect();
			current->SetVec3Param(AL_POSITION,position.find(current)->second);
		}
	}

	void SoundManager::UpdateGain(){
		SOUND_DEVICE->SetGain(CONFIG_MANAGER->getVolumeSounde());
		for (auto& It : m_mapSoundPlayers) {
			It.get()->UpdateGain();
		}
	}
}