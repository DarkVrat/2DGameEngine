#pragma once

#include "../PhysicsAndLogic/Collision.h"

class Collider : public Collision {
public:
	Collider(const glm::vec2& position = glm::vec2(0, 0)) :Collision(position) {};
	Collider(const glm::vec2& position, const std::vector<glm::vec2>& points) :Collision(position, points) {};
	Collider(const Shape& shape) :Collision(shape) {};
	Collider(const Collision& collision) :Collision(collision) {};
	Collider(const Collider& collider) :Collision(collider.m_position, collider.m_points) {};
	Collider(Collider&& collider) noexcept;

	Collider operator=(const Collider& collider);
	Collider operator=(Collider&& collider);

	friend class CollisionController;
};