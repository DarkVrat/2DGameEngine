#pragma once

#include <AL\al.h>
#include <string>
#include <map>

#define SOUND_LIBRARY Audio::SoundEffectsLibrary::Get()

namespace Audio {
	class SoundEffectsLibrary{
	public:
		static SoundEffectsLibrary* Get();
		static void Terminate();

		ALuint Load(const std::string name);
		void UnLoad(const std::string name);

	private:
		SoundEffectsLibrary();
		~SoundEffectsLibrary();

		std::map<std::string, ALuint> p_SoundEffectBuffers;
	};
}
