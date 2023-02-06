#pragma once

#include<string>
#include"../Managers/ResourceManager.h"

class StateController {
public:
	StateController(const std::string initState);

	void setState(const std::string nextState);
	void update(const double duration);
	void render(glm::vec2& position, glm::vec2& size, float rotation, float layer);

private:
	std::vector<std::pair<std::shared_ptr<Renderer::Sprite>, double>> m_Sprites;
	std::shared_ptr<Renderer::StateAnimation> m_State;
	std::string m_nameState;
	std::string m_bufferState;
	size_t m_indexFrame;
	double m_durationFrame;
	double m_time;
};