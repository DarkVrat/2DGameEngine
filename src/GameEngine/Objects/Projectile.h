#pragma once 

#include "Character.h"

struct ProjectileData{
	bool m_physics = false;
	bool m_breakOnCollision = false;
	float m_collisionLengthForBreakage = 0.f;
	float m_distanceTravel = 512.f;
	int8_t m_bounceCounter = -1;
};

class Projectile : public Entity{
	Projectile();
	Projectile(const EntityData& data, const glm::vec2& position = glm::vec2(0, 0)) :Entity(data, position) {}
	Projectile(const EntityData& data, const glm::vec2& position, const std::vector<glm::vec2>& points) :Entity(data, position, points) {}
	Projectile(const EntityData& data, const Shape& shape) :Entity(data, shape) {}
	Projectile(const EntityData& data, const Collision& collision) :Entity(data, collision) {}
	Projectile(const Entity& entity) :Entity(entity) {}
	Projectile(Entity&& entity) noexcept : Entity(std::move(entity)) {}
	Projectile(const Projectile& projectile);
	Projectile(Projectile&& projectile) noexcept;

	Projectile operator=(const Projectile& projectile);
	Projectile operator=(Projectile&& projectile)noexcept;

	Projectile CopyProjectile();

	void setDataProjectile(const ProjectileData& data);

	void CheckCollision(Object& object);
	void CheckCollision(Collider& collider);
	void CheckCollision(Entity& entity);
	void CheckCollision(Character& character);
private:
	ProjectileData m_DataProjectire;
};