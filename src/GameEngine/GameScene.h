#pragma once

#include "Objects/IObject.h"
#include <memory>
#include <string>
#include <vector>
#include "GameSceneResources.h"

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
};