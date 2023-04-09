#include "staticSprite.h"

staticSprite::staticSprite(std::shared_ptr<Renderer::Sprite> sprite, const glm::vec3& position, const glm::vec2& size, const GLfloat& rotation, const glm::vec2& origin)
	:m_sprite(sprite), m_position(position), m_size(size), m_rotation(rotation), m_origin(origin) {}

void staticSprite::render(){
	m_sprite->render(m_position, m_size, m_rotation, m_origin);
}

void staticSprite::renderUI(){
	m_sprite->renderUI(m_position, m_size, m_rotation, m_origin);
}
