#pragma once

#include <vector>
#include <glm/vec2.hpp>
#include "../PhysicsAndLogic/ICollision.h"

class AIController {
public:
	AIController(const std::vector<std::pair<glm::vec2, float>>& coordsPosition, std::shared_ptr<PhysicsAndLogic::ICollosion> visionCollision, const bool& chiken);

	std::shared_ptr<PhysicsAndLogic::ICollosion> getCollision() { return m_visionCollision; }
	glm::vec2 getTarget() { return m_target; }
	void update(const float& value);

	void setPosition(const glm::vec2& pos) { m_visionCollision->setPosition(pos); }
	void setTarget(const glm::vec2& target);
	void setAttackMain(const bool& flag) { m_attackMain = flag; }
	bool getAttackMain() { return m_attackMain; }
	void setTargetMain(const bool& flag) { m_targetMain = flag; }
	bool getTargetMain() { return m_targetMain;	}
	bool getChicken() { return m_chicken; }

	void returnToPatrol();

private:
	std::vector<std::pair<glm::vec2, float>> m_CoordsPatrooleAndTime;
	glm::vec2 m_target;
	float m_timer;
	int m_CurrentCoords;

	std::shared_ptr<PhysicsAndLogic::ICollosion> m_visionCollision;
	bool m_chicken;
	bool m_attackMain;
	bool m_targetMain;
};