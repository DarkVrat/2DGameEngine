#pragma once

#include "../PhysicsAndLogic/Collision.h"

struct ObjectData {
	ObjectData(const bool& breakable = false, const float& health = 1.f, const float& elasticity = 1.f, const float& friction = 1.f)
		:Breakable(breakable), Health(health), Elasticity(elasticity), Friction(friction){}

	bool Breakable;
	float Health;
	float Elasticity;
	float Friction;
};

class Object : public Collision {
public:
	Object();
	Object(const ObjectData& data, const glm::vec2& position = glm::vec2(0, 0)) :m_objectData(data), Collision(position) {};
	Object(const ObjectData& data, const glm::vec2& position, const std::vector<glm::vec2>& points) :m_objectData(data), Collision(position, points) {};
	Object(const ObjectData& data, const Shape& shape) :m_objectData(data), Collision(shape) {};
	Object(const ObjectData& data, const Collision& collision) :m_objectData(data), Collision(collision) {};
	Object(const Object& object) :m_objectData(object.m_objectData), Collision(object.m_position, object.m_points) {};
	Object(Object&& object) noexcept;

	Object operator=(const Object& object);
	Object operator=(Object&& object)noexcept;

	ObjectData& Data() { return m_objectData; }

protected:
	ObjectData m_objectData;
};