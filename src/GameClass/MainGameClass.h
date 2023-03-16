#pragma once

#include <memory>
#include <glm/vec2.hpp>
#include "GameObject.h"
#include "../UI/Button.h"
#include "../UI/SwitchBool.h"
#include "../UI/Slider.h"
#include "../UI/ListParameter.h"
#include "../UI/Translater.h"

#define MAIN_GAME_CLASS MainGameClass

 //(RUS) Главный игровой класс, отвечет за работу с игровыми объектами, их рендеринг и обновление, а так же работает с нажатиями
//(ENG) The main game class, responsible for working with game objects, rendering and updating them, and also works with clicks

class MainGameClass {
public:
	static bool init();

	static void update(const double& duration);
	static void render();

	static void events();
	static void sortGameObject();
	static void setProjectionMat(const glm::ivec2& window);

	static void terminate();

private:
	MainGameClass();
	~MainGameClass() {};

	static std::vector<std::shared_ptr<GameObject>> m_vectorGameObject;

	enum E_GAME_STATE : int8_t{
		MAIN_MENU,
		NEW_GAME,
		PAUSE,
		SAVE_AND_LOAD,
		SETTINGS,
		EXIT
	};
	static glm::ivec2 m_window;
	static E_GAME_STATE m_GameState;
	static double m_time;
	static int m_fps;
	static int size;

	static std::array<UserInterface::Button, 2> m_testButton;
	static UserInterface::SwitchBool m_textSwich;
	static UserInterface::Slider m_testSliderVertical;
	static UserInterface::Slider m_testSliderHorizontal;
	static glm::vec2 m_vecToTestSliders;
	static UserInterface::ListParameter<glm::ivec2> m_testList;

	static std::array<UserInterface::Slider, 8> m_SliderForSpriteSetting;
	static glm::vec3 m_vecForSpritePosition;
	static glm::vec2 m_vecForSpriteSize;
	static float m_floatForSpriteRotation;
	static glm::vec2 m_vecForSpriteOrigin;
};