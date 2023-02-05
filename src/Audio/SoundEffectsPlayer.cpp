#include "SoundEffectsPlayer.h"
#include <iostream>
#include "OpenALErrorCheck.h"
#include "SoundDevice.h"
#include "SoundEffectsLibrary.h"
#include "SoundManager.h"

namespace Audio {
	std::shared_ptr<Audio::SoundEffectsPlayer> SoundEffectsPlayer::MakeSoundEffectPlayer(const std::string soundEffect, const std::string sampleName){
		std::shared_ptr<Audio::SoundEffectsPlayer> player = std::make_shared<Audio::SoundEffectsPlayer>(soundEffect, sampleName);
		SOUND_MANAGER->addPlayer(player);
		return player;
	}

	SoundEffectsPlayer::SoundEffectsPlayer(const std::string soundEffect, const std::string sampleName){
		p_soundEffect = soundEffect;
		p_sampleSource = RESOURCE_MANAGER->getSampleSourse(sampleName);

		CreateEffect();

		
	}
	SoundEffectsPlayer::~SoundEffectsPlayer(){
		alDeleteSources(1, &p_Source);
	}


	void SoundEffectsPlayer::Play() {
		ALint playState;
		alGetSourcei(p_Source, AL_SOURCE_STATE, &playState);
		if(playState!=AL_PLAYING)
			alSourcePlay(p_Source);
		AL_CheckAndThrow();
	}
	void SoundEffectsPlayer::Stop(){
		alSourceStop(p_Source);
		AL_CheckAndThrow();
	}
	void SoundEffectsPlayer::Pause(){
		alSourcePause(p_Source);
		AL_CheckAndThrow();
	}
	void SoundEffectsPlayer::Rewind() {
		alSourceRewind(p_Source);
		AL_CheckAndThrow();
	}

	void SoundEffectsPlayer::SetVec3Param(ALenum param, const glm::vec3& value){
		alSource3f(p_Source, param, value.x, value.y, value.z);
		AL_CheckAndThrow();
	}
	void SoundEffectsPlayer::SetSampleSourse(){
		alSourcef(p_Source, AL_PITCH, p_sampleSource.AlPitch);
		alSourcef(p_Source, AL_MAX_DISTANCE, p_sampleSource.AlMaxDistance);
		alSourcef(p_Source, AL_ROLLOFF_FACTOR, p_sampleSource.AlRolloffFactor);
		alSourcef(p_Source, AL_REFERENCE_DISTANCE, p_sampleSource.AlReferenceDistance);
		alSourcef(p_Source, AL_MIN_GAIN, p_sampleSource.AlMinGain);
		alSourcef(p_Source, AL_MAX_GAIN, p_sampleSource.AlMaxGain);
		alSourcef(p_Source, AL_CONE_OUTER_GAIN, p_sampleSource.AlGainOutCone);
		alSourcef(p_Source, AL_CONE_INNER_ANGLE, p_sampleSource.AlAngleInCone);
		alSourcef(p_Source, AL_CONE_OUTER_ANGLE, p_sampleSource.AlAngleOutCone);
		AL_CheckAndThrow();
	}

	void SoundEffectsPlayer::UpdateGain(){
		alSourcef(p_Source, AL_GAIN, RENDER_ENGINE->getVolumeSample(p_sampleSource.GainString));
	}

	glm::vec3 SoundEffectsPlayer::GetVec3Param(ALenum param){
		glm::vec3 value(0,0,0);
		alGetSource3f(p_Source, param, &value.x, &value.y, &value.z);
		return value;
	}


	void SoundEffectsPlayer::DeleteSourse(){
		alDeleteSources(1, &p_Source);
	}
	void SoundEffectsPlayer::CreateEffect(){
		alGenSources(1, &p_Source);
		alSourcei(p_Source, AL_BUFFER, SOUND_LIBRARY->Load(p_soundEffect));
		SetSampleSourse();
		UpdateGain();
		AL_CheckAndThrow();
	}

	bool SoundEffectsPlayer::isStopped(){
		ALint playState;
		alGetSourcei(p_Source, AL_SOURCE_STATE, &playState);
		return (playState == AL_STOPPED);
	}
}
