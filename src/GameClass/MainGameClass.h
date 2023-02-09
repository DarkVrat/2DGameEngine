#pragma once

#include <memory>
#include <glm/vec2.hpp>
#include "GameObject.h"

#define MAIN_GAME_CLASS MainGameClass

 //(RUS) Главный игровой класс, отвечет за работу с игровыми объектами, их рендеринг и обновление, а так же работает с нажатиями
//(ENG) The main game class, responsible for working with game objects, rendering and updating them, and also works with clicks

class MainGameClass {
public:
	static bool init();

	static void update(double duration);
	static void render();

	static void events();
	static void sortGameObject();
	static void setProjectionMat(glm::ivec2 window);

	static void terminate();

private:
	MainGameClass();
	~MainGameClass() {};

	static std::vector<std::shared_ptr<GameObject>> m_vectorGameObject;

	enum E_GAME_STATE : int8_t{
		Active,
		Pause
	};
	static glm::ivec2 m_window;
	static E_GAME_STATE m_GameState;
	static double m_time;
	static int m_fps;
};