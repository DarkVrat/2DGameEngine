#pragma once

#include <memory>
#include <string>
#include <vector>
#include "GameSceneResources.h"
#include "PhysicsAndLogic/GameSpaceTree.h"
#include "Objects/Character.h"
#include "Objects/Projectile.h"

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
	std::shared_ptr<Character> m_MainEntity;
	std::shared_ptr<Projectile> m_project;
	bool debug = false;
};