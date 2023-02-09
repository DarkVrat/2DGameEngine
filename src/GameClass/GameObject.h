#pragma once

#include <glm/vec2.hpp>
#include <memory>
#include "StateController.h"
#include "../Audio/SoundEffectsPlayer.h"

     //(RUS) игровой объект, имеющий map аудио::SoundEffectPlayer для востпроизведения звуков, 
    //		 StateController для рендеринга спрайтов и контроля анимации, 
   //		 а так же параметры самого объекта
  //(ENG) a game object that has a map audio::SoundEffectPlayer for playing sounds, 
 //		  a StateController for rendering sprites and controlling animation, 
//        as well as the parameters of the object itself

class GameObject {
public:
	GameObject(std::string startState, const glm::vec2& position, const glm::vec2& size,  float rotation,  float layer=0.f);
	~GameObject();

	void update(double duration);
	void render();

	void attack();
	void idle();

	void setPosition(const glm::vec2& position);
	void setSize(const glm::vec2& size);
	void setRotation(const float& rotation);
	void setLayer(const float& layer);
	glm::vec2 getPosition();
	glm::vec2 getSize();
	float getRotation();
	float getLayer();

private:
	std::map<std::string, std::shared_ptr<Audio::SoundEffectsPlayer>> m_mapPlayer;
	std::shared_ptr<StateController> m_stateControll;
	glm::vec2 m_position;
	glm::vec2 m_size;
	float m_rotation;
	float m_layer;
};