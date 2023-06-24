#include "Item.h"

Item::Item(const glm::vec2& position, const glm::vec2& size, std::shared_ptr<Renderer::Sprite> sprite, std::shared_ptr<PhysicsAndLogic::ICollosion> collision, const std::map<std::string, float>& stats){
	m_position = position;
	m_size = size;
	m_collision = collision;
	m_sprite = sprite;
	m_stats = stats;
	m_collision->setPosition(m_position);
}

void Item::setPosition(const glm::vec2 position){
	m_position = position;
	m_collision->setPosition(m_position);
}

void Item::render(){
	m_sprite->render(glm::vec3(m_position, -1), m_size);
}

void Item::update(const double& duration){
}

std::shared_ptr<Item> Item::copy(){
	return std::make_shared<Item>(m_position, m_size, m_sprite, m_collision->copy(), m_stats);
}

std::shared_ptr<PhysicsAndLogic::ICollosion> Item::getCollision(){
	return m_collision;
}

std::map<std::string, float>& Item::getStats(){
	return m_stats;
}
