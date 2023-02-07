#include "StateController.h"

StateController::StateController(const std::string initState){
	m_nameState=initState;
	m_state = RESOURCE_MANAGER::getStateAnimation(m_nameState);
	m_bufferState = m_state->getNextStateName();
	m_sprites = m_state->getFrames();
	m_indexFrame = 0;
	m_durationFrame = m_sprites[m_indexFrame].second;
	m_time=0;
}

void StateController::setState(const std::string nextState){
	if (m_indexFrame >= m_sprites.size()) {
		if (nextState == m_nameState) {
			m_indexFrame = 0;
			m_durationFrame = m_sprites[m_indexFrame].second;
		}
		else {
			std::shared_ptr<Renderer::StateAnimation> newState = RESOURCE_MANAGER::getStateAnimation(nextState);
			m_nameState = nextState;
			m_state = newState;
			m_bufferState = newState->getNextStateName();
			m_sprites = newState->getFrames();
			m_indexFrame = 0;
			m_durationFrame = m_sprites[m_indexFrame].second;
		}
	}
	else {
		if (nextState == m_nameState) return;
		std::shared_ptr<Renderer::StateAnimation> newState = RESOURCE_MANAGER::getStateAnimation(nextState);
		if (!newState->canChange(m_nameState)) { return; }
		if (m_state->getUninterrupted()) {
			m_bufferState = nextState;
			return;
		}
		m_nameState = nextState;
		m_state = newState;
		m_bufferState = newState->getNextStateName();
		m_sprites = newState->getFrames();
		m_indexFrame = 0;
		m_durationFrame = m_sprites[m_indexFrame].second;
	}
}

void StateController::update(double duration){
	m_time += duration;
	while (m_time > m_durationFrame) {
		m_time -= m_durationFrame;
		m_indexFrame++;
	
		if (m_indexFrame < m_sprites.size())m_durationFrame = m_sprites[m_indexFrame].second;
		else setState(m_bufferState);
	}
}

void StateController::render(glm::vec2& position, glm::vec2& size, float rotation, float layer){
	m_sprites[m_indexFrame].first->render(position, size, rotation, layer);
}
