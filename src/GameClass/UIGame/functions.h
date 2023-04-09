#pragma once

#include <functional>
#include "../MainGameClass.h"
#include "../../UI/Button.h"
#include "../../Managers/SoundManager.h"

class Functions {
public:
	template<class T_Return>
	static std::function<T_Return()> getCallBack(const std::string& nameFunction);

	template<class T_Return, class T_Argument>
	static std::function<T_Return(T_Argument)> getCallBack(const std::string& nameFunction);
};

//----------------------------------------T_Return----------------------------------------//
template<class T_Return>
inline std::function<T_Return()> Functions::getCallBack(const std::string& nameFunction){
	return std::function<T_Return()>();
}

//T_Return=void
template<>
inline std::function<void()> Functions::getCallBack<void>(const std::string& nameFunction) {
	if (nameFunction == "SettingFunction") {
		return [&]() {	
			MainGameClass::getMenu().init("Settings");	
		};
	}
	if (nameFunction == "NewGameFunction") {
		return [&]() {
			MainGameClass::getMenu().init("NewGame");
		};
	}
	if (nameFunction == "ExitFunction") {
		return [&]() {
			MainGameClass::getMenu().init("Exit");
		};
	}
	if (nameFunction == "ExitYes") {
		return [&]() {
			RENDER_ENGINE::closeWindow();
		};
	}
	if (nameFunction == "ExitNo") {
		return [&]() {
			MainGameClass::getMenu().init("MainMenu");
		};
	}
	if (nameFunction == "SaveConfig") {
		return [&]() {
			CONFIG_MANAGER::saveConfig();
			RENDER_ENGINE::applySettings();
			MainGameClass::getMenu().init("MainMenu");
		};
	}
	if (nameFunction == "audioRestart") {
		return [&]() {
			SOUND_MANAGER::updateSoundSystem();
		};
	}
	return std::function<void()>();
}
//----------------------------------------T_Return----------------------------------------//

//----------------------------------T_Return, T_Argument----------------------------------//
template<class T_Return, class T_Argument>
inline std::function<T_Return(T_Argument)> Functions::getCallBack(const std::string& nameFunction) {
	return std::function<T_Return(T_Argument)>();
}

//T_Return=void, T_Argument=float
template<>
inline std::function<void(float)> Functions::getCallBack(const std::string& nameFunction) {
	if (nameFunction == "masterVolume") {
		return [](float value) {
			CONFIG_MANAGER::setVolumeSounde(value);
		};
	}
	if (nameFunction == "UIVolume") {
		return [](float value) {
			CONFIG_MANAGER::setVolumeSample("UI", value);
		};
	}
	if (nameFunction == "MusicVolume") {
		return [](float value) {
			CONFIG_MANAGER::setVolumeSample("Music", value);
		};
	}
	return std::function<void(float)>();
}

//T_Return=void, T_Argument=bool
template<>
inline std::function<void(bool)> Functions::getCallBack(const std::string& nameFunction) {
	if (nameFunction == "fullScreen") {
		return [](bool flag) {
			CONFIG_MANAGER::setFullScreen(flag);
		};
	}
	if (nameFunction == "VSync") {
		return [](bool flag) {
			CONFIG_MANAGER::setVSync(flag);
		};
	}
	return std::function<void(bool flag)>();
}

//T_Return=void, T_Argument=int
template<>
inline std::function<void(int)> Functions::getCallBack(const std::string& nameFunction) {
	if (nameFunction == "samples") {
		return [](int value) {
			CONFIG_MANAGER::setSamples(value);
		};
	}
	if (nameFunction == "display") {
		return [](int value) {
			CONFIG_MANAGER::setDisplayNumber(value);
		};
	}
	return std::function<void(int value)>();
}

//T_Return=void, T_Argument=glm::ivec2
template<>
inline std::function<void(glm::ivec2)> Functions::getCallBack(const std::string& nameFunction) {
	if (nameFunction == "windowSize") {
		return [](glm::ivec2 value) {
			CONFIG_MANAGER::setWindowSize(value.x, value.y);
		};
	}
	return std::function<void(glm::ivec2 value)>();
}

//T_Return=void, T_Argument=std::string
template<>
inline std::function<void(std::string)> Functions::getCallBack(const std::string& nameFunction) {
	if (nameFunction == "language") {
		return [](std::string value) {
			CONFIG_MANAGER::setLanguage(value);
		};
	}
	return std::function<void(std::string value)>();
}
//----------------------------------T_Return, T_Argument----------------------------------//
