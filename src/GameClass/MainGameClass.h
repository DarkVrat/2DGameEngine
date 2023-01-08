#pragma once
#include <cstdint>
#include <array>
#include <glm/vec2.hpp>
#include "GameObject.h"

class MainGameClass {
public:
	MainGameClass(const glm::ivec2& window);
	~MainGameClass();

	void update(double duration);
	void render();

	void setKey(const int key,const int action);
	bool init();
	void Events();
	void SetProjectionMat(glm::ivec2 window);

private:
	std::array<bool, 349> m_keys;
	std::vector<GameObject> m_GObject;

	enum class E_GAME_STATE {
		Active,
		Pause
	};
	glm::ivec2 m_window;
	E_GAME_STATE m_GState;
};