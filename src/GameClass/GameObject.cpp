#include "GameObject.h"

#include <glad/glad.h>
#include "../Audio/SoundEffectsLibrary.h"
#include "../Managers/ResourceManager.h"
#include "../Renderer/PrintText.h"

 //(RUS) �������� �������� ������� �� ����� �����������
//(ENG) Creating a game object with all parameters
GameObject::GameObject(std::string startState, const glm::vec2& position, const  glm::vec2& size, float rotation, float layer){
	m_stateControll = std::make_shared<StateController>(startState);
	m_position=position;
	m_size=size;
	m_rotation=rotation;
	m_layer = layer;

	std::shared_ptr<Audio::SoundEffectsPlayer> player = MAKE_SOUND_PLAYER("sword");
	player->setVec3Param(AL_POSITION, glm::vec3(position, 0.f));
	m_mapPlayer.emplace("Attack1", player);
}

 //(RUS) �������� �������� �������
//(ENG) game object removal
GameObject::~GameObject(){
	m_mapPlayer.clear();
}

 //(RUS) ���������� �������� �������
//(ENG) game object update
void GameObject::update(double duration){
	m_stateControll->update(duration);
}

 //(RUS) ��������� �������� �������
//(ENG) game object rendering
void GameObject::render(){
	m_stateControll->render(m_position, m_size, m_rotation, m_layer);
}

 //(RUS) ������� �����, ������ ��������, � ��������������� �����
//(ENG) attack event, start animation, and play sound
void GameObject::attack(){
	m_stateControll->setState("Attack1");
	m_mapPlayer.at("Attack1")->play();
}

 //(RUS) ������� �����������
//(ENG) idle event
void GameObject::idle(){
	m_stateControll->setState("Idle");
}

 //(RUS) ������� � �������
//(ENG) getters and setters
void GameObject::setPosition(const glm::vec2& position) { m_position = position; }
void GameObject::setSize(const glm::vec2& size) { m_size = size; }
void GameObject::setRotation(const float& rotation) { m_rotation = rotation; }
void GameObject::setLayer(const float& layer) { m_layer = layer; }
glm::vec2 GameObject::getPosition() { return m_position; }
glm::vec2 GameObject::getSize() { return m_size; }
float GameObject::getRotation() { return m_rotation; }
float GameObject::getLayer() { return m_layer; }
