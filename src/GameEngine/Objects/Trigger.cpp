#include "Trigger.h"

Trigger::Trigger(){
	m_triggerData = TriggerData();
	m_position = glm::vec2(0, 0);
	ShapeIsPoint();
}

Trigger::Trigger(Trigger&& trigger) noexcept{
	m_position = std::move(trigger.m_position);
	m_points = std::move(trigger.m_points);
	m_triggerData = std::move(trigger.m_triggerData);
}

Trigger Trigger::operator=(const Trigger& trigger){
	m_position = trigger.m_position;
	m_points = trigger.m_points;
	m_triggerData = trigger.m_triggerData;
	return *this;
}

Trigger Trigger::operator=(Trigger&& trigger) noexcept{
	m_position = std::move(trigger.m_position);
	m_points = std::move(trigger.m_points);
	m_triggerData = std::move(trigger.m_triggerData);
	return *this;
}

void Trigger::run(Entity& entity){
	m_triggerData.Script(entity);
	if (m_triggerData.Disposable) m_stopWork = true;
}
