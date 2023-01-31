#include "SoundEffectsLibrary.h"
#include "../Resources/ResourceManager.h"
#include <sndfile.h>
#include <inttypes.h>
#include <AL/alext.h>
#include <iostream>

Audio::SoundEffectsLibrary* sndbuf = nullptr;

namespace Audio {
	SoundEffectsLibrary* SoundEffectsLibrary::Get(){
		if(sndbuf==nullptr)
			sndbuf = new SoundEffectsLibrary();
		return sndbuf;
	}
	void SoundEffectsLibrary::Terminate(){
		delete sndbuf;
	}

	ALuint SoundEffectsLibrary::Load(const std::string name){
		if (p_SoundEffectBuffers.find(name) != p_SoundEffectBuffers.end())
			return p_SoundEffectBuffers.at(name);

		FileOfSound file = RESOURCE_MANAGER->getSound(name);

		ALenum format=file.getFormat();
		SNDFILE* sndfile= file.getSndFile();
		SF_INFO sfinfo=file.getSfInfo();
		short* membuf=file.getMemBuf();

		ALenum err;
		ALuint buffer;
		sf_count_t num_frames;
		ALsizei num_bytes;

		num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
		if (num_frames < 1){
			free(membuf);
			sf_close(sndfile);
			std::cerr << "(!) Failed to read samples in " << file.getFileName() << "(" << num_frames << ")" << std::endl;
			return 0;
		}
		num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);

		buffer = 0;
		alGenBuffers(1, &buffer);
		alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);

		free(membuf);
		sf_close(sndfile);

		err = alGetError();
		if (err != AL_NO_ERROR){
			std::cerr << "(!) OpenAL Error: " << alGetString(err) << std::endl;
			if (buffer && alIsBuffer(buffer))
				alDeleteBuffers(1, &buffer);
			return 0;
		}

		p_SoundEffectBuffers.emplace(name, buffer);

		return buffer;
	}

	void SoundEffectsLibrary::UnLoad(const std::string name){
		auto It = p_SoundEffectBuffers.find(name);
		if (It != p_SoundEffectBuffers.end()) {
			alDeleteBuffers(1, &It->second);
			p_SoundEffectBuffers.erase(It);
		}
	}

	SoundEffectsLibrary::SoundEffectsLibrary(){
		p_SoundEffectBuffers.clear();
	}

	SoundEffectsLibrary::~SoundEffectsLibrary(){
		auto It = p_SoundEffectBuffers.begin();
		while (It != p_SoundEffectBuffers.end()) {
			alDeleteBuffers(1, &It->second);
			It++;
		}
		p_SoundEffectBuffers.clear();
	}
}
