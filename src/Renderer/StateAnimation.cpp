#include "StateAnimation.h"

namespace Renderer{
	StateAnimation::StateAnimation(std::vector<std::pair<std::shared_ptr<Renderer::Sprite>, double>> frames, std::vector<std::string> sources, std::string nextState, bool uninterrupted)
		:m_frames(std::move(frames)),m_sources(std::move(sources)),m_nextState(nextState), m_uninterrupted(uninterrupted) {
	}


	bool StateAnimation::canChange(const std::string stateName) {
		for (std::string current : m_sources) {
			if (current == stateName) return true;
		}
		return false;
	}
}