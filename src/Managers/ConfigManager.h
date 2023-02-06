#pragma once

#include <string>
#include <rapidjson/document.h>
#include <glm/vec2.hpp>

#define CONFIG_MANAGER ConfigManager::Get()

class ConfigManager {
public:
	static ConfigManager* Get();

	void loadConfig();
	void saveConfig();

	void setWindowSize(glm::vec2 windowSize = glm::vec2(800, 600));
	void setDisplayNumber(int monitorNumber = 0);
	void setFullScreen(bool fullScreen = true);
	void setVolumeSounde(double volume = 1.0);
	void setVolumeSample(std::string name, double volume = 1.0);

	glm::vec2 getWindowSize();
	int getDisplayNumber();
	bool getFullScreen();
	double getVolumeSounde();
	double getVolumeSample(std::string name);

private:
	rapidjson::Document JSONDoc;
};