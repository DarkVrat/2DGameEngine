#include "GameScene.h"

#include "../Renderer/MapRender.h"
#include "../Control/KeyboardControl.h"
#include "../Control/MouseControl.h"
#include "Objects/Camera.h"
#include "PhysicsAndLogic/DebugRender.h"

GameScene::GameScene(){
	
}

void GameScene::init(const std::string& gameScene){
	std::string jsonPath = "res/Scenes/" + gameScene + ".json";

	clear();

	m_Resources.load(jsonPath);

	glm::vec2 sezeMap = MAP::getSize();
	m_GSTree = GameSpaceTree(sezeMap);

	EntityData dat1(1.f, 20.f, 256.f, 0.5f, 50.f);
	std::vector<glm::vec2> vecPol;
	vecPol.push_back(glm::vec2(32, 32));
	vecPol.push_back(glm::vec2(32, -32));
	vecPol.push_back(glm::vec2(-32, -32));
	vecPol.push_back(glm::vec2(-32, 32));
	m_MainEntity = std::make_shared<Entity>(dat1, glm::vec2(40, 40), vecPol);
	m_MainEntity->ShapeIsCircle(32, 16);
	m_GSTree.addToTree(m_MainEntity);
	m_GSTree.updatePositionCamera();
	m_GSTree.updateSizeCamera();
}
 
void GameScene::render(){
	
	if (debug) {
		DebugRender::updateUniform();
		m_GSTree.DebugRender();
	}

	MAP::render();
}

void GameScene::update(const double& duration){

	
	if (KEYBOARD::ifClamped(GLFW_KEY_LEFT)) {
		glm::vec2 pos = CAMERA::getCoords(); 
		pos.x -= duration * CAMERA::getSize().x / 1000;
		CAMERA::setCoords(pos);
		m_GSTree.updatePositionCamera();
	}
	if (KEYBOARD::ifClamped(GLFW_KEY_RIGHT)) {
		glm::vec2 pos = CAMERA::getCoords();
		pos.x += duration * CAMERA::getSize().x / 1000;
		CAMERA::setCoords(pos);
		m_GSTree.updatePositionCamera();
	}
	if (KEYBOARD::ifClamped(GLFW_KEY_DOWN)) {
		glm::vec2 pos = CAMERA::getCoords();
		pos.y -= duration * CAMERA::getSize().x / 1000;
		CAMERA::setCoords(pos);
		m_GSTree.updatePositionCamera();
	}
	if (KEYBOARD::ifClamped(GLFW_KEY_UP)) {
		glm::vec2 pos = CAMERA::getCoords();
		pos.y += duration * CAMERA::getSize().x / 1000;
		CAMERA::setCoords(pos);
		m_GSTree.updatePositionCamera();
	}

	glm::vec2 direction = glm::vec2(0, 0);
	if (KEYBOARD::ifClamped(GLFW_KEY_W)) direction.y += 1.f;
	if (KEYBOARD::ifClamped(GLFW_KEY_S)) direction.y -= 1.f;
	if (KEYBOARD::ifClamped(GLFW_KEY_A)) direction.x -= 1.f;
	if (KEYBOARD::ifClamped(GLFW_KEY_D)) direction.x += 1.f;
	m_MainEntity->SetDirection(direction);

	if (KEYBOARD::ifClamped(GLFW_KEY_Q)) {
		m_MainEntity->Rotate(duration/20);
	}
	if (KEYBOARD::ifClamped(GLFW_KEY_E)) {
		m_MainEntity->Rotate(-duration/20);
	}

	if (MOUSE::ifPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
		m_MainEntity->AddImpulse(glm::vec2(500, 500));
	}

	if (KEYBOARD::ifPressed(GLFW_KEY_F2)) {
		debug = !debug;
	}

	if (MOUSE::ifPressed(GLFW_MOUSE_BUTTON_LEFT)) {
		glm::vec2 sizeCam = Camera::getSize();
		glm::vec2 posCam = Camera::getCoords();
		glm::vec2 posmouse = MOUSE::getPosition();
		glm::vec2 posFromSizeCamera = posmouse * Camera::getSize() + glm::vec2(posCam.x-sizeCam.x/2, posCam.y - sizeCam.y/2);

		EntityData dat1(1.f, 1.f, 1.f, 0.2f, 20.f);
		std::vector<glm::vec2> vecPol;
		vecPol.push_back(glm::vec2(32, 32));
		vecPol.push_back(glm::vec2(32, -32));
		vecPol.push_back(glm::vec2(-32, -32));
		vecPol.push_back(glm::vec2(-32, 32));

		Entity obj(dat1, posFromSizeCamera, vecPol);
		obj.ShapeIsCircle(4, 8);
		m_GSTree.addToTree(obj);


	}

	m_GSTree.Update(duration);
}

void GameScene::events(){
	float s = MOUSE::getScroll().y;
	if (std::abs(s) > 0) {
		CAMERA::setSize(CAMERA::getSize().x * (20-s)/20);
		m_GSTree.updateSizeCamera();
	}
}

void GameScene::clear(){
}
