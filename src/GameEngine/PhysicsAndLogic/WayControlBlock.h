#pragma once

#include "WaySearch.h"

class WayControlBlock {
public:
	glm::vec2 followWayTo(const glm::vec2& positionStart, const glm::vec2& positionEnd);
	void updateDirection(glm::vec2& direction, const glm::vec2& position);
private:
	std::vector<glm::vec2> m_way;
	float m_lengthError = 1.f;
	float m_distanceToTarget = 0;
};