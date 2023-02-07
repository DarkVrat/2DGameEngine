#pragma once

#include <AL\al.h>
#include <string>
#include <map>

#define SOUND_LIBRARY Audio::SoundEffectsLibrary//::Get()

namespace Audio {
	class SoundEffectsLibrary{
	public:
		//static SoundEffectsLibrary* Get();
		static void init();
		static void terminate();

		static ALuint load(const std::string name);
		static void unLoad(const std::string name);
	private:
		static std::map<std::string, ALuint> m_SoundEffectBuffers;
	};
}
