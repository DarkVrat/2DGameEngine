#pragma once

#include "../PhysicsAndLogic/Collision.h"
#include "Entity.h"
#include <functional>

class Entity;

struct TriggerData{
	TriggerData(const bool& disposible = true, const bool& forMainEntity = true, const std::function<void(Entity& entity, const double& duration)>& script = [](Entity& entity, const double& duration){})
		:Disposable(disposible), ForMainEntity(forMainEntity), Script(script) {};

	bool Disposable;
	bool ForMainEntity;
	std::function<void(Entity& entity, const double& duration)> Script;
};

class Trigger : public Collision {
public:
	Trigger();
	Trigger(const TriggerData& triggerData, const glm::vec2& position = glm::vec2(0, 0)) :m_triggerData(triggerData), Collision(position) {};
	Trigger(const TriggerData& triggerData, const glm::vec2& position, const std::vector<glm::vec2>& points) :m_triggerData(triggerData), Collision(position, points) {};
	Trigger(const TriggerData& triggerData, const Shape& shape) :m_triggerData(triggerData), Collision(shape) {};
	Trigger(const TriggerData& triggerData, const Collision& collision) :m_triggerData(triggerData), Collision(collision) {};
	Trigger(const Trigger& trigger) :m_triggerData(trigger.m_triggerData), Collision(trigger.m_position, trigger.m_points) {};
	Trigger(Trigger&& trigger) noexcept;

	Trigger operator=(const Trigger& trigger);
	Trigger operator=(Trigger&& trigger) noexcept;

	void run(Entity& entity, const double& duration) const;

	TriggerData& Data() { return m_triggerData; }

private:
	TriggerData m_triggerData;
};