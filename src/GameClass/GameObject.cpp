#include<cmath>
#include "GameObject.h"
#include"../Resources/ResourceManager.h"
#include"../Audio/SoundEffectsLibrary.h"

GameObject::GameObject(std::string startState, glm::vec2& position, glm::vec2& size, float rotation, float layer){
	m_stateControll = std::make_shared<StateController>(startState);
	m_position=position;
	m_size=size;
	m_rotation=rotation;
	m_layer = layer;

	Audio::SoundEffectsPlayer player(SOUND_LIBRARY->Load("sword"));
	player.SetVec3Param(AL_POSITION, glm::vec3(position, 0.f));
	player.SetFloatParam(AL_MAX_DISTANCE, 5.f);
	m_mapPlayer.emplace("Attack1", std::move(player));
}

void GameObject::attack(){
	m_stateControll->setState("Attack1");
	PRINT_TEXT->AddTextInTimeBuffer("I attack!", glm::vec3(m_position.x-abs(m_size.x)/6, m_position.y + m_size.y / 3, 100), 0.25, glm::vec3(1, 1, 1), 1000);
	m_mapPlayer.at("Attack1").Play();
}
