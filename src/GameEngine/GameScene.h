#pragma once

#include "Objects/IObject.h"
#include <memory>
#include <string>
#include <vector>
#include "GameSceneResources.h"
#include "PhysicsAndLogic/Collision.h"

class GameScene {
public:
	void init(const std::string& gameScene);
	void render();
	void update(const double& duration);
	void events();
	void clear();
private:
	GameSceneResources m_Resources;
	std::vector<std::shared_ptr<IObject>> m_Objects;
	Collision col_1;
	Collision col_2;
	bool debug = false;
};