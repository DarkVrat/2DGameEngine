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
#include "../UI/Translater.h"

rapidjson::Document ConfigManager::m_configDoc;

 //(RUS) Загрузка файла конфига
//(ENG) Loading a config file
void ConfigManager::loadConfig() { 
	m_configDoc = RESOURCE_MANAGER::loadJSONDoc("res/config.json"); 
}

 //(RUS) сохранение файла конфига
//(ENG) save config file
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

 //(RUS) установка размера окна
//(ENG) setting the window size
void ConfigManager::setWindowSize(const int& x , const int& y) {
	m_configDoc.FindMember("window size")->value["width"].SetInt(x);
	m_configDoc.FindMember("window size")->value["height"].SetInt(y);
}

 //(RUS) установка номера монитора для вывода при полноэкранном режиме
//(ENG) setting the monitor number to display in full screen mode
void ConfigManager::setDisplayNumber(const unsigned& monitorNumber) {
	m_configDoc.FindMember("display")->value.SetInt(monitorNumber); 
}

//(RUS) переключение полноэкранного режима
//(ENG) toggle full screen mode
void ConfigManager::setFullScreen(const bool& fullScreen) {
	m_configDoc.FindMember("full screen")->value.SetBool(fullScreen);
}

 //(RUS) установка общей громкости
//(ENG) setting the overall volume
void ConfigManager::setVolumeSounde(const double& volume) {
	m_configDoc.FindMember("volume")->value.SetDouble(volume);
	SOUND_MANAGER::updateGain();
}

 //(RUS) установка громкости определённых элементов
//(ENG) setting the volume of certain elements
void ConfigManager::setVolumeSample(const std::string& name, const double& volume) {
	for (auto& It : m_configDoc.FindMember("volumeSample")->value.GetArray()) {
		if (It["name"].GetString() == name) {
			It["volume"].SetDouble(volume);
			SOUND_MANAGER::updateGain();
		}
	}
}

void ConfigManager::setSamples(const unsigned& sample){
	m_configDoc.FindMember("samples")->value.SetInt(sample);
}

void ConfigManager::setVSync(const unsigned& VSync){
	m_configDoc.FindMember("VSync")->value.SetBool(VSync);
}

void ConfigManager::setLanguage(const std::string& language){
	rapidjson::Value languageValue;
	languageValue.SetString(language.c_str(), m_configDoc.GetAllocator());
	m_configDoc.FindMember("language")->value = languageValue;
	Translater::setLanguage(language);
}

 //(RUS) получение размера окна
//(ENG) getting window size
glm::ivec2 ConfigManager::getWindowSize() {
	int x = m_configDoc.FindMember("window size")->value["width"].GetInt();
	int y = m_configDoc.FindMember("window size")->value["height"].GetInt();
	return glm::ivec2(x, y);
}

 //(RUS) получение номера монитора
//(ENG) get monitor number
unsigned ConfigManager::getDisplayNumber() {
	return m_configDoc.FindMember("display")->value.GetInt();
}

 //(RUS) получение полноэкранной режима
//(ENG) getting full screen mode
bool ConfigManager::getFullScreen() {
	return m_configDoc.FindMember("full screen")->value.GetBool(); 
}

 //(RUS) получение общей громкости
//(ENG) getting the overall volume
double ConfigManager::getVolumeSounde() {
	return m_configDoc.FindMember("volume")->value.GetDouble(); 
}

 //(RUS) получение громкости определённых элементов
//(ENG) getting the volume of certain elements
double ConfigManager::getVolumeSample(const std::string& name) {
	for (auto& It : m_configDoc.FindMember("volumeSample")->value.GetArray()) {
		if (It["name"].GetString() == name) {
			return It["volume"].GetDouble();
		}
	}
	return 1.0;
}

unsigned ConfigManager::getSamples(){
	return m_configDoc.FindMember("samples")->value.GetInt();
}

unsigned ConfigManager::getVSync(){
	return m_configDoc.FindMember("VSync")->value.GetBool();
}

std::string ConfigManager::getLanguage(){
	return m_configDoc.FindMember("language")->value.GetString();
}
