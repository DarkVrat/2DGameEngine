#include "Projectile.h"

#include <iostream>
#include <glm/glm.hpp>

Projectile::Projectile(const Projectile& projectile) {
	m_DataProjectire = projectile.m_DataProjectire;
	m_entityData = projectile.m_entityData;
	m_points = projectile.m_points;
	m_circumradius = projectile.m_circumradius;
	startMoveController();
}

Projectile::Projectile(Projectile&& projectile) noexcept {
	m_DataProjectire = std::move(projectile.m_DataProjectire);
	m_entityData = std::move(projectile.m_entityData);
	m_points = std::move(projectile.m_points);
	m_circumradius = std::move(projectile.m_circumradius);
	startMoveController();
}

Projectile Projectile::operator=(const Projectile& projectile){
	m_DataProjectire = projectile.m_DataProjectire;
	m_entityData = projectile.m_entityData;
	m_points = projectile.m_points;
	m_circumradius = projectile.m_circumradius;
	startMoveController();
	return *this;
}

Projectile Projectile::operator=(Projectile&& projectile) noexcept{
	m_DataProjectire = std::move(projectile.m_DataProjectire);
	m_entityData = std::move(projectile.m_entityData);
	m_points = std::move(projectile.m_points);
	m_circumradius = std::move(projectile.m_circumradius);
	startMoveController();
	return *this;
}

Projectile Projectile::CopyProjectile(){
	return Projectile(*this);
}

void Projectile::setDataProjectile(const ProjectileData& data){
	m_DataProjectire = data;
	startMoveController();
}

void Projectile::physicsCollision(const Shape& shape){
	EPAResult result = Collision::CheckCollision(shape);
	if (result.hasCollision) {
		m_position -= result.penetrationVector;
		float Speed = glm::length(m_CurrentSpeed);
		glm::vec2 reflect = glm::reflect(glm::normalize(m_CurrentSpeed), glm::normalize(result.penetrationVector));

		if (std::isnan(reflect.x)) return;

		float e = m_entityData.Elasticity; 
		m_CurrentSpeed = Speed * (1.f - e) * reflect;
		m_countBounce++;
	}
}

void Projectile::breakageCollision(const Shape& shape){
	if (!m_DataProjectire.m_destructible) { return; }

	EPAResult result = Collision::CheckCollision(shape);
	if (result.hasCollision && m_DataProjectire.m_collisionLengthForBreakage < glm::length(result.penetrationVector))
		m_break = true;
}

void Projectile::characterCollision(Character& character){
	if (m_DataProjectire.m_permanentDamage) {
		m_DataProjectire.m_function(*this, character);
		return;
	}
	else if (m_DataProjectire.m_oneTimeUse && !m_break) {
		m_DataProjectire.m_function(*this, character);
		m_break = true;
		return;
	}

	auto it = m_contactWithCharacters.find(&character);
	if (it == m_contactWithCharacters.end()) {
		m_DataProjectire.m_function(*this, character);
		m_contactWithCharacters.emplace(&character, true);
	}
	else {
		it->second = true;
	}
}

void Projectile::Update(const double& duration){
	if (m_localUpdateSync == m_updateSync) return;
	if (SyncBreak() > 1) {
		m_break = true;
		return;
	}
	m_localUpdateSync = m_updateSync;

	for (auto it = m_contactWithCharacters.begin(); it != m_contactWithCharacters.end();) {
		if (it->second) {
			it->second = false;
			it++;
		}
		else
			it = m_contactWithCharacters.erase(it);
	}

	m_controllerMove.m_timeCurrentInstruction += duration;
	if (m_DataProjectire.m_moveScript.UpdateProjectile(*this, m_controllerMove.m_indexInstruction, duration) < m_controllerMove.m_timeCurrentInstruction) {
		m_controllerMove.m_indexInstruction++;
		m_controllerMove.m_timeCurrentInstruction = 0.0;
		if (m_controllerMove.m_SizeScript <= m_controllerMove.m_indexInstruction) {
			m_break = true;
		}
	}
}

void Projectile::UpdateAsEntity(const double& duration){
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

	m_position += static_cast<float>(duration / 1000) * (m_CurrentSpeed);
}

bool Projectile::BounceIsOver(){
	return m_countBounce>=m_DataProjectire.m_maxBounceCounter;
}

void Projectile::SetDirection(const glm::vec2& direction){
	if (m_DataProjectire.m_physics) {
		m_CurrentSpeed = direction * m_entityData.MovementSpeed;
	}
	m_DirectionMove = direction;
}

void Projectile::startMoveController(){
	m_controllerMove.m_SizeScript = m_DataProjectire.m_moveScript.SizeScript();

	if (m_controllerMove.m_SizeScript == 0) {
		m_DataProjectire.m_moveScript.addMoveInstruction([](Projectile& projectile, const double& duration) {projectile.UpdateAsEntity(duration); }, 10000);
		m_controllerMove.m_SizeScript = 1;
	}
}
