#include "GameSceneResources.h"

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include "../Managers/ResourceManager.h"
#include "../Renderer/MapRender.h"
#include "Objects/Camera.h"

void GameSceneResources::load(const std::string& gameScene){
	rapidjson::Document JSONDoc = RESOURCE_MANAGER::loadJSONDoc(gameScene);

	std::string MapPath = JSONDoc.FindMember("map")->value["mapPath"].GetString();
	float CellMapSize = JSONDoc.FindMember("map")->value["sizeCell"].GetFloat();

	MAP::setMap(MapPath, CellMapSize);

	float CameraX = JSONDoc.FindMember("camera")->value["position"].FindMember("x")->value.GetFloat();
	float CameraY = JSONDoc.FindMember("camera")->value["position"].FindMember("y")->value.GetFloat();
	float CameraSize = JSONDoc.FindMember("camera")->value["sizeCamera"].GetFloat();

	CAMERA::setCoords(glm::vec2(CameraX, CameraY));
	CAMERA::setSize(CameraSize);
}

void GameSceneResources::clear(){
}

