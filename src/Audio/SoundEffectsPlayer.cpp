#include "SoundEffectsPlayer.h"
#include <iostream>
#include "OpenALErrorCheck.h"
#include "SoundDevice.h"
#include "SoundEffectsLibrary.h"
#include "SoundManager.h"

namespace Audio {
	SoundEffectsPlayer::SoundEffectsPlayer(const std::string soundEffect){
		p_soundEffect = soundEffect;
		alGenSources(1, &p_Source);
		alSourcei(p_Source, AL_BUFFER, SOUND_LIBRARY->Load(soundEffect));
		AL_CheckAndThrow();
	}
	SoundEffectsPlayer::~SoundEffectsPlayer(){
		alDeleteSources(1, &p_Source);
	}

	std::shared_ptr<Audio::SoundEffectsPlayer> SoundEffectsPlayer::MakeSoundEffectPlayer(const std::string soundEffect){
		std::shared_ptr<Audio::SoundEffectsPlayer> player = std::make_shared< Audio::SoundEffectsPlayer>(soundEffect);
		SOUND_MANAGER->addPlayer(player);
		return player;
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

	void SoundEffectsPlayer::SetFloatParam(ALenum param, const float& value){
		alSourcef(p_Source, param, value);
		AL_CheckAndThrow();
	}
	void SoundEffectsPlayer::SetVec3Param(ALenum param, const glm::vec3& value){
		alSource3f(p_Source, param, value.x, value.y, value.z);
		AL_CheckAndThrow();
	}
	void SoundEffectsPlayer::SetIntParam(ALenum param, const int& value){
		alSourcei(p_Source, param, value);
		AL_CheckAndThrow();
	}
	void SoundEffectsPlayer::SetSampleSourse(const SampleSourse sample){
		alSourcef(p_Source, AL_PITCH, sample.AlPitch);
		alSourcef(p_Source, AL_GAIN, sample.AlGain);
		alSourcef(p_Source, AL_MAX_DISTANCE, sample.AlMaxDistance);
		alSourcef(p_Source, AL_ROLLOFF_FACTOR, sample.AlRolloffFactor);
		alSourcef(p_Source, AL_REFERENCE_DISTANCE, sample.AlReferenceDistance);
		alSourcef(p_Source, AL_MIN_GAIN, sample.AlMinGain);
		alSourcef(p_Source, AL_MAX_GAIN, sample.AlMaxGain);
		alSourcef(p_Source, AL_CONE_OUTER_GAIN, sample.AlGainOutCone);
		alSourcef(p_Source, AL_CONE_INNER_ANGLE, sample.AlAngleInCone);
		alSourcef(p_Source, AL_CONE_OUTER_ANGLE, sample.AlAngleOutCone);
	}

	float SoundEffectsPlayer::GetFloatParam(ALenum param){
		float value;
		alGetSourcef(p_Source, param, &value);
		return value;
	}
	glm::vec3 SoundEffectsPlayer::GetVec3Param(ALenum param){
		glm::vec3 value(0,0,0);
		alGetSource3f(p_Source, param, &value.x, &value.y, &value.z);
		return value;
	}
	int SoundEffectsPlayer::GetIntParam(ALenum param){
		int value;
		alGetSourcei(p_Source, param, &value);
		return value;
	}
	Audio::SampleSourse SoundEffectsPlayer::GetSampleSourse(){
		Audio::SampleSourse sample;
		alGetSourcef(p_Source, AL_PITCH, &sample.AlPitch);
		alGetSourcef(p_Source, AL_GAIN, &sample.AlGain);
		alGetSourcef(p_Source, AL_MAX_DISTANCE, &sample.AlMaxDistance);
		alGetSourcef(p_Source, AL_ROLLOFF_FACTOR, &sample.AlRolloffFactor);
		alGetSourcef(p_Source, AL_REFERENCE_DISTANCE, &sample.AlReferenceDistance);
		alGetSourcef(p_Source, AL_MIN_GAIN, &sample.AlMinGain);
		alGetSourcef(p_Source, AL_MAX_GAIN, &sample.AlMaxGain);
		alGetSourcef(p_Source, AL_CONE_OUTER_GAIN, &sample.AlGainOutCone);
		alGetSourcef(p_Source, AL_CONE_INNER_ANGLE, &sample.AlAngleInCone);
		alGetSourcef(p_Source, AL_CONE_OUTER_ANGLE, &sample.AlAngleOutCone);
		return sample;
	}

	void SoundEffectsPlayer::DeleteSourse(){
		alDeleteSources(1, &p_Source);
		p_Source = 0;
	}
	void SoundEffectsPlayer::CreateEffect(){
		alGenSources(1, &p_Source);
		alSourcei(p_Source, AL_BUFFER, SOUND_LIBRARY->Load(p_soundEffect));
		AL_CheckAndThrow();
	}

	bool SoundEffectsPlayer::isStopped(){
		ALint playState;
		alGetSourcei(p_Source, AL_SOURCE_STATE, &playState);
		return (playState == AL_STOPPED);
	}
}
