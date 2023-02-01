#include "SoundEffectsPlayer.h"
#include <iostream>
#include "OpenALErrorCheck.h"
#include "SoundDevice.h"
#include "SoundEffectsLibrary.h"

namespace Audio {
	SoundEffectsPlayer::SoundEffectsPlayer(){
		alGenSources(1, &p_Source);
		alSourcei(p_Source, AL_BUFFER, 0);
		AL_CheckAndThrow();
	}
	SoundEffectsPlayer::SoundEffectsPlayer(const std::string soundEffect){
		p_soundEffect = soundEffect;
		alGenSources(1, &p_Source);
		alSourcei(p_Source, AL_BUFFER, SOUND_LIBRARY->Load(soundEffect));
		AL_CheckAndThrow();
	}
	SoundEffectsPlayer::SoundEffectsPlayer(const std::string soundEffect, const SampleSourse sample) {
		p_soundEffect = soundEffect;
		alGenSources(1, &p_Source);
		alSourcei(p_Source, AL_BUFFER, SOUND_LIBRARY->Load(soundEffect));
		SetSampleSourse(sample);
		AL_CheckAndThrow();
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

	bool SoundEffectsPlayer::isStopped(){
		ALint playState;
		alGetSourcei(p_Source, AL_SOURCE_STATE, &playState);
		return (playState == AL_STOPPED);
	}
}
