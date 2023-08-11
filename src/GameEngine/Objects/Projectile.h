#pragma once 

#include "Character.h"

struct ProjectileData{
	bool m_physics;
	float m_collisionLengthForBreakage;
	float m_distanceTravel;
	int8_t m_bounceCounter;

	ProjectileData(const bool& physics = false, const float& lengthBreak = 0.f, const float& distance = 512.f, const int8_t& bounce = -1) :
		m_physics(physics), m_collisionLengthForBreakage(lengthBreak), m_distanceTravel(distance), m_bounceCounter(bounce){}
};

class Projectile : public Entity{
public:
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

	void physicsCollision(const Shape& shape);
	void breakageCollision(const Shape& shape);

	void Update(const double& duration);

	friend class CollisionController;
private:
	ProjectileData m_DataProjectire;
};