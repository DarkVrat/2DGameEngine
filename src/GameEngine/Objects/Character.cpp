#include "Character.h"

#include <glm/glm.hpp>

Character::Character(){
	m_entityData = EntityData();
	m_position = glm::vec2(0, 0);
	ShapeIsPoint();
}

Character::Character(const Character& character){
	m_position = character.m_position;
	m_points = character.m_points;
	m_entityData = character.m_entityData;
	m_way = character.m_way;
}

Character::Character(Character&& character) noexcept{
	m_position = std::move(character.m_position);
	m_points = std::move(character.m_points);
	m_entityData = std::move(character.m_entityData);
	m_way = std::move(character.m_way);
}

Character Character::operator=(const Character& character){
	m_position = character.m_position;
	m_points = character.m_points;
	m_entityData = character.m_entityData;
	m_way = character.m_way;
	return *this;
}

Character Character::operator=(Character&& character) noexcept{
	m_position = std::move(character.m_position);
	m_points = std::move(character.m_points);
	m_entityData = std::move(character.m_entityData);
	m_way = std::move(character.m_way);
	return *this;
}

void Character::CheckCollision(Entity& entity){
	Entity::CheckCollision(entity);
}

void Character::CheckCollision(Character& character){
	Entity::CheckCollision(character);
}

void Character::Update(const double& duration){
	m_way.updateDirection(m_DirectionMove, m_position);
	Entity::Update(duration);
}

void Character::FollowWayTo(const glm::vec2& position){
	m_DirectionMove = m_way.followWayTo(m_position, position);
}
