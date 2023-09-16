#include "Entity.h"

#include <iostream>
#include <glm/gtx/norm.hpp>

uint8_t Entity::m_updateSync = 0;

Entity::Entity(){
	m_entityData = EntityData();
	m_position = glm::vec2(0, 0);
	ShapeIsPoint();
}

Entity::Entity(const Entity& entity){
	m_position = entity.m_position;
	m_points = entity.m_points;
	m_circumradius = entity.m_circumradius;
	m_entityData = entity.m_entityData;
}

Entity::Entity(Entity&& entity) noexcept{
	m_position = std::move(entity.m_position);
	m_points = std::move(entity.m_points);
	m_circumradius = std::move(entity.m_circumradius);
	m_entityData = std::move(entity.m_entityData);
}

Entity Entity::operator=(const Entity& entity){ 
	m_position = entity.m_position;
	m_points = entity.m_points;
	m_circumradius = entity.m_circumradius;
	m_entityData = entity.m_entityData;
	return *this;
}

Entity Entity::operator=(Entity&& entity)noexcept {
	m_position = std::move(entity.m_position);
	m_points = std::move(entity.m_points);
	m_circumradius = std::move(entity.m_circumradius);
	m_entityData = std::move(entity.m_entityData);
	return *this;
}

void Entity::SetDirection(const glm::vec2& direction){
	m_DirectionMove = direction;
	if ((m_DirectionMove.x > 0.1f || m_DirectionMove.x < -0.1f) && (m_DirectionMove.y > 0.1f || m_DirectionMove.y < -0.1f)) {
		m_DirectionMove = glm::normalize(m_DirectionMove);
	}
}

void Entity::AddImpulse(const glm::vec2& impulse){
	m_CurrentSpeed += impulse / m_entityData.Mass;
}

void Entity::AddForce(const glm::vec2& force){
	m_Acceleration += force / m_entityData.Mass;
}

void Entity::Update(const double& duration){
	if (m_localUpdateSync == m_updateSync) {
		return;
	}

	if (SyncBreak() > 1) {
		std::cout << this << ": " << static_cast<int>(m_localUpdateSync) << "\t" << static_cast<int>(m_updateSync) << std::endl;
	}

	m_CurrentSpeed += m_Acceleration * static_cast<float>(duration / 1000);
	m_Acceleration = glm::vec2(0, 0);
	
	float lenghtSpeed = glm::length(m_CurrentSpeed);
	if (lenghtSpeed > 0.00001) {
		glm::vec2 friction = -glm::normalize(m_CurrentSpeed) * m_entityData.Friction * static_cast<float>(duration / 1000);

		if (glm::length(friction) > lenghtSpeed)
			m_CurrentSpeed = glm::vec2(0, 0);
		else
			m_CurrentSpeed += friction;
	}

	m_position += static_cast<float>(duration / 1000) * (m_entityData.MovementSpeed * m_DirectionMove + m_CurrentSpeed);
	
	m_localUpdateSync = m_updateSync;
}

uint8_t Entity::SyncBreak(){
	return m_updateSync-m_localUpdateSync;
}

void Entity::UpdateSyncIncrement(){
	m_updateSync++;
}
