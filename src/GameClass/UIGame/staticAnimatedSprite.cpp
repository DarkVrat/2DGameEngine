#include "staticAnimatedSprite.h"

staticAnimatedSprite::staticAnimatedSprite(const std::string& state, const glm::vec3& position, const glm::vec2& size, const GLfloat& rotation, const glm::vec2& origin)
	:m_position(position), m_size(size), m_rotation(rotation), m_origin(origin) {
	m_state = std::make_shared<StateAnimationController>(state);
}

void staticAnimatedSprite::renderUI(){
	m_state->renderUI(m_position, m_size, m_rotation, m_origin);
}

void staticAnimatedSprite::render(){
	m_state->render(m_position, m_size, m_rotation, m_origin);
}

void staticAnimatedSprite::update(const double& duration){
	m_state->update(duration);
}
