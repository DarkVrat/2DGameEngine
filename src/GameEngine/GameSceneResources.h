#pragma once

#include <string>
#include "Objects/IObject.h"
#include <map>
#include <memory>

class GameSceneResources{
public:
	void load(const std::string& gameScene);
	void clear();

	std::shared_ptr<IObject> createFromID(const uint32_t& id);
private:
	std::map<uint32_t, std::shared_ptr<IObject>> m_mapSamples;
};