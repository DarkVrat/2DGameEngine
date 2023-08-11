#include "Projectile.h"

#include <glm/glm.hpp>

Projectile::Projectile(const Projectile& projectile) {
	m_DataProjectire = projectile.m_DataProjectire;
	m_entityData = projectile.m_entityData;
	m_points = projectile.m_points;
}

Projectile::Projectile(Projectile&& projectile) noexcept {
	m_DataProjectire = std::move(projectile.m_DataProjectire);
	m_entityData = std::move(projectile.m_entityData);
	m_points = std::move(projectile.m_points);
}

Projectile Projectile::operator=(const Projectile& projectile){
	m_DataProjectire = projectile.m_DataProjectire;
	m_entityData = projectile.m_entityData;
	m_points = projectile.m_points;
	return *this;
}

Projectile Projectile::operator=(Projectile&& projectile) noexcept{
	m_DataProjectire = std::move(projectile.m_DataProjectire);
	m_entityData = std::move(projectile.m_entityData);
	m_points = std::move(projectile.m_points);
	return *this;
}

Projectile Projectile::CopyProjectile(){
	return Projectile(*this);
}

void Projectile::setDataProjectile(const ProjectileData& data){
	m_DataProjectire = data;
}

void Projectile::physicsCollision(const Shape& shape){
	EPAResult result = Collision::CheckCollision(shape);
	if (result.hasCollision) {
		m_position -= result.penetrationVector;
		float Speed = glm::length(m_CurrentSpeed);
		glm::vec2 reflect = glm::reflect(glm::normalize(m_CurrentSpeed), glm::normalize(result.penetrationVector));
		float e = m_entityData.Elasticity;
		m_CurrentSpeed = Speed * (1.f - e) * reflect;
		m_DataProjectire.m_bounceCounter--;
	}
}

void Projectile::breakageCollision(const Shape& shape){
	EPAResult result = Collision::CheckCollision(shape);
	if (result.hasCollision && m_DataProjectire.m_collisionLengthForBreakage<glm::length(result.penetrationVector)) {
		m_break = true;
	}
}

void Projectile::Update(const double& duration){
	glm::vec2 start = m_position;
	Entity::Update(duration);
	m_DataProjectire.m_distanceTravel -= glm::length(start - m_position);
	if (m_DataProjectire.m_distanceTravel < 0) {
		m_break = true;
	}
}
