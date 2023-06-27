#include "GameScene.h"

#include "../Renderer/MapRender.h"
#include "../Control/KeyboardControl.h"
#include "../Control/MouseControl.h"
#include "Objects/Camera.h"
#include "PhysicsAndLogic/DebugRender.h"

GameScene::GameScene(){
	std::function<void(Entity& entity, const double& duration)> fun = [](Entity& entity, const double& duration) {
		entity.Rotate(duration / 20);
	};

	EntityData dat1(1, 20, 1);
	//TriggerData dat2(true, true, fun);

	std::vector<glm::vec2> vecPol;
	vecPol.push_back(glm::vec2(32, 32));
	vecPol.push_back(glm::vec2(32, -32));
	vecPol.push_back(glm::vec2(-32, -32));
	vecPol.push_back(glm::vec2(-32, 32));

	col_1 = Entity(dat1, glm::vec2(0, 0), vecPol);
	col_2 = Collider(glm::vec2(0, 70), vecPol);
}

void GameScene::init(const std::string& gameScene){
	std::string jsonPath = "res/Scenes/" + gameScene + ".json";

	clear();

	m_Resources.load(jsonPath);
}

void GameScene::render(){
	
	if (debug) {
		DebugRender::updateUniform();
		DebugRender::drawShape(col_1, glm::vec4(0, 1, 0, 1));
		DebugRender::drawShape(col_2, glm::vec4(0, 1, 0, 1));
	}

	MAP::render();
}

void GameScene::update(const double& duration){
	
	if (KEYBOARD::ifClamped(GLFW_KEY_LEFT)) {
		glm::vec2 pos = CAMERA::getCoords(); 
		pos.x -= duration * CAMERA::getSize().x / 1000;
		CAMERA::setCoords(pos);
	}
	if (KEYBOARD::ifClamped(GLFW_KEY_RIGHT)) {
		glm::vec2 pos = CAMERA::getCoords();
		pos.x += duration * CAMERA::getSize().x / 1000;
		CAMERA::setCoords(pos);
	}
	if (KEYBOARD::ifClamped(GLFW_KEY_DOWN)) {
		glm::vec2 pos = CAMERA::getCoords();
		pos.y -= duration * CAMERA::getSize().x / 1000;
		CAMERA::setCoords(pos);
	}
	if (KEYBOARD::ifClamped(GLFW_KEY_UP)) {
		glm::vec2 pos = CAMERA::getCoords();
		pos.y += duration * CAMERA::getSize().x / 1000;
		CAMERA::setCoords(pos);
	}

	if (KEYBOARD::ifClamped(GLFW_KEY_A)) {
		col_1.Move(glm::vec2(-duration / 20, 0));
	}
	if (KEYBOARD::ifClamped(GLFW_KEY_D)) {
		col_1.Move(glm::vec2(duration / 20, 0));
	}
	if (KEYBOARD::ifClamped(GLFW_KEY_S)) {
		col_1.Move(glm::vec2(0, -duration / 20));
	}
	if (KEYBOARD::ifClamped(GLFW_KEY_W)) {
		col_1.Move(glm::vec2(0, duration / 20));
	}
	if (KEYBOARD::ifClamped(GLFW_KEY_Q)) {
		col_1.Rotate(duration/20);
	}
	if (KEYBOARD::ifClamped(GLFW_KEY_E)) {
		col_1.Rotate(-duration/20);
	}

	if (KEYBOARD::ifClamped(GLFW_KEY_LEFT_SHIFT)) {
		if (KEYBOARD::ifPressed(GLFW_KEY_F2)) {
			debug = !debug;
		}
	}
	if (KEYBOARD::ifPressed(GLFW_KEY_1)) {
		col_2.Scale(1.5f, -45.f);
	}
	col_1.CheckCollision(col_2);
}

void GameScene::events(){
	float s = MOUSE::getScroll().y;
	if (std::abs(s) > 0) {
		CAMERA::setSize(CAMERA::getSize().x * (20-s)/20);
	}
}

void GameScene::clear(){
}
