#include "SoundManager.h"

Audio::SoundManager* soundManager = nullptr;

namespace Audio {
	SoundManager* SoundManager::Get() {
		if (soundManager == nullptr)
			soundManager = new SoundManager();
		return soundManager;
	}
	void SoundManager::Terminate() {
		delete soundManager;
	}

	void SoundManager::UpdateSoundSystem(){

	}
}