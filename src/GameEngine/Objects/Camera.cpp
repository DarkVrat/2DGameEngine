#include "Camera.h"

#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "../../Managers/ConfigManager.h"
#include "../../Managers/ResourceManager.h"

std::shared_ptr<IObject> Camera::m_Object;
glm::vec2 Camera::m_followingRectangle;
float Camera::m_BaseSpeed;
float Camera::m_DistanceSpeed;
glm::vec2 Camera::m_coordsCamera;
glm::vec2 Camera::m_size;

void Camera::setCoords(const glm::vec2& newCoords){
	m_coordsCamera = newCoords;
	RESOURCE_MANAGER::setView(glm::vec3(m_coordsCamera.x - m_size.x / 2, m_coordsCamera.y - m_size.y / 2, 0));
}

glm::vec2 Camera::getCoords(){
	return m_coordsCamera;
}

void Camera::setSize(const float& width) {
	glm::vec2 sizeWindow = CONFIG_MANAGER::getWindowSize();

	m_size.x = width;
	m_size.y = m_size.x * sizeWindow.y / sizeWindow.x;
	glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_size.x), 0.f, static_cast<float>(m_size.y), -100.f, 100.f);


	RESOURCE_MANAGER::setProjection(projectionMatrix, true);
	RESOURCE_MANAGER::setView(glm::vec3(m_coordsCamera.x - m_size.x / 2, m_coordsCamera.y - m_size.y / 2, 0));
}

glm::vec2 Camera::getSize(){
	return m_size;
} 


void Camera::update(){
	glm::vec2 posObject = m_Object->getPosition();
	float distanceToTargetX = std::abs(posObject.x - m_coordsCamera.x);
	float distanceToTargetY = std::abs(posObject.y - m_coordsCamera.y);

	if (distanceToTargetX > m_followingRectangle.x) {
		float interpolationFactorX = m_BaseSpeed + m_DistanceSpeed * distanceToTargetX / m_followingRectangle.x;
		m_coordsCamera.x += (posObject.x - m_coordsCamera.x) * interpolationFactorX;
	}
	if (distanceToTargetY > m_followingRectangle.y) {
		float interpolationFactorY = m_BaseSpeed + m_DistanceSpeed * distanceToTargetY / m_followingRectangle.y;
		m_coordsCamera.y += (posObject.y - m_coordsCamera.y) * interpolationFactorY;
	}
}

void Camera::updateSize(){
	setSize(m_size.x);
}

void Camera::setSettings(const glm::vec2& RectangleFollowing, const float& baseSpeed, const float& distanceSpeed){
	m_followingRectangle.x = RectangleFollowing.x / 2;
	m_followingRectangle.y = RectangleFollowing.y / 2;
	m_BaseSpeed = baseSpeed;
	m_DistanceSpeed = distanceSpeed;
}
