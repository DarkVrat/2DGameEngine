#include "interactiveObject.h"

InteractiveObject::InteractiveObject(const glm::vec2& position, const glm::vec2& size, std::shared_ptr<Script> script, const std::string& state_first, const std::string& state_second, std::shared_ptr<PhysicsAndLogic::ICollosion> collision, const std::string& nameSound){
	m_position = position;
	m_size = size;
	m_script = script;
	m_state = std::make_shared<StateAnimationController>(state_first);
	m_next_state = state_second;
	m_collision = collision;
	m_completeJob = false;
	m_readyToStart = false;
	m_collision->setPosition(m_position);
	m_sound = nullptr;
	m_sound = MAKE_SOUND_PLAYER(nameSound);
}

void InteractiveObject::render(){
	m_state->render(glm::vec3(m_position, -2), m_size, 0);
}

void InteractiveObject::update(const double& duration){
	m_state->update(duration);
}

std::shared_ptr<PhysicsAndLogic::ICollosion> InteractiveObject::getCollision(){
	return m_collision;
}

void InteractiveObject::runScript(std::shared_ptr<Entity> entity) {
	if (!m_completeJob) {
		m_script->run(entity);
		m_state->setState(m_next_state);
		if (m_sound != nullptr) {
			m_sound->play();
		}
		m_completeJob = true;
	}
} 


