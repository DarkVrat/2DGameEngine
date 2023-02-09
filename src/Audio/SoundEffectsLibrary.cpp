#include "SoundEffectsLibrary.h"

#include <iostream>
#include <sndfile.h>
#include <inttypes.h>
#include <AL/alext.h>
#include "FileOfSound.h"
#include "../Managers/ResourceManager.h"

Audio::SoundEffectsLibrary* sndbuf = nullptr;

std::map<std::string, ALuint> Audio::SoundEffectsLibrary::m_SoundEffectBuffers;

namespace Audio {
	   //(RUS) создание буфера со звуком из FileOfSound, пересоздание FileOfSound в ресурс менеджере, 
	  //       и передача ID  буфера если звук уже загружен
	 //(ENG) creating a buffer with sound from FileOfSound, recreating FileOfSound in the resource manager, 
	//       and passing the buffer ID if the sound is already loaded
	ALuint SoundEffectsLibrary::load(const std::string name){
		if (m_SoundEffectBuffers.find(name) != m_SoundEffectBuffers.end())
			return m_SoundEffectBuffers.at(name);

		std::shared_ptr<FileOfSound> file = RESOURCE_MANAGER::getSound(name);

		ALenum format=file->getFormat();
		SNDFILE* sndfile= file->getSndFile();
		SF_INFO sfinfo=file->getSfInfo();
		short* membuf=file->getMemBuf();

		ALenum err;
		ALuint buffer;
		sf_count_t num_frames;
		ALsizei num_bytes;

		num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
		if (num_frames < 1){
			free(membuf);
			sf_close(sndfile);
			std::cerr << "(!) Failed to read samples in " << file->getFileName() << "(" << num_frames << ")" << std::endl;
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

		m_SoundEffectBuffers.emplace(name, buffer);

		RESOURCE_MANAGER::loadSound(name, file->getFileName());

		return buffer;
	}

	 //(RUS) Удаление звукового буфера
	//(ENG) Deleting the sound buffer
	void SoundEffectsLibrary::unLoad(const std::string name){
		auto It = m_SoundEffectBuffers.find(name);
		if (It != m_SoundEffectBuffers.end()) {
			alDeleteBuffers(1, &It->second);
			m_SoundEffectBuffers.erase(It);
		}
	}
	
	 //(RUS) Инициализация библиотеки
	//(ENG) Library initialization
	void SoundEffectsLibrary::init(){
		m_SoundEffectBuffers.clear();
	}

	 //(RUS) Очистка буферов
	//(ENG) Clearing buffers
	void SoundEffectsLibrary::terminate(){
		auto It = m_SoundEffectBuffers.begin();
		while (It != m_SoundEffectBuffers.end()) {
			alDeleteBuffers(1, &It->second);
			It++;
		}
		m_SoundEffectBuffers.clear();
	}
}
