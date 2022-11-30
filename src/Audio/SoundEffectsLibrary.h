#pragma once
#include <AL\al.h>
#include <iostream>
#include <map>
#include"FileOfSound.h"

#define GETLIB Audio::SoundEffectsLibrary::Get();

namespace Audio {
	class SoundEffectsLibrary{
	public:
		static SoundEffectsLibrary* Get();

		ALuint Load(const std::string name);
		void UnLoad(const std::string name);

	private:
		SoundEffectsLibrary();
		~SoundEffectsLibrary();

		std::map<std::string, ALuint> p_SoundEffectBuffers;
	};
}
