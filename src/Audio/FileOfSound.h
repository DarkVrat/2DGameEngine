#pragma once
#include <sndfile.h>
#include <AL/al.h>
#include <AL/alext.h>
#include <iostream>

namespace Audio {
	class FileOfSound {
	public:
		FileOfSound(const std::string filePath);
		FileOfSound();

		SNDFILE* getSndFile() { return m_sndFile; }
		SF_INFO getSfInfo() { return m_sfInfo; }
		ALenum getFormat() { return m_format; }
		short* getMemBuf() { return m_memBuf; }
		std::string getFileName() { return m_fileName; }

		bool operator!();
	private:
		SNDFILE* m_sndFile;
		SF_INFO m_sfInfo;
		ALenum m_format;
		short* m_memBuf;
		std::string m_fileName;
	};
}