#pragma once

#include <glm/vec2.hpp>
#include <memory>
#define CAMERA Camera

class Camera {
public:
	static void setCoords(const glm::vec2& newCoords);
	static glm::vec2 getCoords();
	static void setSize(const float& width);
	static glm::vec2 getSize();

	static void update();
	static void updateSize();
	static void setSettings(const glm::vec2& RectangleFollowing, const float& baseSpeed, const float& distanceSpeed);
private:
	static glm::vec2 m_followingRectangle;
	static float m_BaseSpeed;
	static float m_DistanceSpeed;

	static glm::vec2 m_coordsCamera;
	static glm::vec2 m_size;
};