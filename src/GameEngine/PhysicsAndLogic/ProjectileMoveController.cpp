#include "ProjectileMoveController.h"

void ProjectileMoveController::addMoveInstruction(std::function<void(Projectile&, const double&)> function, const double& time){
	m_moveScript.push_back(std::make_pair(function, time));
}

double ProjectileMoveController::UpdateProjectile(Projectile& projectile, const int8_t& indexInstruction, const double& duration){
	m_moveScript.at(indexInstruction).first(projectile, duration);
	return m_moveScript.at(indexInstruction).second;
}

int8_t ProjectileMoveController::SizeScript(){
	return m_moveScript.size();
}
