#pragma once

#include <memory>
#include <string>
#include <vector>
#include "GameSceneResources.h"
#include "Objects/Collider.h"
#include "Objects/Entity.h"
#include "Objects/Object.h"
#include "Objects/Trigger.h"

class GameScene {
public:
	GameScene();
	void init(const std::string& gameScene);
	void render();
	void update(const double& duration);
	void events();
	void clear();
private:
	GameSceneResources m_Resources;
	Entity col_1;
	Collider col_2;
	bool debug = false;
};