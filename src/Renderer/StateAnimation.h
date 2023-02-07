#pragma once

#include <vector>
#include <string>
#include "Sprite.h"

namespace Renderer{
	class StateAnimation {
	public:
		StateAnimation(std::vector<std::pair<std::shared_ptr<Renderer::Sprite>, double>> frames, std::vector<std::string> sources, std::string nextState, bool uninterrupted);

		bool canChange(const std::string stateName);

		bool getUninterrupted();
		std::vector<std::pair<std::shared_ptr<Renderer::Sprite>, double>> getFrames();
		std::string getNextStateName();

	private:
		std::vector<std::pair<std::shared_ptr<Renderer::Sprite>, double>> m_frames;
		std::vector<std::string> m_sources;
		std::string m_nextState;
		bool m_uninterrupted;
	};
}
