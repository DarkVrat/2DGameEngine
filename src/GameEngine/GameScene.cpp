#include "GameScene.h"

#include "../Renderer/MapRender.h"

void GameScene::init(const std::string& gameScene){
	std::string jsonPath = "res/Scenes/" + gameScene + ".json";

	clear();

	m_Resources.load(jsonPath);
}

void GameScene::render(){
	MAP::render();
}

void GameScene::update(const double& duration){
}

void GameScene::events(){
}

void GameScene::clear(){
	m_Objects.clear();
}
