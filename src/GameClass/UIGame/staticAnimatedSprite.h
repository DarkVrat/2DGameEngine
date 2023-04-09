#pragma once

#include "../Animation/StateAnimationController.h"

class staticAnimatedSprite {
public:
	staticAnimatedSprite(const std::string& state, const glm::vec3& position, const glm::vec2& size, const GLfloat& rotation, const glm::vec2& origin);

	void renderUI();
	void render();

	void update(const double& duration);
private:
	std::shared_ptr<StateAnimationController> m_state;
	glm::vec3 m_position;
	glm::vec2 m_size;
	GLfloat m_rotation;
	glm::vec2 m_origin;
};