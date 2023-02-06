#pragma once

#include <memory>
#include <glm/vec2.hpp>
#include "GameObject.h"

#define MAIN_GAME_CLASS MainGameClass::Get()

class MainGameClass {
public:
	static MainGameClass* Get();
	static void Terminate();

	bool init();

	void update(double duration);
	void render();

	void Events();
	void sortGameObject();
	void SetProjectionMat(glm::ivec2 window);

private:
	MainGameClass():m_window(glm::ivec2(0,0)),m_GState(E_GAME_STATE::Pause), time(0.0),fps(0) {};
	~MainGameClass();

	std::vector<std::shared_ptr<GameObject>> m_GObject;

	enum class E_GAME_STATE {
		Active,
		Pause
	};
	glm::ivec2 m_window;
	E_GAME_STATE m_GState;
	double time;
	int fps;
};