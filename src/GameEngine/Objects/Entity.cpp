#include "Entity.h"

#include <iostream>
#include <glm/gtx/norm.hpp>


Entity::Entity(){
	m_entityData = EntityData();
	m_position = glm::vec2(0, 0);
	ShapeIsPoint();
}

Entity::Entity(Entity&& entity) noexcept{
	m_position = std::move(entity.m_position);
	m_points = std::move(entity.m_points);
	m_entityData = std::move(entity.m_entityData);
}

Entity Entity::operator=(const Entity& entity){ 
	m_position = entity.m_position;
	m_points = entity.m_points;
	m_entityData = entity.m_entityData;
	return *this;
}

Entity Entity::operator=(Entity&& entity)noexcept {
	m_position = std::move(entity.m_position);
	m_points = std::move(entity.m_points);
	m_entityData = std::move(entity.m_entityData);
	return *this;
}

void Entity::CheckCollision(Trigger& trigger, const double& duration){
	if (Collision::CheckCollision(trigger).hasCollision) {
		trigger.run(*this, duration);
	}
}

void Entity::CheckCollision(Collider& collider){
	EPAResult result = Collision::CheckCollision(collider);
	if (result.hasCollision) {
		m_position -= result.penetrationVector;
	}
}

void Entity::CheckCollision( Object& object){
	EPAResult result = Collision::CheckCollision(object);
	if (result.hasCollision) {
		m_position -= result.penetrationVector;

		float Speed = glm::length(m_CurrentSpeed);
		if (Speed > 0.0001f) {
			glm::vec2 reflect = glm::reflect(glm::normalize(m_CurrentSpeed), glm::normalize(result.penetrationVector));
			float e = (m_entityData.Elasticity + object.Data().Elasticity) / 2.f;
			m_CurrentSpeed = Speed * (1.f - e) * reflect;
		}
	}
}

void Entity::CheckCollision(Entity& entity){
	EPAResult result = Collision::CheckCollision(entity);
	if (result.hasCollision) {
		float ration = m_entityData.Mass / (m_entityData.Mass + entity.m_entityData.Mass);

		m_position -= result.penetrationVector * (1 - ration);
		entity.m_position += result.penetrationVector * ration;

		glm::vec2 normal = glm::normalize(result.penetrationVector);
		float resultDot = glm::dot(entity.m_CurrentSpeed - m_CurrentSpeed, normal);

		if (resultDot <= 0) {
			float e = (m_entityData.Elasticity + entity.m_entityData.Elasticity) / 2.f;

			glm::vec2 impulse = -(1 + e) * resultDot * normal / (1 / m_entityData.Mass + 1 / entity.m_entityData.Mass);
			AddImpulse(-impulse);
			entity.AddImpulse(impulse);
		}
	}
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
}
