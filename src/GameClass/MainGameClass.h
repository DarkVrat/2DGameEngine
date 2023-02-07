#pragma once

#include <memory>
#include <glm/vec2.hpp>
#include "GameObject.h"

#define MAIN_GAME_CLASS MainGameClass

class MainGameClass {
public:
	static bool init();

	static void update(double duration);
	static void render();

	static void events();
	static void sortGameObject();
	static void setProjectionMat(glm::ivec2 window);

private:
	MainGameClass();
	~MainGameClass();

	static std::vector<std::shared_ptr<GameObject>> m_vectorGameObject;

	enum class E_GAME_STATE {
		Active,
		Pause
	};
	static glm::ivec2 m_window;
	static E_GAME_STATE m_GameState;
	static double m_time;
	static int m_fps;
};