#include "Entity.h"

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

void Entity::CheckCollision(const Trigger& trigger, const double& duration){
	if (Collision::CheckCollision(trigger).hasCollision) {
		trigger.run(*this, duration);
	}
}

void Entity::CheckCollision(const Collider& collider){
	EPAResult result = Collision::CheckCollision(collider);
	if (result.hasCollision) {
		Move(-result.penetrationVector);
	}
}

void Entity::CheckCollision(const Object& object){
	EPAResult result = Collision::CheckCollision(object);
	if (result.hasCollision) {
		Move(-result.penetrationVector);
	}
}

void Entity::CheckCollision(Entity& entity){
	EPAResult result = Collision::CheckCollision(entity);
	if (result.hasCollision) {
		float ration = m_entityData.Mass / (m_entityData.Mass + entity.m_entityData.Mass);
		Move(-result.penetrationVector*(1-ration));
		entity.Move(result.penetrationVector * ration);
	}
}
