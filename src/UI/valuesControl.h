#pragma once

#include <functional>
#include "../Renderer/RenderEngine.h"
#include "../UI/Translater.h"

class valuesControl {
public:
	template<class T_Type_Vector>
	static std::vector<T_Type_Vector> getList(const std::string& nameFunction);

	template<class T_Argument>
	static std::function<std::string(T_Argument value)> getTypeToString(const std::string& nameFunction);

	template<class T_Type_Start>
	static T_Type_Start getStartValue(const std::string& nameFunction);
};

//----------------------------------------T_Type_Vector----------------------------------------//
template<class T_Type_Vector>
inline std::vector<T_Type_Vector> valuesControl::getList(const std::string& nameFunction)
{
	return std::vector<T_Type_Vector>();
}

//T=int
template<>
inline std::vector<int> valuesControl::getList<int>(const std::string& nameFunction) {
	if (nameFunction == "samples") {
		return std::vector<int>({ 1,2,4,8,16 });
	}
	if (nameFunction == "display") {
		std::vector<int> vec;
		for (int i = 0; i < RENDER_ENGINE::getCountMonitor(); i++) {
			vec.push_back(i);
		}
		return vec;
	}
	return std::vector<int>();
}

//T=glm::ivec2
template<>
inline std::vector<glm::ivec2> valuesControl::getList<glm::ivec2>(const std::string& nameFunction) {
	if (nameFunction == "windowSize") {
		return RENDER_ENGINE::getScreenResolutions();
	}
	return std::vector<glm::ivec2>();
}

//T=std::string
template<>
inline std::vector<std::string> valuesControl::getList<std::string>(const std::string& nameFunction) {
	if (nameFunction == "language") {
		return std::vector<std::string>({ "EN","RU" });
	}
	return std::vector<std::string>();
}
//----------------------------------T_Type_Vector----------------------------------//

//------------------------------------T_Argument-----------------------------------//
template<class T_Argument>
inline std::function<std::string(T_Argument value)> valuesControl::getTypeToString(const std::string& nameFunction)
{
	return std::function<std::string(T_Argument value)>();
}

//T=int
template<>
inline std::function<std::string(int value)> valuesControl::getTypeToString<int>(const std::string& nameFunction) {
	if (nameFunction == "samples") {
		return [](int value) {
			if (value == 1) {
				return TRANSLETE("menu", "no");
			}
			return "x" + std::to_string(value);
		};
	}
	if (nameFunction == "display") {
		return [](int value) {
			return std::to_string(value);
		};
	}
	return std::function<std::string(int value)>();
}

//T=glm::ivec2
template<>
inline std::function<std::string(glm::ivec2 value)> valuesControl::getTypeToString<glm::ivec2>(const std::string& nameFunction) {
	if (nameFunction == "windowSize") {
		return [](glm::ivec2 value) {
			return std::to_string(value.x) + "x" + std::to_string(value.y);
		};
	}
	return std::function<std::string(glm::ivec2 value)>();
}

//T=std::string
template<>
inline std::function<std::string(std::string value)> valuesControl::getTypeToString<std::string>(const std::string& nameFunction) {
	if (nameFunction == "language") {
		return [](std::string value) {
			return value;
		};
	}
	return std::function<std::string(std::string value)>();
}
//------------------------------------T_Argument-----------------------------------//

//-----------------------------------T_Type_Start----------------------------------//
template<class T_Type_Start>
inline T_Type_Start valuesControl::getStartValue(const std::string& nameFunction){
	return T_Type_Start();
}

//T_Type_Start=int
template<>
inline int valuesControl::getStartValue(const std::string& nameFunction) {
	if (nameFunction == "samples") {
		return CONFIG_MANAGER::getSamples();
	}
	if (nameFunction == "display") {
		return CONFIG_MANAGER::getDisplayNumber();
	}
	return 0;
}

//T_Type_Start=glm::ivec2
template<>
inline glm::ivec2 valuesControl::getStartValue(const std::string& nameFunction) {
	if (nameFunction == "windowSize") {
		return CONFIG_MANAGER::getWindowSize();
	}
	return glm::ivec2(0, 0);
}

//T_Type_Start=std::string
template<>
inline std::string valuesControl::getStartValue(const std::string& nameFunction) {
	if (nameFunction == "language") {
		return CONFIG_MANAGER::getLanguage();
	}
	return "";
}

//T_Type_Start=float
template<>
inline float valuesControl::getStartValue(const std::string& nameFunction) {
	if (nameFunction == "masterVolume") {
		return CONFIG_MANAGER::getVolumeSounde();
	}
	if (nameFunction == "UIVolume") {
		return CONFIG_MANAGER::getVolumeSample("UI");
	}
	if (nameFunction == "MusicVolume") {
		return CONFIG_MANAGER::getVolumeSample("Music");
	}
	return 0.0f;
}

//T_Type_Start=bool
template<>
inline bool valuesControl::getStartValue(const std::string& nameFunction) {
	if (nameFunction == "fullScreen") {
		return CONFIG_MANAGER::getFullScreen();
	}
	if (nameFunction == "VSync") {
		return CONFIG_MANAGER::getVSync();
	}
	return false;
}
//-----------------------------------T_Type_Start----------------------------------//

