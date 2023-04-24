#pragma once

#include "../../Renderer/Sprite.h"
#include <glad/glad.h>

class staticSprite {
public:
	staticSprite(std::shared_ptr<Renderer::Sprite> sprite, const glm::vec3& position, const glm::vec2& size, const GLfloat& rotation, const glm::vec2& origin);

	void render();
	void renderUI();
private:
	std::shared_ptr<Renderer::Sprite> m_sprite;
	glm::vec3 m_position;
	glm::vec2 m_size;
	GLfloat m_rotation;
	glm::vec2 m_origin;
};