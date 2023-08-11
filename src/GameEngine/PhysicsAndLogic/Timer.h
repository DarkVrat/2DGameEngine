#pragma once

#include <functional>
#include <vector>

class Timer {
public:
	static void addTimer(const double& time, const std::function<void(const double&)> function);
	static void updateTimers(const double& duration);
private:
	static std::vector<std::pair<double, std::function<void(const double&)>>> m_vecTimers;
};