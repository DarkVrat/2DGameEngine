#include "GameObject.h"
#include"../Resources/ResourceManager.h"

GameObject::GameObject(std::string startState, glm::vec2& position, glm::vec2& size, float rotation, float layer){
	m_stateControll = std::make_shared<StateController>(startState);
	m_position=position;
	m_size=size;
	m_rotation=rotation;
	m_layer = layer;
}
