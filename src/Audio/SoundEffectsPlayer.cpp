#include "SoundEffectsPlayer.h"

#include "SoundDevice.h"
#include "SoundEffectsLibrary.h"
#include "../Managers/SoundManager.h"
#include "../Managers/ConfigManager.h"
#include "../Managers/ResourceManager.h"

namespace Audio {
	   //(RUS) Создание звукового эффекта, помещение его в звуковой менеджер
	  //   (!) создание плеера не через MakeSoundEffectPlayer Сделает его невозможным для перезагрузки звука, и обновления громкости
	 //(ENG) Creating a sound effect, placing it in the sound manager
	//   (!) making a player not via MakeSoundEffectPlayer Will make it impossible to reload the sound, and volume updates
	std::shared_ptr<Audio::SoundEffectsPlayer> SoundEffectsPlayer::MakeSoundEffectPlayer(const std::string& soundEffect, const std::string& sampleName){
		std::shared_ptr<Audio::SoundEffectsPlayer> player = std::make_shared<Audio::SoundEffectsPlayer>(soundEffect, sampleName);
		SOUND_MANAGER::addPlayer(player);
		return player;
	}

	 //(RUS) Конструктор звукового плеера
	//(ENG) Sound player constructor
	SoundEffectsPlayer::SoundEffectsPlayer(const std::string& soundEffect, const std::string& sampleName){
		m_soundEffect = soundEffect;
		m_sampleSource = RESOURCE_MANAGER::getSampleSourse(sampleName);
		createEffect();
	}

	 //(RUS) Удаление звукового плеера
	//(ENG) Deleting the sound player
	SoundEffectsPlayer::~SoundEffectsPlayer(){
		alDeleteSources(1, &m_source);
	}

	 //(RUS) Воспроизведение, остановка, пауза и перемотка назад звукового плеера
	//(ENG) Play, stop, pause and rewind audio player
	void SoundEffectsPlayer::play() {
		ALint playState;
		alGetSourcei(m_source, AL_SOURCE_STATE, &playState);
		if(playState!=AL_PLAYING)
			alSourcePlay(m_source);
		SOUND_DEVICE::AL_CheckAndThrow();
	}
	void SoundEffectsPlayer::stop(){
		alSourceStop(m_source);
		SOUND_DEVICE::AL_CheckAndThrow();
	}
	void SoundEffectsPlayer::pause(){
		alSourcePause(m_source);
		SOUND_DEVICE::AL_CheckAndThrow();
	}
	void SoundEffectsPlayer::rewind() {
		alSourceRewind(m_source);
		SOUND_DEVICE::AL_CheckAndThrow();
	}

	 //(RUS) Установка параметров источника
	//(ENG) Setting Source Options
	void SoundEffectsPlayer::setVec3Param(const ALenum& param, const glm::vec3& value){
		alSource3f(m_source, param, value.x, value.y, value.z);
		SOUND_DEVICE::AL_CheckAndThrow();
	}
	void SoundEffectsPlayer::setSampleSourse(){
		alSourcef(m_source, AL_PITCH, m_sampleSource->AlPitch);
		alSourcef(m_source, AL_MAX_DISTANCE, m_sampleSource->AlMaxDistance);
		alSourcef(m_source, AL_ROLLOFF_FACTOR, m_sampleSource->AlRolloffFactor);
		alSourcef(m_source, AL_REFERENCE_DISTANCE, m_sampleSource->AlReferenceDistance);
		alSourcef(m_source, AL_MIN_GAIN, m_sampleSource->AlMinGain);
		alSourcef(m_source, AL_MAX_GAIN, m_sampleSource->AlMaxGain);
		alSourcef(m_source, AL_CONE_OUTER_GAIN, m_sampleSource->AlGainOutCone);
		alSourcef(m_source, AL_CONE_INNER_ANGLE, m_sampleSource->AlAngleInCone);
		alSourcef(m_source, AL_CONE_OUTER_ANGLE, m_sampleSource->AlAngleOutCone);
		SOUND_DEVICE::AL_CheckAndThrow();
	}

	 //(RUS) Обновление громкости плеера при изменении громкости в конфиге
	//(ENG) Update the player's volume when changing the volume in the config
	void SoundEffectsPlayer::updateGain(){
		alSourcef(m_source, AL_GAIN, CONFIG_MANAGER::getVolumeSample(m_sampleSource->GainString));
	}

 	 //(RUS) Получение параметра плеера
	//(ENG) Getting the player parameter
	void SoundEffectsPlayer::getVec3Param(const ALenum& param, glm::vec3& value){
		alGetSource3f(m_source, param, &value.x, &value.y, &value.z);
	}

	 //(RUS) Удаление звукового источника
	//(ENG) Deleting the sound sourse
	void SoundEffectsPlayer::deleteSourse() { alDeleteSources(1, &m_source); }

	 //(RUS) Создание источника и установка параметров
	//(ENG) Creating a Source and Setting Parameters
	void SoundEffectsPlayer::createEffect(){
		alGenSources(1, &m_source);
		alSourcei(m_source, AL_BUFFER, SOUND_LIBRARY::load(m_soundEffect));
		updateGain();
		setSampleSourse();
	}

	 //(RUS) Проверка остановлен ли плеер
	//(ENG) Checking if the player is stopped
	bool SoundEffectsPlayer::isStopped(){
		ALint playState;
		alGetSourcei(m_source, AL_SOURCE_STATE, &playState);
		return (playState == AL_STOPPED);
	}
}
