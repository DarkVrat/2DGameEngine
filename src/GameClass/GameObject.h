#pragma once

#include <glm/vec2.hpp>
#include <memory>
#include "Animation/StateAnimationController.h"
#include "../Audio/SoundEffectsPlayer.h"

     //(RUS) игровой объект, имеющий map аудио::SoundEffectPlayer для востпроизведения звуков, 
    //		 StateController для рендеринга спрайтов и контроля анимации, 
   //		 а так же параметры самого объекта
  //(ENG) a game object that has a map audio::SoundEffectPlayer for playing sounds, 
 //		  a StateController for rendering sprites and controlling animation, 
//        as well as the parameters of the object itself

class GameObject {
public:
	GameObject(const std::string& startState, const glm::vec3& position, const glm::vec2& size, const float& rotation);
	~GameObject();

	void update(const double& duration);
	void render();

	void attack();
	void idle();

	void setPosition(const glm::vec3& position);
	void setSize(const glm::vec2& size);
	void setRotation(const float& rotation);
	glm::vec3 getPosition();
	glm::vec2 getSize();
	float getRotation();

private:
	std::map<std::string, std::shared_ptr<Audio::SoundEffectsPlayer>> m_mapPlayer;
	std::shared_ptr<StateAnimationController> m_stateControll;
	glm::vec3 m_position;
	glm::vec2 m_size;
	float m_rotation;
};