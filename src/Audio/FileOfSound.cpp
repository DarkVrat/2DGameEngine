#include "FileOfSound.h"

#include <AL/alext.h>
#include "../Managers/ResourceManager.h"


namespace Audio{ 
	FileOfSound::FileOfSound(const std::string filePath){
		m_fileName =  filePath;
		 
		m_sndFile = sf_open((RESOURCE_MANAGER->getExecutablePath()+filePath).c_str(), SFM_READ, &m_sfInfo);
		if (!m_sndFile)
			std::cerr << "(!) Could not open audio in " << filePath << ": " << sf_strerror(m_sndFile) << std::endl;

		if (m_sfInfo.frames < 1 || m_sfInfo.frames >(sf_count_t)(INT_MAX / sizeof(short)) / m_sfInfo.channels) {
			std::cerr << "(!) Bad sample count in " << filePath << "(" << m_sfInfo.frames << ")" << std::endl;
			sf_close(m_sndFile);
		}

		m_format = AL_NONE;
		switch (m_sfInfo.channels) {
		case 1:
			m_format = AL_FORMAT_MONO16;
			break;
		case 2:
			m_format = AL_FORMAT_STEREO16;
			break;
		case 3:
			if (sf_command(m_sndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
				m_format = AL_FORMAT_BFORMAT2D_16;
			break;
		case 4:
			if (sf_command(m_sndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
				m_format = AL_FORMAT_BFORMAT3D_16;
			break;
		default:
			sf_close(m_sndFile);
			m_sndFile = NULL;
			std::cerr << "(!) Unsupported channel count from file" << std::endl;
		}

		m_memBuf = static_cast<short*>(malloc((size_t)(m_sfInfo.frames * m_sfInfo.channels) * sizeof(short)));
	}

	FileOfSound::FileOfSound(){
		m_sndFile=nullptr;
		m_format=AL_NONE;
		m_memBuf=nullptr;
		m_fileName="";
	}

	SNDFILE* FileOfSound::getSndFile(){
		return m_sndFile;
	}

	SF_INFO FileOfSound::getSfInfo(){
		return m_sfInfo;
	}

	ALenum FileOfSound::getFormat(){
		return m_format;
	}

	short* FileOfSound::getMemBuf(){
		return m_memBuf;
	}

	std::string FileOfSound::getFileName(){
		return m_fileName;
	}

	bool FileOfSound::operator!()
	{
		if (m_sndFile != nullptr) {
			return false; 
		}
		return true;
	}


}