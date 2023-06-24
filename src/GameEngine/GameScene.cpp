#include "GameScene.h"

#include "../Renderer/MapRender.h"
#include "../Control/KeyboardControl.h"
#include "../Control/MouseControl.h"
#include "Objects/Camera.h"
#include "PhysicsAndLogic/DebugRender.h"

void GameScene::init(const std::string& gameScene){
	std::string jsonPath = "res/Scenes/" + gameScene + ".json";

	std::vector<glm::vec2> vecPol;
	vecPol.push_back(glm::vec2(32, 32));
	vecPol.push_back(glm::vec2(32, -32));
	vecPol.push_back(glm::vec2(-32, -32));
	vecPol.push_back(glm::vec2(-32, 32));

	col_1.SetPosition(glm::vec2(0, 0));
	col_1.ShapeIsPolygon(vecPol);

	col_2.SetPosition(glm::vec2(0, 70));
	col_2.ShapeIsCircle(32.f, 32);

	clear();

	m_Resources.load(jsonPath);
}

void GameScene::render(){
	if (debug) {
		DebugRender::updateUniform();
		EPAResult result = col_1.CheckCollision(col_2);
		if (result.hasCollision) {
			col_1.Move(-result.penetrationVector);
			DebugRender::drawShape(col_1, glm::vec4(1, 0, 0, 1));
			DebugRender::drawShape(col_2, glm::vec4(1, 0, 0, 1));
		}
		else {
			DebugRender::drawShape(col_1, glm::vec4(0, 1, 0, 1));
			DebugRender::drawShape(col_2, glm::vec4(0, 1, 0, 1));
		}
	}
	else {
		col_1.Move(-col_1.CheckCollision(col_2).penetrationVector);
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
}

void GameScene::events(){
	float s = MOUSE::getScroll().y;
	if (std::abs(s) > 0) {
		CAMERA::setSize(CAMERA::getSize().x * (100-s)/100.f);
	}
}

void GameScene::clear(){
	m_Objects.clear();
}
