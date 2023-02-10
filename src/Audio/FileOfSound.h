#pragma once

#include <iostream>
#include <sndfile.h>
#include <AL/al.h>

 //(RUS) необходим для загрузки аудиофайлов, и передачи их в Audio::SoundEffectLibrary
//(ENG) needed to load audio files and pass them to Audio::SoundEffectLibrary

namespace Audio {
	class FileOfSound {
	public:
		FileOfSound(const std::string& filePath);

		SNDFILE* getSndFile();
		SF_INFO getSfInfo();
		ALenum getFormat();
		short* getMemBuf();
		std::string getFileName();

	private:
		SNDFILE* m_sndFile;
		SF_INFO m_sfInfo;
		ALenum m_format;
		short* m_memBuf;
		std::string m_fileName;
	};
}