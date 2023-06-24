#pragma once

#include <glm/vec2.hpp>
#include "../../Managers/ResourceManager.h"
#include "../PhysicsAndLogic/ICollision.h"

class Item {
public:
	Item(const glm::vec2& position, const glm::vec2& size, std::shared_ptr<Renderer::Sprite> sprite, std::shared_ptr<PhysicsAndLogic::ICollosion> collision, const std::map<std::string,float>& stats);

	void setPosition(const glm::vec2 position);
	void render();
	void update(const double& duration);
	glm::vec2 getPos() { return m_position; }

	std::shared_ptr<Item> copy();

	std::shared_ptr<PhysicsAndLogic::ICollosion> getCollision();

	std::map<std::string, float>& getStats();
private:
	glm::vec2 m_size;
	glm::vec2 m_position;
	std::map<std::string, float> m_stats;
	std::shared_ptr<Renderer::Sprite> m_sprite;
	std::shared_ptr<PhysicsAndLogic::ICollosion> m_collision;
};