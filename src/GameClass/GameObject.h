#pragma once

#include <glm/vec2.hpp>
#include <memory>
#include "StateController.h"
#include "../Audio/SoundEffectsPlayer.h"

class GameObject {
public:
	GameObject(std::string startState, glm::vec2& position, glm::vec2& size, float rotation, float layer=0.f);
	~GameObject();

	void update(double duration);
	void render();

	void attack();
	void idle();

	void setPosition(const glm::vec2& position);
	void setSize(const glm::vec2& size);
	void setRotation(const float rotation);

	glm::vec2 getPosition();
	float getLayer();

private:
	std::map<std::string, std::shared_ptr<Audio::SoundEffectsPlayer>> m_mapPlayer;
	std::shared_ptr<StateController> m_stateControll;
	glm::vec2 m_position;
	glm::vec2 m_size;
	float m_rotation;
	float m_layer;
};