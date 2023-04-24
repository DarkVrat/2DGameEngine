#pragma once

#include <functional>
#include "../GameClass/UIGame/functions.h"

class valuesControl {
public:
	static std::string format(std::string str);

	template<class T_Type_Vector>
	static std::vector<T_Type_Vector> getList(const std::string& startValue);

	template<class T_Argument>
	static std::function<std::string(T_Argument value)> getTypeToString(const std::string& startValue);

	template<class T_Type_Start>
	static T_Type_Start getStartValue(const std::string& startValue);
};

std::string valuesControl::format(std::string str){
	while (str.find("  ") != -1) { 
		str.replace(str.find("  "), 2, " ");
	}
	while (str[str.length() - 1] == ' ' || str[str.length() - 1] == ';') {
		str = str.substr(0, str.length() - 1);
	}
	if (str[0] == ' ') {
		str = str.substr(1);
	}

	return str;
}

//----------------------------------------T_Type_Vector----------------------------------------//
template<class T_Type_Vector>
inline std::vector<T_Type_Vector> valuesControl::getList(const std::string& startValue){
	return std::vector<T_Type_Vector>();
}

//T=int
template<>
inline std::vector<int> valuesControl::getList<int>(const std::string& startValue) {
	std::string str = format(startValue);
	if (str.find("window ") == 0) {
		std::string nameParam = str.substr(7);

		if (nameParam == "samples") {
			return std::vector<int>({ 1,2,4,8,16 });
		}
		else if (nameParam == "display") {
			std::vector<int> vec;
			for (int i = 0; i < RENDER_ENGINE::getCountMonitor(); i++) {
				vec.push_back(i);
			}
			return vec;
		}
	}
	return std::vector<int>();
}

//T=glm::ivec2
template<>
inline std::vector<glm::ivec2> valuesControl::getList<glm::ivec2>(const std::string& startValue) {
	std::string str = format(startValue);

	if (str.find("window ") == 0) {
		std::string nameParam = str.substr(7);

		if (nameParam == "size") {
			return RENDER_ENGINE::getScreenResolutions();
		}
	}
	return std::vector<glm::ivec2>();
}

//T=std::string
template<>
inline std::vector<std::string> valuesControl::getList<std::string>(const std::string& startValue) {
	std::string str = format(startValue);

	if (str == "language") {
		return std::vector<std::string>({ "EN","RU" });
	}

	return std::vector<std::string>();
}
//----------------------------------T_Type_Vector----------------------------------//

//------------------------------------T_Argument-----------------------------------//
template<class T_Argument>
inline std::function<std::string(T_Argument value)> valuesControl::getTypeToString(const std::string& startValue)
{
	return std::function<std::string(T_Argument value)>();
}

//T=int
template<>
inline std::function<std::string(int value)> valuesControl::getTypeToString<int>(const std::string& startValue) {
	std::string str = format(startValue);

	if (str.find("window ") == 0) {
		std::string nameParam = str.substr(7);

		if (nameParam == "samples") {
			return [](int value) {
				if (value == 1) {
					return TRANSLETE("menu", "no");
				}
				return "x" + std::to_string(value);
			};
		}
		else if (nameParam == "display") {
			return [](int value) {
				return std::to_string(value);
			};
		}
	}
	return std::function<std::string(int value)>();
}

//T=glm::ivec2
template<>
inline std::function<std::string(glm::ivec2 value)> valuesControl::getTypeToString<glm::ivec2>(const std::string& startValue) {
	std::string str = format(startValue);

	if (str.find("window ") == 0) {
		std::string nameParam = str.substr(7);

		if (nameParam == "size") {
			return [](glm::ivec2 value) {
				return std::to_string(value.x) + "x" + std::to_string(value.y);
			};
		}
	}

	return std::function<std::string(glm::ivec2 value)>();
}

//T=std::string
template<>
inline std::function<std::string(std::string value)> valuesControl::getTypeToString<std::string>(const std::string& startValue) {
	std::string str = format(startValue);

	if (str == "language") {
		return [](std::string value) {
			return value;
		};
	}

	return std::function<std::string(std::string value)>();
}
//------------------------------------T_Argument-----------------------------------//

//-----------------------------------T_Type_Start----------------------------------//
template<class T_Type_Start>
inline T_Type_Start valuesControl::getStartValue(const std::string& startValue){
	return T_Type_Start();
}

//T_Type_Start=int
template<>
inline int valuesControl::getStartValue(const std::string& startValue) {
	std::string str = format(startValue);

	if (str.find("window ") == 0) {
		std::string nameParam = str.substr(7);

		if (nameParam == "samples") {
			return CONFIG_MANAGER::getSamples();
		}
		else if (nameParam == "display") {
			return CONFIG_MANAGER::getDisplayNumber();
		}
	}

	return 0;
}

//T_Type_Start=glm::ivec2
template<>
inline glm::ivec2 valuesControl::getStartValue(const std::string& startValue) {
	std::string str = format(startValue);

	if (str.find("window ") == 0) {
		std::string nameParam = str.substr(7);

		if (nameParam == "size") {
			return CONFIG_MANAGER::getWindowSize();
		}
	}
	return glm::ivec2(0, 0);
}

//T_Type_Start=std::string
template<>
inline std::string valuesControl::getStartValue(const std::string& startValue) {
	std::string str = format(startValue);

	if (str == "language") {
		return CONFIG_MANAGER::getLanguage();
	}

	return "";
}

//T_Type_Start=float
template<>
inline float valuesControl::getStartValue(const std::string& startValue) {
	std::string str = format(startValue);

	if (str.find("sound ") == 0) {
		std::string nameParam = str.substr(6);

		if (nameParam == "master") {
			return CONFIG_MANAGER::getVolumeSounde();
		}
		else {
			return CONFIG_MANAGER::getVolumeSample(nameParam);
		}
	}

	return 0.0f;
}

//T_Type_Start=bool
template<>
inline bool valuesControl::getStartValue(const std::string& startValue) {
	std::string str = format(startValue);

	if (str.find("window ") == 0) {
		std::string nameParam = str.substr(7);

		if (nameParam == "fullScreen") {
			return CONFIG_MANAGER::getFullScreen();
		}
		else if (nameParam == "VSync") {
			return CONFIG_MANAGER::getVSync(); 
		}
	}

	return false;
}
//-----------------------------------T_Type_Start----------------------------------//

