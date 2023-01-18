#pragma once

#include<glad/glad.h>
#include<glm/vec2.hpp>
#include<memory>
#include"StateController.h"
#include"../Renderer/PrintText.h"
#include"../Audio/SoundEffectsPlayer.h"

class GameObject {
public:
	GameObject(std::string startState, glm::vec2& position, glm::vec2& size, float rotation, float layer=0.f);

	void update(double duration) { m_stateControll->update(duration); }
	void render() { m_stateControll->render(m_position, m_size, m_rotation, m_layer); }
	void attack();
	void idle() { m_stateControll->setState("Idle"); }

	void setPosition(const glm::vec2& position) {m_position = position;}
	void setSize(const glm::vec2& size){ m_size = size; }
	void setRotation(const float rotation){ m_rotation = rotation; }

	glm::vec2 getPosition() { return m_position; }
	float getLayer() { return m_layer; }
private:
	std::shared_ptr<StateController> m_stateControll;
	glm::vec2 m_position;
	glm::vec2 m_size;
	float m_rotation;
	float m_layer;
	std::map<std::string, Audio::SoundEffectsPlayer> m_mapPlayer;
};