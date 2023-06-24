#include "GameUI.h"

#include "../UI/Translater.h"

void GameUI::init() {
	m_coeff = 0.225f * Renderer::Sprite::getWindow().y / 64.f;

	m_sprites.emplace("HUD",		std::make_shared<staticSprite>(RESOURCE_MANAGER::getSprite("HUD"),			glm::vec3(0, 0, 50),						glm::vec2(128 * m_coeff, 64 * m_coeff), 0.f, glm::vec2(0, 0)));
	m_sprites.emplace("mana",		std::make_shared<staticSprite>(RESOURCE_MANAGER::getSprite("mana"),			glm::vec3(3 * m_coeff, 10 * m_coeff, 51), glm::vec2(11 * m_coeff, 14 * m_coeff), 0.f, glm::vec2(0, 0)));
	m_sprites.emplace("healt",		std::make_shared<staticSprite>(RESOURCE_MANAGER::getSprite("healt"),		glm::vec3(114 * m_coeff, 26 * m_coeff, 51), glm::vec2(11 * m_coeff, 14 * m_coeff), 0.f, glm::vec2(0, 0)));
	m_sprites.emplace("projectile", std::make_shared<staticSprite>(RESOURCE_MANAGER::getSprite("projectile_0"), glm::vec3(64 * m_coeff, 32 * m_coeff, 51), glm::vec2(34 * m_coeff, 34 * m_coeff), 0.f, glm::vec2(0.5f, 0.5f)));
} 

void GameUI::render(){
	for (auto& current : m_sprites) {
		current.second->render();
	}

	if (m_dialog && m_dialogs.size() > 0) {
		m_dialogs.at(0).first->render();
	}
}

void GameUI::update(const double& duration){
	glm::vec2& sizeMana = m_sprites.at("mana")->getSize();
	float procentMana = m_MainEntity->getStat("mana") / m_MainEntity->getStat("maxMana");
	sizeMana.y = procentMana * 28 * m_coeff;

	glm::vec2& sizeHealt = m_sprites.at("healt")->getSize();
	float procentHealt = m_MainEntity->getStat("healt") / m_MainEntity->getStat("maxHealt");
	sizeHealt.y = procentHealt * 28 * m_coeff; 

	if (m_dialog&&m_dialogs.size()>0) {
		m_dialogs.at(0).first->update(duration);
		m_dialogs.at(0).second -= duration;
		if (m_dialogs.at(0).second < 0) {
			m_dialogs.erase(m_dialogs.begin());
			if (m_dialogs.size() < 0) {
				m_dialog = false;
			}
		}
	}
}

void GameUI::setMain(std::shared_ptr<Entity> entity){
	m_MainEntity = entity;
}

void GameUI::updateType(){
	switch (m_MainEntity->getManagerProjectile()->getType()){
	case 0:
		m_sprites.at("projectile")=std::make_shared<staticSprite>(RESOURCE_MANAGER::getSprite("projectile_0"), glm::vec3(64 * m_coeff, 32 * m_coeff, 51), glm::vec2(34 * m_coeff, 34 * m_coeff), 0.f, glm::vec2(0.5f, 0.5f));
		break;
	case 1:
		m_sprites.at("projectile")=std::make_shared<staticSprite>(RESOURCE_MANAGER::getSprite("projectile_1"), glm::vec3(64 * m_coeff, 32 * m_coeff, 51), glm::vec2(34 * m_coeff, 34 * m_coeff), 0.f, glm::vec2(0.5f, 0.5f));
		break;
	case 2:
		m_sprites.at("projectile") = std::make_shared<staticSprite>(RESOURCE_MANAGER::getSprite("projectile_2"), glm::vec3(64 * m_coeff, 32 * m_coeff, 51), glm::vec2(34 * m_coeff, 34 * m_coeff), 0.f, glm::vec2(0.5f, 0.5f));
		break;
	case 3:
		m_sprites.at("projectile") = std::make_shared<staticSprite>(RESOURCE_MANAGER::getSprite("projectile_3"), glm::vec3(64 * m_coeff, 32 * m_coeff, 51), glm::vec2(34 * m_coeff, 34 * m_coeff), 0.f, glm::vec2(0.5f, 0.5f));
		break;
	default:
		break;
	}
}

void GameUI::addDialog(const std::string& dialog){
	std::vector<std::shared_ptr<staticSprite>> globalSprite;
	std::vector<std::shared_ptr<staticAnimatedSprite>> globalAnimated;
	std::string path = "res/Dialogs/" + dialog + ".json";
	rapidjson::Document JSONDoc = RESOURCE_MANAGER::loadJSONDoc(path);

	globalSprite = loadSpritesFromDoc(JSONDoc);
	globalAnimated = loadAnimatedFromDoc(JSONDoc);

	auto text = JSONDoc.FindMember("text");
	if (text != JSONDoc.MemberEnd()) {
		for (const auto& currentText : text->value.GetArray()) {
			float time = currentText["time"].GetFloat();

			std::string subjectTranslete = currentText["subjectTranslete"].GetString();
			std::string textForTranslete = currentText["textForTranslete"].GetString();
			float scaleText = currentText["scaleText"].GetFloat();
			float sizeByXScreen = currentText["sizeByXScreen"].GetFloat();
			bool center = currentText["center"].GetBool();
			glm::vec3 position = Position(currentText);
			glm::vec3 color = Color(currentText);

			std::shared_ptr<Renderer::Sprite> backSprite = nullptr;
			float shift = 0;

			std::shared_ptr<staticTextForUI> text = std::make_shared<staticTextForUI>(TEXT(TRANSLETE(subjectTranslete, textForTranslete), position, scaleText, color), sizeByXScreen, center, backSprite, shift);

			m_dialogs.push_back(std::make_pair(std::make_shared<dialogElement>(globalSprite, globalAnimated, text), time));
		}
	}

	m_dialog = true;
}

std::vector<std::shared_ptr<staticSprite>> GameUI::loadSpritesFromDoc(const rapidjson::Document& JSONDoc){
	std::vector<std::shared_ptr<staticSprite>> spritesVec;

	auto global = JSONDoc.FindMember("global");
	if (global == JSONDoc.MemberEnd())return spritesVec;

	auto sprites = global->value.FindMember("staticSprites");
	if (sprites == JSONDoc.MemberEnd())return spritesVec;

	for (const auto& currentSsprite : sprites->value.GetArray()) {
		std::string name = currentSsprite["spriteName"].GetString();
		GLfloat rotation = currentSsprite["rotation"].GetFloat();
		glm::vec3 position = Position(currentSsprite);
		glm::vec2 size = Size(currentSsprite);
		glm::vec2 origin = Origin(currentSsprite);

		spritesVec.push_back(std::make_shared<staticSprite>(RESOURCE_MANAGER::getSprite(name), position, size, rotation, origin));
	}

	return spritesVec;
}

std::vector<std::shared_ptr<staticAnimatedSprite>> GameUI::loadAnimatedFromDoc(const rapidjson::Document& JSONDoc){
	std::vector<std::shared_ptr<staticAnimatedSprite>> AnimVec;

	auto global = JSONDoc.FindMember("global");
	if (global == JSONDoc.MemberEnd())return AnimVec;

	auto sprites = global->value.FindMember("animatedSprite");
	if (sprites == JSONDoc.MemberEnd())return AnimVec;

	for (const auto& currentSsprite : sprites->value.GetArray()) {
		std::string name = currentSsprite["StateName"].GetString();
		GLfloat rotation = currentSsprite["rotation"].GetFloat();
		glm::vec3 position = Position(currentSsprite);
		glm::vec2 size = Size(currentSsprite);
		glm::vec2 origin = Origin(currentSsprite);

		AnimVec.push_back(std::make_shared<staticAnimatedSprite>(name, position, size, rotation, origin));
	}

	return AnimVec;
}

glm::vec3 GameUI::Position(const rapidjson::Value& From) {
	glm::vec3 position(0, 0, 0);
	position.x = From.FindMember("position")->value["x"].GetFloat();
	position.y = From.FindMember("position")->value["y"].GetFloat();
	position.z = From.FindMember("position")->value["z"].GetFloat();
	return position;
}

glm::vec3 GameUI::Color(const rapidjson::Value& From) {
	glm::vec3 color(0, 0, 0);
	color.r = From.FindMember("color")->value["r"].GetFloat();
	color.g = From.FindMember("color")->value["g"].GetFloat();
	color.b = From.FindMember("color")->value["b"].GetFloat();
	return color;
}

glm::vec2 GameUI::Size(const rapidjson::Value& From) {
	glm::vec2 size(0, 0);
	size.x = From.FindMember("size")->value["x"].GetFloat();
	size.y = From.FindMember("size")->value["y"].GetFloat();
	return size;
}

glm::vec2 GameUI::Origin(const rapidjson::Value& From) {
	glm::vec2 origin(0, 0);
	origin.x = From.FindMember("origin")->value["x"].GetFloat();
	origin.y = From.FindMember("origin")->value["y"].GetFloat();
	return origin;
}
