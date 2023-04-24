#pragma once

#include <glm/vec2.hpp>
#include <memory>

class IObject {
public:
	virtual void render() = 0;
	virtual void update() = 0;
	
	virtual void setPosition(const glm::vec2& newCoords) {
		m_coords = newCoords;
	}
	virtual glm::vec2 getPosition(){
		return m_coords;
	}
	virtual std::shared_ptr<IObject> copy() = 0;

public:
	glm::vec2 m_coords;
};