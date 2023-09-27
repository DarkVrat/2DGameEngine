#pragma once

#include "../Objects/Projectile.h"

class ProjectileSpawner {
public:
	ProjectileSpawner();
	ProjectileSpawner(std::shared_ptr<Projectile> prototype);
	void setPrototype(std::shared_ptr<Projectile> prototype);
	std::shared_ptr<Projectile> SpawnProjectile(const glm::vec2& position, const glm::vec2& direction);
private:
	std::shared_ptr<Projectile> m_prototype;
};