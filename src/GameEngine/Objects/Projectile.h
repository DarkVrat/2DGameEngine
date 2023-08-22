#pragma once 

#include "Character.h"
#include "../PhysicsAndLogic/ProjectileMoveController.h"
#include <map>
#include <functional>

struct ProjectileData{
	bool m_physics = false;
	bool m_destructible = true;
	bool m_breakableWhenStopped = true;
	bool m_permanentDamage = false;
	bool m_oneTimeUse = true;
	float m_collisionLengthForBreakage = 0.f;
	int8_t m_maxBounceCounter = 0;
	std::function<void(Projectile&, Character&)> m_function = [](Projectile& p, Character& c) {c.addDamage(1);};
	ProjectileMoveController m_moveScript;

	ProjectileData() {}

	ProjectileData(bool physics , bool destructible , bool breakableWhenStopped, bool permanentDamage, bool oneTimeUse , float collisionLengthForBreakage , int8_t bounceCounter , std::function<void(Projectile&, Character&)> functionForProjectile, ProjectileMoveController moveScript)
		:m_physics(physics), 
		m_destructible(destructible), 
		m_breakableWhenStopped(breakableWhenStopped), 
		m_permanentDamage(permanentDamage), 
		m_oneTimeUse(oneTimeUse), 
		m_collisionLengthForBreakage(collisionLengthForBreakage), 
		m_maxBounceCounter(bounceCounter),
		m_function(functionForProjectile),
		m_moveScript(moveScript) {
	}
};

class Projectile : public Entity {
public:
	Projectile() { startMoveController(); };
	Projectile(const ProjectileData& projectileData, const EntityData& entitydata, const glm::vec2& position = glm::vec2(0, 0)) :m_DataProjectire(projectileData), Entity(entitydata, position) { startMoveController(); }
	Projectile(const ProjectileData& projectileData, const EntityData& entitydata, const glm::vec2& position, const std::vector<glm::vec2>& points) :m_DataProjectire(projectileData), Entity(entitydata, position, points) { startMoveController(); }
	Projectile(const ProjectileData& projectileData, const EntityData& entitydata, const Shape& shape) :m_DataProjectire(projectileData), Entity(entitydata, shape) { startMoveController(); }
	Projectile(const ProjectileData& projectileData, const EntityData& entitydata, const Collision& collision) :m_DataProjectire(projectileData), Entity(entitydata, collision) { startMoveController(); }
	Projectile(const ProjectileData& projectileData, const Entity& entity) :m_DataProjectire(projectileData), Entity(entity) { startMoveController(); }
	Projectile(const ProjectileData& projectileData, Entity&& entity) noexcept :m_DataProjectire(projectileData), Entity(std::move(entity)) { startMoveController(); }
	Projectile(const Projectile& projectile);
	Projectile(Projectile&& projectile) noexcept;

	Projectile operator=(const Projectile& projectile);
	Projectile operator=(Projectile&& projectile)noexcept;

	Projectile CopyProjectile();

	void setDataProjectile(const ProjectileData& data);

	void physicsCollision(const Shape& shape);
	void breakageCollision(const Shape& shape);
	void characterCollision(Character& character);

	void Update(const double& duration);
	void UpdateAsEntity(const double& duration);

	bool BounceIsOver();

	friend class CollisionController;
	friend struct ProjectileData;
private:
	void startMoveController();

	ProjectileData m_DataProjectire;

	int8_t m_countBounce = 0;
	std::map<Character*, bool> m_contactWithCharacters;
	MoveScriptController m_controllerMove;
};
