#pragma once

#include <string>
#include <rapidjson/document.h>
#include <glm/vec2.hpp>

#define CONFIG_MANAGER ConfigManager

 //(RUS) Необходим для работы с файлом "res/config.json", загрузки, изменения и сохранения
//(ENG) Required to work with the "res/config.json" file, load, modify and save

class ConfigManager {
public:
	static void loadConfig();
	static void saveConfig();

	static void setWindowSize(const int& x=800, const int& y = 600);
	static void setDisplayNumber(const unsigned& monitorNumber = 0);
	static void setFullScreen(const bool& fullScreen = true);
	static void setVolumeSounde(const double& volume = 1.0);
	static void setVolumeSample(const std::string& name, const double& volume = 1.0);
	static void setSamples(const unsigned& sample = 1);

	static glm::ivec2 getWindowSize();
	static unsigned getDisplayNumber();
	static bool getFullScreen();
	static double getVolumeSounde();
	static double getVolumeSample(const std::string& name);
	static unsigned getSamples();

private:
	static rapidjson::Document m_configDoc;
};