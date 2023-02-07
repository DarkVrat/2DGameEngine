#include "SoundManager.h"

#include <glm/vec3.hpp>
#include "../Audio/SoundDevice.h"
#include "../Audio/SoundEffectsLibrary.h"
#include "../Managers/ConfigManager.h"

std::vector<std::shared_ptr<Audio::SoundEffectsPlayer>> Audio::SoundManager::m_mapSoundPlayers;

namespace Audio {
	void SoundManager::deletePlayer(std::shared_ptr<Audio::SoundEffectsPlayer> player){
		for (int i = 0; i < m_mapSoundPlayers.size(); i++) {
			if (m_mapSoundPlayers.at(i) == player) {
				m_mapSoundPlayers.erase(m_mapSoundPlayers.begin()+i);
			}
		}
	}

	void SoundManager::updateSoundSystem(){
		std::map<std::shared_ptr<Audio::SoundEffectsPlayer>, glm::vec3> position;
		for (int i = 0; i < m_mapSoundPlayers.size();i++) {
			std::shared_ptr<Audio::SoundEffectsPlayer> current = m_mapSoundPlayers.at(i);
			position.emplace(current, current->getVec3Param(AL_POSITION));
			current->deleteSourse();
		}

		SOUND_LIBRARY::terminate();

		glm::vec3 positionDevice;
		glm::vec3 OrientationAt;
		glm::vec3 OrientationUp;
		SOUND_DEVICE::getPosition(positionDevice);
		SOUND_DEVICE::getOrientation(OrientationAt, OrientationUp);
		SOUND_DEVICE::terminate();

		SOUND_DEVICE::init();
		SOUND_DEVICE::setGain(CONFIG_MANAGER::getVolumeSounde());
		SOUND_DEVICE::setAttunation(AL_INVERSE_DISTANCE_CLAMPED);
		SOUND_DEVICE::setPosition(positionDevice);
		SOUND_DEVICE::setOrientation(OrientationAt, OrientationUp);

		SOUND_LIBRARY::init();

		for (int i = 0; i < m_mapSoundPlayers.size(); i++) {
			std::shared_ptr<Audio::SoundEffectsPlayer> current = m_mapSoundPlayers.at(i);
			current->createEffect();
			current->setVec3Param(AL_POSITION,position.find(current)->second);
		}
	}

	void SoundManager::updateGain(){
		SOUND_DEVICE::setGain(CONFIG_MANAGER::getVolumeSounde());
		for (auto& It : m_mapSoundPlayers) {
			It.get()->updateGain();
		}
	}
}