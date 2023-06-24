#pragma once

#include <glm/vec2.hpp>
#include "../../Managers/ResourceManager.h"
#include "../PhysicsAndLogic/ICollision.h"
#include "../PhysicsAndLogic/Script.h"
#include "../../GameClass/Animation/StateAnimationController.h"
#include "../../Managers/SoundManager.h"

class InteractiveObject {
public:
	InteractiveObject(const glm::vec2& position, const glm::vec2& size, std::shared_ptr<Script> script, const std::string& state_first, const std::string& state_second, std::shared_ptr<PhysicsAndLogic::ICollosion> collision, const std::string& nameSound);

	void render();
	void update(const double& duration);

	std::shared_ptr<PhysicsAndLogic::ICollosion> getCollision();

	void setReady(const bool& flag) { m_readyToStart = flag; }
	bool getReady() { return m_readyToStart; }
	void setCompleteJob(const bool& flag) { m_completeJob = flag; }
	bool getCompleteJob() { return m_completeJob; }

	void runScript(std::shared_ptr<Entity> entity);
private:
	bool m_readyToStart;
	bool m_completeJob;
	glm::vec2 m_size;
	glm::vec2 m_position;
	std::shared_ptr<Script> m_script;
	std::shared_ptr<StateAnimationController> m_state;
	std::shared_ptr<Audio::SoundEffectsPlayer> m_sound;
	std::string m_next_state;
	std::shared_ptr<PhysicsAndLogic::ICollosion> m_collision;
};