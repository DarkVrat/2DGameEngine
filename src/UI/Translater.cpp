#include "Translater.h"

#include "../Managers/ResourceManager.h"

std::map<std::string, std::map<std::string, Translater::Dictionary>> Translater::m_transleter;
std::string Translater::m_language;

void Translater::init(rapidjson::Document language){
	auto subject = language.FindMember("subject");
	if (subject != language.MemberEnd()) {
		for (const auto& currentSubject : subject->value.GetArray()) {

			std::map<std::string, Dictionary> dictionSub;
			for (const auto& current : currentSubject["dictionary"].GetArray()) {
				dictionSub.emplace(current["name"].GetString(), Dictionary(current["EN"].GetString(), current["RU"].GetString()));
			}

			m_transleter.emplace(currentSubject["nameSubject"].GetString(), dictionSub);
		}
	}

	m_language = "EN";
}

std::string Translater::find(const std::string& subject, const std::string& text){
	if(m_language=="RU")
		return m_transleter.at(subject).at(text).getRU();
	if (m_language == "EN")
		return m_transleter.at(subject).at(text).getEN();
	return text;
}

void Translater::setLanguage(const std::string& language){
	m_language = language;
}
