#pragma once
#include <cstdint>
#include <array>
#include <glm/vec2.hpp>
#include "GameObject.h"

#define MAIN_GAME_CLASS MainGameClass::Get()

class MainGameClass {
public:
	static MainGameClass* Get();

	void update(double duration);
	void render();

	void sortGameObject();
	bool comp(GameObject a, GameObject b);

	bool init();
	void Events();
	void SetProjectionMat(glm::ivec2 window);

private:
	MainGameClass():m_window(glm::ivec2(0,0)),m_GState(E_GAME_STATE::Pause) {};
	~MainGameClass() {};

	std::vector<GameObject> m_GObject;

	enum class E_GAME_STATE {
		Active,
		Pause
	};
	glm::ivec2 m_window;
	E_GAME_STATE m_GState;
};