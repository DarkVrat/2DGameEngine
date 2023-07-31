#include "Projectile.h"

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

void Projectile::CheckCollision(Object& object){
	if (m_DataProjectire.m_physics) {

	}
	else {

	}
}

void Projectile::CheckCollision(Collider& collider){
}

void Projectile::CheckCollision(Entity& entity){
}

void Projectile::CheckCollision(Character& character){
}
