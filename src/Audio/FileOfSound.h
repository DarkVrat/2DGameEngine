#pragma once

#include <iostream>
#include <sndfile.h>
#include <AL/al.h>

namespace Audio {
	class FileOfSound {
	public:
		FileOfSound(const std::string filePath);
		FileOfSound();

		SNDFILE* getSndFile();
		SF_INFO getSfInfo();
		ALenum getFormat();
		short* getMemBuf();
		std::string getFileName();

		bool operator!();

	private:
		SNDFILE* m_sndFile;
		SF_INFO m_sfInfo;
		ALenum m_format;
		short* m_memBuf;
		std::string m_fileName;
	};
}