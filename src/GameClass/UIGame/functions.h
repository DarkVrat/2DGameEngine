#pragma once

#include <sstream>
#include "../MainGameClass.h"

class Functions {
public:
	template<class T_Return>
	static std::function<T_Return()> getCallBack(const std::string& nameFunction);

	template<class T_Return, class T_Argument>
	static std::function<T_Return(T_Argument)> getCallBack(const std::string& nameFunction);

	static std::vector<std::string> split(std::string str, char delim);
};

std::vector<std::string> Functions::split(std::string str, char delim) {
	while (str.find("  ") != -1) {
		str.replace(str.find("  "), 2, " ");
	}
	while (str.find(" ;") != -1) {
		str.replace(str.find(" ;"), 2, ";");
	}
	std::vector<std::string> result;
	std::stringstream ss(str);
	std::string item;
	while (std::getline(ss, item, delim)) {
		if (item[0] == ' ') {
			item = item.substr(1);
		}
		result.push_back(item);
	}
	return result;
}

//----------------------------------------T_Return----------------------------------------//
template<class T_Return>
inline std::function<T_Return()> Functions::getCallBack(const std::string& nameFunction){
	return std::function<T_Return()>();
}

//T_Return=void
template<>
inline std::function<void()> Functions::getCallBack<void>(const std::string& nameFunction) {
	std::vector<std::function<void()>> vecCommands;
	std::vector<std::string> commands = split(nameFunction, ';');

	for (const auto& cmd : commands) {
		if (cmd.find("call ") == 0) {
			std::string menuName = cmd.substr(5);

			vecCommands.push_back([menuName]() {MAIN_GAME_CLASS::getMenu().init(menuName);});
		}
		else if (cmd.find("load ") == 0) {
			std::string scene = cmd.substr(5);

			vecCommands.push_back([scene]() {MAIN_GAME_CLASS::getScene().init(scene); });
		}
		else if (cmd == "toGame") {
			vecCommands.push_back([]() {MAIN_GAME_CLASS::setGame(true); });
		}
		else if (cmd == "toMenu") {
			vecCommands.push_back([]() {MAIN_GAME_CLASS::setGame(false); });
		}
		else if (cmd == "saveConfig") {
			vecCommands.push_back([]() {CONFIG_MANAGER::saveConfig(); });
		}
		else if (cmd == "applySettings") {
			vecCommands.push_back([]() {RENDER_ENGINE::applySettings(); });
		}
		else if (cmd == "updateSoundSystem") {
			vecCommands.push_back([]() {SOUND_MANAGER::updateSoundSystem(); });
		}
		else if (cmd == "closeWindow") {
			vecCommands.push_back([]() {RENDER_ENGINE::closeWindow(); });
		}
	}

	return [vecCommands]() {
		for (auto& command : vecCommands) {
			command();
		}
	}; 
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
	std::vector<std::function<void(float)>> vecCommands;
	std::vector<std::string> commands = split(nameFunction, ';');

	for (const auto& cmd : commands) {
		if (cmd.find("set ") == 0) {
			std::string nameParam = cmd.substr(4);

			if (nameParam.find("sound ") == 0) {
				std::string argument = nameParam.substr(6);

				if (argument == "master") {
					vecCommands.push_back([](float value) {CONFIG_MANAGER::setVolumeSounde(value); });
				}
				else {
					vecCommands.push_back([argument](float value) {CONFIG_MANAGER::setVolumeSample(argument, value); });
				}
			}
		}
	}

	return [vecCommands](float value) {
		for (auto& command : vecCommands) {
			command(value);
		}
	}; 
}

//T_Return=void, T_Argument=bool
template<>
inline std::function<void(bool)> Functions::getCallBack(const std::string& nameFunction) {
	std::vector<std::function<void(bool)>> vecCommands;
	std::vector<std::string> commands = split(nameFunction, ';');

	for (const auto& cmd : commands) {

		if (cmd.find("set ") == 0) {
			std::string nameParam = cmd.substr(4);

			if (nameParam.find("window ") == 0) {
				std::string argument = nameParam.substr(7);

				if (argument == "fullScreen") {
					vecCommands.push_back([](bool value) {CONFIG_MANAGER::setFullScreen(value); });
				}
				else if (argument == "VSync") {
					vecCommands.push_back([](bool value) {CONFIG_MANAGER::setVSync(value); });
				}
			}
		}
	}

	return [vecCommands](bool value) {
		for (auto& command : vecCommands) {
			command(value);
		}
	};
}

//T_Return=void, T_Argument=int
template<>
inline std::function<void(int)> Functions::getCallBack(const std::string& nameFunction) {
	std::vector<std::function<void(int)>> vecCommands;
	std::vector<std::string> commands = split(nameFunction, ';');

	for (const auto& cmd : commands) {
		if (cmd.find("set ") == 0) {
			std::string nameParam = cmd.substr(4);

			if (nameParam.find("window ") == 0) {
				std::string argument = nameParam.substr(7);

				if (argument == "samples") {
					vecCommands.push_back([](int value) {CONFIG_MANAGER::setSamples(value); });
				}
				else if (argument == "display") {
					vecCommands.push_back([](int value) {CONFIG_MANAGER::setDisplayNumber(value); });	
				}
			}
		}
	}

	return [vecCommands](int value) {
		for (auto& command : vecCommands) {
			command(value);
		}
	};
}

//T_Return=void, T_Argument=glm::ivec2
template<>
inline std::function<void(glm::ivec2)> Functions::getCallBack(const std::string& nameFunction) {
	std::vector<std::function<void(glm::ivec2)>> vecCommands;
	std::vector<std::string> commands = split(nameFunction, ';');

	for (const auto& cmd : commands) {
		if (cmd.find("set ") == 0) {
			std::string nameParam = cmd.substr(4);

			if (nameParam.find("window ") == 0) {
				std::string argument = nameParam.substr(7);

				if (argument == "size") {
					vecCommands.push_back([](glm::ivec2 value) {CONFIG_MANAGER::setWindowSize(value.x, value.y); });
				}
			}
		}
	}

	return [vecCommands](glm::ivec2 value) {
		for (auto& command : vecCommands) {
			command(value);
		} 
	};
}

//T_Return=void, T_Argument=std::string
template<>
inline std::function<void(std::string)> Functions::getCallBack(const std::string& nameFunction) {
	std::vector<std::function<void(std::string)>> vecCommands;
	std::vector<std::string> commands = split(nameFunction, ';');

	for (const auto& cmd : commands) {

		if (cmd.find("set ") == 0) {
			std::string nameParam = cmd.substr(4);

			if (nameParam == "language") {
				vecCommands.push_back([](std::string value) {CONFIG_MANAGER::setLanguage(value); });
			}
		}
	}

	return [vecCommands](std::string value) {
		for (auto& command : vecCommands) {
			command(value);
		}
	};
}
//----------------------------------T_Return, T_Argument----------------------------------//
