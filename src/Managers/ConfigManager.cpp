#include "ConfigManager.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <rapidjson/error/en.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include "../Audio/SoundDevice.h"
#include "../Managers/SoundManager.h"
#include "../Managers/ResourceManager.h"

static ConfigManager* configManager = nullptr;

ConfigManager* ConfigManager::Get() {
	if (configManager == nullptr)
		configManager = new ConfigManager();
	return configManager;
}

void ConfigManager::loadConfig() {
	const std::string JSONString = RESOURCE_MANAGER->getFileString("res/config.json");

	if (JSONString.empty()) {
		std::cerr << "(!) No JSON resources file" << std::endl;
		return;
	}

	rapidjson::ParseResult parseResult = JSONDoc.Parse(JSONString.c_str());

	if (!parseResult) {
		std::cerr << "(!) JSON parse error: " << rapidjson::GetParseError_En(parseResult.Code()) << "(" << parseResult.Offset() << ")" << std::endl;
		std::cerr << "(!) in JSON resources file: " << std::endl;
		return;
	}
}

void ConfigManager::saveConfig() {
	std::ofstream f;
	f.open(RESOURCE_MANAGER->getExecutablePath() + "/" + "res/config.json");
	if (!f.is_open()) {
		std::cerr << "(!) Failed to open: res / config.json" << std::endl;
		return;
	}

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	JSONDoc.Accept(writer);

	f << buffer.GetString();
}

void ConfigManager::setWindowSize(glm::vec2 windowSize) {
	JSONDoc.FindMember("window size")->value["width"].SetInt(windowSize.x);
	JSONDoc.FindMember("window size")->value["height"].SetInt(windowSize.y);
}

void ConfigManager::setFullScreen(bool fullScreen) { 
	JSONDoc.FindMember("full screen")->value.SetBool(fullScreen); 
}

void ConfigManager::setDisplayNumber(int monitorNumber) { 
	JSONDoc.FindMember("display")->value.SetInt(monitorNumber); 
}

void ConfigManager::setVolumeSounde(double volume) {
	JSONDoc.FindMember("volume")->value.SetDouble(volume);
	SOUND_MANAGER->UpdateGain();
}

void ConfigManager::setVolumeSample(std::string name, double volume) {
	for (auto& It : JSONDoc.FindMember("volumeSample")->value.GetArray()) {
		if (It["name"].GetString() == name) {
			It["volume"].SetDouble(volume);
		}
	}
	SOUND_MANAGER->UpdateGain();
}

glm::vec2 ConfigManager::getWindowSize() {
	int x = JSONDoc.FindMember("window size")->value["width"].GetInt();
	int y = JSONDoc.FindMember("window size")->value["height"].GetInt();
	return glm::vec2(x, y);
}

int ConfigManager::getDisplayNumber() {
	return JSONDoc.FindMember("display")->value.GetInt();
}

bool ConfigManager::getFullScreen() {
	return JSONDoc.FindMember("full screen")->value.GetBool(); 
}

double ConfigManager::getVolumeSounde() {
	return JSONDoc.FindMember("volume")->value.GetDouble(); 
}

double ConfigManager::getVolumeSample(std::string name) {
	for (auto& It : JSONDoc.FindMember("volumeSample")->value.GetArray()) {
		if (It["name"].GetString() == name) {
			return It["volume"].GetDouble();
		}
	}
	return 1.0;
}
