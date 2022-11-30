#include "SoundEffectsPlayer.h"
#include <iostream>
#include "OpenALErrorCheck.h"

static Audio::SoundEffectsPlayer* player = nullptr;

namespace Audio {
	SoundEffectsPlayer::SoundEffectsPlayer(){
		alGenSources(1, &p_Source);
		alSourcei(p_Source, AL_BUFFER, 0);
		AL_CheckAndThrow();
	}
	SoundEffectsPlayer::SoundEffectsPlayer(const ALuint& buffer_to_play){
		alGenSources(1, &p_Source);
		alSourcei(p_Source, AL_BUFFER, buffer_to_play);
		AL_CheckAndThrow();
	}
	SoundEffectsPlayer::~SoundEffectsPlayer(){
		alDeleteSources(1, &p_Source);
	}

	void SoundEffectsPlayer::Play() {
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

	

	bool SoundEffectsPlayer::isPlaying(){
		ALint playState;
		alGetSourcei(p_Source, AL_SOURCE_STATE, &playState);
		return (playState == AL_PLAYING);
	}
}
