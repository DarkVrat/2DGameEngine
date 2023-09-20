#pragma once

#include "../PhysicsAndLogic/Collision.h"
#include "Trigger.h"
#include "Collider.h"
#include "Object.h"

class Trigger;

struct EntityData {
	EntityData(const float& health = 1.f, const float& mass = 1.f, const float& movementSpeed = 1.f, const float& elasticity = 0.1f, const float& friction=10.f)
		:Health(health), Mass(mass), MovementSpeed(movementSpeed), Elasticity(elasticity),Friction(friction)  {
		if (Health < 0) Health *= -1;
		if (Mass < 0) Mass *= -1;
		if (MovementSpeed < 0) MovementSpeed *= -1;
		if (Friction < 0) Friction *= -1;

		if (Elasticity < 0.01f) Elasticity = 0.01f;
		else if (Elasticity > 1.f) Elasticity = 1.f;
	}

	float Health;
	float Mass;
	float MovementSpeed;
	float Elasticity;
	float Friction;
};

class Entity : public Collision {
public:
	Entity();
	Entity(const EntityData& data, const glm::vec2& position = glm::vec2(0, 0)) :m_entityData(data), Collision(position) {}
	Entity(const EntityData& data, const glm::vec2& position, const std::vector<glm::vec2>& points) :m_entityData(data), Collision(position, points) {}
	Entity(const EntityData& data, const Shape& shape) :m_entityData(data), Collision(shape) {}
	Entity(const EntityData& data, const Collision& collision) :m_entityData(data), Collision(collision) {}
	Entity(const Entity& entity);
	Entity(Entity&& entity) noexcept;

	Entity operator=(const Entity& entity);
	Entity operator=(Entity&& entity)noexcept;

	EntityData& Data() { return m_entityData; }

	void SetDirection(const glm::vec2& direction);
	void SetSpeed(const glm::vec2& speed) { m_CurrentSpeed = speed; }

	void AddImpulse(const glm::vec2& impulse);
	void AddForce(const glm::vec2& force);

	virtual void Update(const double& duration);

	void setSpecial(const bool& flag) { m_Special = flag; }
	bool IsBreak() { return m_break; }
	void SetBreak(const bool& flag) { m_break = flag; }
	int GetTeam() { return m_team; }
	void SetTeam(const int& team) { m_team=team; }

	uint8_t SyncBreak();
	static void UpdateSyncIncrement();

	friend class CollisionController;
protected:
	EntityData m_entityData;

	bool m_Special = false;
	bool m_break=false;
	int m_team = 0;

	static uint8_t m_updateSync;
	uint8_t m_localUpdateSync = m_updateSync;

	glm::vec2 m_DirectionMove = glm::vec2(0, 0);
	glm::vec2 m_CurrentSpeed = glm::vec2(0, 0);
	glm::vec2 m_Acceleration = glm::vec2(0, 0);
};