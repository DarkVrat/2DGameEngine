#include "SoundManager.h"

#include <glm/vec3.hpp>
#include "../Audio/SoundDevice.h"
#include "../Audio/SoundEffectsLibrary.h"
#include "../Managers/ConfigManager.h"

std::vector<std::shared_ptr<Audio::SoundEffectsPlayer>> Audio::SoundManager::m_vectorSoundPlayers;

namespace Audio {
	 //(RUS) Добавление плеера в вектор
	//(ENG) Adding a player to a vector
	void SoundManager::addPlayer(std::shared_ptr<Audio::SoundEffectsPlayer> player) { m_vectorSoundPlayers.push_back(player); }

	 //(RUS) Удаление плеера
    //(ENG) Deleting a player
	void SoundManager::deletePlayer(std::shared_ptr<Audio::SoundEffectsPlayer> player){
		for (int i = 0; i < m_vectorSoundPlayers.size(); i++) {
			if (m_vectorSoundPlayers.at(i) == player) {
				m_vectorSoundPlayers.erase(m_vectorSoundPlayers.begin()+i);
			}
		}
	}

	 //(RUS) Обновление звуковой системы, сохранение необходимых не постоянных параметров, и создание всего заного
	//(ENG) Updating the sound system, saving the necessary non-permanent parameters, and creating everything from scratch
	void SoundManager::updateSoundSystem(){
		std::map<std::shared_ptr<Audio::SoundEffectsPlayer>, glm::vec3> position;
		std::map<std::shared_ptr<Audio::SoundEffectsPlayer>, bool> playingSound;
		for (int i = 0; i < m_vectorSoundPlayers.size();i++) {
			std::shared_ptr<Audio::SoundEffectsPlayer> current = m_vectorSoundPlayers.at(i);

			glm::vec3 val;
			current->getVec3Param(AL_POSITION,val);
			position.emplace(current, val);

			bool play = !current->isStopped();
			playingSound.emplace(current, play);

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

		for (int i = 0; i < m_vectorSoundPlayers.size(); i++) {
			std::shared_ptr<Audio::SoundEffectsPlayer> current = m_vectorSoundPlayers.at(i);
			current->createEffect();
			current->setVec3Param(AL_POSITION,position.find(current)->second);
			if (playingSound.at(current)) {
				current->play();
			}
		}
	}

	 //(RUS) Обновление громкости звука
	//(ENG) Sound volume update
	void SoundManager::updateGain(){
		SOUND_DEVICE::setGain(CONFIG_MANAGER::getVolumeSounde());
		for (auto& It : m_vectorSoundPlayers) {
			It.get()->updateGain();
		}
	}

	 //(RUS) Очистка вектора, и уничтожение источника с девайсом
	//(ENG) Clearing the vector, and destroying the source with the device
	void SoundManager::terminate(){
		m_vectorSoundPlayers.clear();
		SOUND_LIBRARY::terminate();
		SOUND_DEVICE::terminate();
	}
}