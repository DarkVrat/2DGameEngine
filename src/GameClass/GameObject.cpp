#include "GameObject.h"

#include <glad/glad.h>
#include "../Audio/SoundEffectsLibrary.h"
#include "../Managers/ResourceManager.h"
#include "../Renderer/PrintText.h"

GameObject::GameObject(std::string startState, glm::vec2& position, glm::vec2& size, float rotation, float layer){
	m_stateControll = std::make_shared<StateController>(startState);
	m_position=position;
	m_size=size;
	m_rotation=rotation;
	m_layer = layer;

	std::shared_ptr<Audio::SoundEffectsPlayer> player = MAKE_SOUND_PLAYER("sword");
	player->setVec3Param(AL_POSITION, glm::vec3(position, 0.f));
	m_mapPlayer.emplace("Attack1", player);
}

GameObject::~GameObject(){
	m_mapPlayer.clear();
}

void GameObject::update(double duration){
	m_stateControll->update(duration);
}

void GameObject::render(){
	m_stateControll->render(m_position, m_size, m_rotation, m_layer);
}

void GameObject::attack(){
	m_stateControll->setState("Attack1");
	PRINT_TEXT::addTextInTimeBuffer("I attack!", glm::vec3(m_position.x-abs(m_size.x)/6, m_position.y + m_size.y / 3, 100), 0.25, glm::vec3(1, 1, 1), 1000);
	m_mapPlayer.at("Attack1")->play();
}

void GameObject::idle(){
	m_stateControll->setState("Idle");
}

void GameObject::setPosition(const glm::vec2& position){
	m_position = position;
}

void GameObject::setSize(const glm::vec2& size){
	m_size = size;
}

void GameObject::setRotation(const float rotation){
	m_rotation = rotation;
}

glm::vec2 GameObject::getPosition(){
	return m_position;
}

float GameObject::getLayer(){
	return m_layer;
}
