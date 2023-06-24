#include "AIController.h"

AIController::AIController(const std::vector<std::pair<glm::vec2, float>>& coordsPosition, std::shared_ptr<PhysicsAndLogic::ICollosion> visionCollision, const bool& chiken){
	m_CoordsPatrooleAndTime = coordsPosition;
	m_visionCollision = visionCollision;
	m_chicken = chiken;
	m_CurrentCoords = 0;
	m_attackMain = false;
	m_targetMain = false;
	m_timer = m_CoordsPatrooleAndTime.at(m_CurrentCoords).second;
	m_target = m_CoordsPatrooleAndTime.at(m_CurrentCoords).first;
}

void AIController::update(const float& value){
	m_timer -= value;
	if (m_timer < 0) {
		m_CurrentCoords++;
		if (m_CurrentCoords >= m_CoordsPatrooleAndTime.size()) {m_CurrentCoords = 0;}
		m_timer = m_CoordsPatrooleAndTime.at(m_CurrentCoords).second;
		m_target = m_CoordsPatrooleAndTime.at(m_CurrentCoords).first;
	}
}

void AIController::setTarget(const glm::vec2& target){
	m_target = target;
	m_timer = 0.f;
}

void AIController::returnToPatrol(){
	m_timer = m_CoordsPatrooleAndTime.at(m_CurrentCoords).second;
	m_target = m_CoordsPatrooleAndTime.at(m_CurrentCoords).first;
}
