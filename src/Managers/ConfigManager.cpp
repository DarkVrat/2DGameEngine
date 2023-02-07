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


rapidjson::Document ConfigManager::m_configDoc;

void ConfigManager::loadConfig() {
	m_configDoc = RESOURCE_MANAGER::loadJSONDoc("res/config.json");
}

void ConfigManager::saveConfig() {
	std::ofstream f;
	f.open(RESOURCE_MANAGER::getExecutablePath() + "/" + "res/config.json");
	if (!f.is_open()) {
		std::cerr << "(!) Failed to open: res / config.json" << std::endl;
		return;
	}

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	m_configDoc.Accept(writer);

	f << buffer.GetString();
}

void ConfigManager::setWindowSize(glm::vec2 windowSize) {
	m_configDoc.FindMember("window size")->value["width"].SetInt(windowSize.x);
	m_configDoc.FindMember("window size")->value["height"].SetInt(windowSize.y);
}

void ConfigManager::setFullScreen(bool fullScreen) { 
	m_configDoc.FindMember("full screen")->value.SetBool(fullScreen); 
}

void ConfigManager::setDisplayNumber(int monitorNumber) { 
	m_configDoc.FindMember("display")->value.SetInt(monitorNumber); 
}

void ConfigManager::setVolumeSounde(double volume) {
	m_configDoc.FindMember("volume")->value.SetDouble(volume);
	SOUND_MANAGER::updateGain();
}

void ConfigManager::setVolumeSample(std::string name, double volume) {
	for (auto& It : m_configDoc.FindMember("volumeSample")->value.GetArray()) {
		if (It["name"].GetString() == name) {
			It["volume"].SetDouble(volume);
		}
	}
	SOUND_MANAGER::updateGain();
}

glm::vec2 ConfigManager::getWindowSize() {
	int x = m_configDoc.FindMember("window size")->value["width"].GetInt();
	int y = m_configDoc.FindMember("window size")->value["height"].GetInt();
	return glm::vec2(x, y);
}

int ConfigManager::getDisplayNumber() {
	return m_configDoc.FindMember("display")->value.GetInt();
}

bool ConfigManager::getFullScreen() {
	return m_configDoc.FindMember("full screen")->value.GetBool(); 
}

double ConfigManager::getVolumeSounde() {
	return m_configDoc.FindMember("volume")->value.GetDouble(); 
}

double ConfigManager::getVolumeSample(std::string name) {
	for (auto& It : m_configDoc.FindMember("volumeSample")->value.GetArray()) {
		if (It["name"].GetString() == name) {
			return It["volume"].GetDouble();
		}
	}
	return 1.0;
}
