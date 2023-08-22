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
		m_countBounce++;
	}
}

void Projectile::breakageCollision(const Shape& shape){
	EPAResult result = Collision::CheckCollision(shape);
	if (result.hasCollision && m_DataProjectire.m_collisionLengthForBreakage<glm::length(result.penetrationVector)) {
		m_break = true;
	}
}

void Projectile::characterCollision(Character& character){
	m_DataProjectire.m_function(*this, character);
	m_contactWithCharacters.emplace(&character , true);
}

void Projectile::Update(const double& duration){
	for (auto currentCharacter : m_contactWithCharacters) {
		if (currentCharacter.second) {
			currentCharacter.second = false;
		}
		else {
			m_contactWithCharacters.extract(currentCharacter.first);
		}
	}

	if (m_controllerMove.m_stop == true) {
		return;
	}

	m_controllerMove.m_timeCurrentInstruction += duration;
	if (m_DataProjectire.m_moveScript.UpdateProjectile(*this, m_controllerMove.m_indexInstruction, duration) < m_controllerMove.m_timeCurrentInstruction) {
		m_controllerMove.m_indexInstruction++;
		if (m_controllerMove.m_SizeScript == 0) {
			m_controllerMove.m_stop = true;
			if (m_DataProjectire.m_breakableWhenStopped) {
				m_break = true;
			}
		}
	}
}

void Projectile::UpdateAsEntity(const double& duration){
	Entity::Update(duration);
}

bool Projectile::BounceIsOver(){
	return m_countBounce>=m_DataProjectire.m_maxBounceCounter;
}

void Projectile::startMoveController(){
	m_controllerMove.m_SizeScript = m_DataProjectire.m_moveScript.SizeScript();
	if (m_controllerMove.m_SizeScript == 0) {
		m_DataProjectire.m_moveScript.addMoveInstruction([](Projectile& projectile, const double& duration) {projectile.UpdateAsEntity(duration); }, 10000);
		m_controllerMove.m_SizeScript = 1;
	}
}
