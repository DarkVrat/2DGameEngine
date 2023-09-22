#pragma once

#include <functional>
#include <vector>

class Projectile;

class ProjectileMoveController{
public:
	void addMoveInstruction(std::function<void(Projectile&, const double&)> function, const double& time=0.f);
	double UpdateProjectile(Projectile& projectile, const int8_t& indexInstruction, const double& duration);
	int8_t SizeScript();
private:
	std::vector<std::pair<std::function<void(Projectile&, const double&)>, double>> m_moveScript;
};

struct MoveScriptController {
	int8_t m_indexInstruction = 0;
	int8_t m_SizeScript = 0;
	double m_timeCurrentInstruction= 0.0;
};
