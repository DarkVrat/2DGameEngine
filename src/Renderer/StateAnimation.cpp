#include "StateAnimation.h"

namespace Renderer{
	 //(RUS) Конструктор, заполняющий параметры
	//(ENG) Constructor that populates parameters
	StateAnimation::StateAnimation(const std::vector<std::pair<std::shared_ptr<Renderer::Sprite>, double>>& frames, const std::vector<std::string>& sources, const std::string& nextState, const bool& uninterrupted)
		:m_frames(frames),m_sources(sources),m_nextState(nextState), m_uninterrupted(uninterrupted) {}

	 //(RUS) Функция, позволяющая определить можно ли перейти с stateName, на this 
	//(ENG) A function that allows you to determine whether it is possible to switch from stateName to this
	bool StateAnimation::canChange(const std::string& stateName) {
		for (std::string current : m_sources) {
			if (current == stateName) return true;
		}
		return false;
	}

	 //(RUS) Получение параметров состояния
	//(ENG) Getting state parameters
	bool StateAnimation::getUninterrupted() { return m_uninterrupted; }
	std::vector<std::pair<std::shared_ptr<Renderer::Sprite>, double>> StateAnimation::getFrames() { return m_frames; }
	std::string StateAnimation::getNextStateName() { return m_nextState; }
}