#pragma once

#include <string>
#include "../Managers/ResourceManager.h"

 //(RUS) Контроллер состояний анимации, управляет её переключением, и логикой
//(ENG) Animation state controller, controls its switching, and logic

class StateController {
public:
	StateController(const std::string& initState);

	void setState(const std::string& nextState);
	void update(const double& duration);
	void render(const glm::vec2& position, const glm::vec2& size, const float& rotation, const float& layer);

private:
	std::vector<std::pair<std::shared_ptr<Renderer::Sprite>, double>> m_sprites;
	std::shared_ptr<Renderer::StateAnimation> m_state;
	std::string m_nameState;
	std::string m_bufferState;
	size_t m_indexFrame;
	double m_durationFrame;
	double m_time;
};