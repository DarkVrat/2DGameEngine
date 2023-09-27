#include "ProjectileSpawner.h"

ProjectileSpawner::ProjectileSpawner(){
	std::function<void(Projectile&, Character&)> functionP = [](Projectile& p, Character& c) {
		c.addDamage(0.1f);
	};

	ProjectileMoveController moveScript;
	moveScript.addMoveInstruction([](Projectile& p, const double& d) {p.UpdateAsEntity(d);}, 10000);

	m_prototype = std::make_shared<Projectile>(ProjectileData(true, true, false, true, 0.25f, 1, functionP, moveScript), EntityData(1, 1, 128, 0, 1), glm::vec2(-1, -1));
	m_prototype->ShapeIsCircle(1.f, 4);
}

ProjectileSpawner::ProjectileSpawner(std::shared_ptr<Projectile> prototype){
	m_prototype = prototype;
}

void ProjectileSpawner::setPrototype(std::shared_ptr<Projectile> prototype){
	m_prototype = prototype;
}

std::shared_ptr<Projectile> ProjectileSpawner::SpawnProjectile(const glm::vec2& position, const glm::vec2& direction){
	std::shared_ptr<Projectile> proj = std::make_shared<Projectile>(m_prototype->CopyProjectile());
	proj->SetPosition(position);
	proj->SetDirection(direction);
	return proj;
}
