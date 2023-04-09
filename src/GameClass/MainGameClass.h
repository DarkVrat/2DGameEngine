#pragma once

#include <memory>
#include <glm/vec2.hpp>
#include "UIGame/Menu.h"

#define MAIN_GAME_CLASS MainGameClass

 //(RUS) Главный игровой класс, отвечет за работу с игровыми объектами, их рендеринг и обновление, а так же работает с нажатиями
//(ENG) The main game class, responsible for working with game objects, rendering and updating them, and also works with clicks

class MainGameClass {
public:
	static bool init(glm::vec2 window);

	static void update(const double& duration);
	static void render();

	static void events();
	static void setProjectionMat(const glm::ivec2& window);

	static void terminate();

	static Menu& getMenu();

private:
	MainGameClass();
	~MainGameClass() {};

	static Menu m_Menu;
	static glm::ivec2 m_window;
	static double m_time;
	static int m_fps;
};