#include "SoundEffectsPlayer.h"

#include "OpenALErrorCheck.h"
#include "SoundDevice.h"
#include "SoundEffectsLibrary.h"
#include "../Managers/SoundManager.h"
#include "../Managers/ConfigManager.h"
#include "../Managers/ResourceManager.h"

namespace Audio {
	std::shared_ptr<Audio::SoundEffectsPlayer> SoundEffectsPlayer::MakeSoundEffectPlayer(const std::string soundEffect, const std::string sampleName){
		std::shared_ptr<Audio::SoundEffectsPlayer> player = std::make_shared<Audio::SoundEffectsPlayer>(soundEffect, sampleName);
		SOUND_MANAGER::addPlayer(player);
		return player;
	}

	SoundEffectsPlayer::SoundEffectsPlayer(const std::string soundEffect, const std::string sampleName){
		m_soundEffect = soundEffect;
		m_sampleSource = RESOURCE_MANAGER::getSampleSourse(sampleName);

		createEffect();

		
	}
	SoundEffectsPlayer::~SoundEffectsPlayer(){
		alDeleteSources(1, &m_source);
	}


	void SoundEffectsPlayer::play() {
		ALint playState;
		alGetSourcei(m_source, AL_SOURCE_STATE, &playState);
		if(playState!=AL_PLAYING)
			alSourcePlay(m_source);
		AL_CheckAndThrow();
	}
	void SoundEffectsPlayer::stop(){
		alSourceStop(m_source);
		AL_CheckAndThrow();
	}
	void SoundEffectsPlayer::pause(){
		alSourcePause(m_source);
		AL_CheckAndThrow();
	}
	void SoundEffectsPlayer::rewind() {
		alSourceRewind(m_source);
		AL_CheckAndThrow();
	}

	void SoundEffectsPlayer::setVec3Param(ALenum param, const glm::vec3& value){
		alSource3f(m_source, param, value.x, value.y, value.z);
		AL_CheckAndThrow();
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
		AL_CheckAndThrow();
	}

	void SoundEffectsPlayer::updateGain(){
		alSourcef(m_source, AL_GAIN, CONFIG_MANAGER::getVolumeSample(m_sampleSource->GainString));
	}

	glm::vec3 SoundEffectsPlayer::getVec3Param(ALenum param){
		glm::vec3 value(0,0,0);
		alGetSource3f(m_source, param, &value.x, &value.y, &value.z);
		return value;
	}

	ALuint SoundEffectsPlayer::getSource(){
		return m_source;
	}


	void SoundEffectsPlayer::deleteSourse(){
		alDeleteSources(1, &m_source);
	}
	void SoundEffectsPlayer::createEffect(){
		alGenSources(1, &m_source);
		alSourcei(m_source, AL_BUFFER, SOUND_LIBRARY::load(m_soundEffect));
		setSampleSourse();
		updateGain();
		AL_CheckAndThrow();
	}

	bool SoundEffectsPlayer::isStopped(){
		ALint playState;
		alGetSourcei(m_source, AL_SOURCE_STATE, &playState);
		return (playState == AL_STOPPED);
	}
}
