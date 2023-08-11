#include "Timer.h"

std::vector<std::pair<double, std::function<void(const double&)>>> Timer::m_vecTimers;

void Timer::addTimer(const double& time, const std::function<void(const double&)> function){
	m_vecTimers.push_back(std::make_pair(time, function));
}

void Timer::updateTimers(const double& duration){
	for (int i = m_vecTimers.size() - 1; i >= 0; i--) {
		m_vecTimers.at(i).first -= duration;
		m_vecTimers.at(i).second(duration);

		if (m_vecTimers.at(i).first < 0.f) {
			m_vecTimers.erase(m_vecTimers.begin() + i);
		}
	}
}
