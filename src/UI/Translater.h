#pragma once

#include <string>
#include <map>
#include <rapidjson/document.h>

#define TRANSLETE Translater::find

class Translater {
public:

	struct Dictionary {
		std::string ms_RU;
		std::string ms_EN;

		Dictionary(const std::string& EN, const std::string& RU) :ms_EN(EN), ms_RU(RU) {};

		std::string getRU() { return ms_RU; }
		std::string getEN() { return ms_EN; }
	};

	static void init(rapidjson::Document language);

	static std::string find(const std::string& subject, const std::string& text);

	static void setLanguage(const std::string& language);

private:
	static std::map<std::string, std::map<std::string, Dictionary>> m_transleter;
	static std::string m_language;
};