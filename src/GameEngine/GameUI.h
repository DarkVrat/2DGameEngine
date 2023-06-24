#pragma once

#include <memory>
#include <map>
#include "Objects/Entity.h"
#include "dialogElement.h"

class GameUI {
public:
	void init();
	void render();
	void update(const double& duration);
	void setMain(std::shared_ptr<Entity> entity);
	void updateType();

	void setDialog(const bool& flag) { m_dialog = flag; }
	void addDialog(const std::string& dialog);

	std::vector<std::shared_ptr<staticSprite>> loadSpritesFromDoc(const rapidjson::Document& JSONDoc);
	std::vector<std::shared_ptr<staticAnimatedSprite>> loadAnimatedFromDoc(const rapidjson::Document& JSONDoc);
private:
	glm::vec3 Position(const rapidjson::Value& From);
	glm::vec3 Color(const rapidjson::Value& From);
	glm::vec2 Size(const rapidjson::Value& From);
	glm::vec2 Origin(const rapidjson::Value& From);

	std::shared_ptr<Entity> m_MainEntity;
	std::map<std::string, std::shared_ptr<staticSprite>> m_sprites;
	float m_coeff;

	std::vector<std::pair<std::shared_ptr<dialogElement>, float>> m_dialogs;
	bool m_dialog;
};