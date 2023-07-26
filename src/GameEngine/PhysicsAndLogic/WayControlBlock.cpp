#include "WayControlBlock.h"

#include <glm/glm.hpp>

glm::vec2 WayControlBlock::followWayTo(const glm::vec2& positionStart, const glm::vec2& positionEnd){
	m_way = WaySearch::FindWay(positionStart, positionEnd);

	if (!m_way.empty()) {
		m_lengthError = WaySearch::getLengthError();
		m_distanceToTarget = glm::length(*(m_way.end() - 1) - positionStart);
		return glm::normalize(*(m_way.end() - 1) - positionStart);
	}

	return glm::vec2(0,0);
}

void WayControlBlock::updateDirection(glm::vec2& direction, const glm::vec2& position){
	if (!m_way.empty()) {
		float distanceNew = glm::length(position - *(m_way.end() - 1));

		if (distanceNew < m_distanceToTarget) {
			m_distanceToTarget = distanceNew;
		}
		else {
			direction = glm::normalize(*(m_way.end() - 1) - position);
		}

		if (m_distanceToTarget < m_lengthError) {
			m_way.pop_back();
			if (m_way.empty()) {
				direction = glm::vec2(0, 0);
			}
			else {
				direction = glm::normalize(*(m_way.end() - 1) - position);
				m_distanceToTarget = glm::length(*(m_way.end() - 1) - position);
			}
		}
	}
}
