#pragma once

#include "Entity.h"
#include "../PhysicsAndLogic/WayControlBlock.h"

class ProjectileSpawner;

class Character : public Entity {
public:
	Character();
	Character(const EntityData& data, const glm::vec2& position = glm::vec2(0, 0)) :Entity(data, position) {}
	Character(const EntityData& data, const glm::vec2& position, const std::vector<glm::vec2>& points) :Entity(data, position, points) {}
	Character(const EntityData& data, const Shape& shape) :Entity(data, shape) {}
	Character(const EntityData& data, const Collision& collision) :Entity(data, collision) {}
	Character(const Entity& entity) :Entity(entity) {}
	Character(Entity&& entity) noexcept : Entity(std::move(entity)) {}
	Character(const Character& character);
	Character(Character&& character) noexcept;
	~Character() {if(m_projectileSpawner!=nullptr) delete m_projectileSpawner; }

	Character operator=(const Character& character);
	Character operator=(Character&& character)noexcept;

	void Update(const double& duration);

	void FollowWayTo(const glm::vec2& position);

	float getHealt() { return m_entityData.Health; }
	void addDamage(const float& damage);

	void setSpawner(ProjectileSpawner* spawn) { m_projectileSpawner = spawn; }
	ProjectileSpawner* projectileSpawner() { return m_projectileSpawner; }

	friend class CollisionController;
private:
	WayControlBlock m_way;
	ProjectileSpawner* m_projectileSpawner=nullptr;
};