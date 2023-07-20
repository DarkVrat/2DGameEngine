#pragma once

#include <memory>
#include <string>
#include <vector>
#include "GameSceneResources.h"
#include "PhysicsAndLogic/GameSpaceTree.h"

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
	GameSpaceTree m_GSTree;
	std::shared_ptr<Entity> m_MainEntity;
	bool debug = false;
	std::vector<glm::vec2> vectorWay;
	Shape shapeWay;
};