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

	static void setWindowSize(glm::vec2 windowSize = glm::vec2(800, 600));
	static void setDisplayNumber(int monitorNumber = 0);
	static void setFullScreen(bool fullScreen = true);
	static void setVolumeSounde(double volume = 1.0);
	static void setVolumeSample(std::string name, double volume = 1.0);

	static glm::vec2 getWindowSize();
	static int getDisplayNumber();
	static bool getFullScreen();
	static double getVolumeSounde();
	static double getVolumeSample(std::string name);

private:
	static rapidjson::Document m_configDoc;
};