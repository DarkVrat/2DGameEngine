#include "GameScene.h"

#include "../Renderer/MapRender.h"
#include "../Control/KeyboardControl.h"
#include "../Control/MouseControl.h"
#include "PhysicsAndLogic/WaySearch.h"
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

	EntityData dat1(1.f, 20.f, 128.f, 0.7f, 50.f);
	std::vector<glm::vec2> vecPol;
	vecPol.push_back(glm::vec2(8, 8));
	vecPol.push_back(glm::vec2(8, -8));
	vecPol.push_back(glm::vec2(-8, -8));
	vecPol.push_back(glm::vec2(-8, 8));
	m_MainEntity = std::make_shared<Character>(dat1, glm::vec2(504, 504), vecPol);
	m_GSTree.addToTree(m_MainEntity);
	m_GSTree.updatePositionCamera();
	m_GSTree.updateSizeCamera();
	CAMERA::setFollowingEntity(m_MainEntity);
	CAMERA::setCoords(glm::vec2(504, 504));
	CAMERA::setSettings(glm::vec2(32, 32), 0.0003, 0.0008); 

	m_project = std::make_shared<Projectile>(ProjectileData(), EntityData(1, 1, 128, 0, 1), glm::vec2(-1, -1));
	m_project->ShapeIsCircle(2, 6);
} 
 
void GameScene::render(){
	
	if (debug) {
		DebugRender::updateUniform();
		m_GSTree.DebugRender();
	}

	MAP::render();
}

void GameScene::update(const double& duration){
	if (!KEYBOARD::ifClamped(GLFW_KEY_LEFT_CONTROL)) {
		CAMERA::update(duration);
	}
	else {
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
	}
	m_GSTree.updatePositionCamera();

	if (!KEYBOARD::ifClamped(GLFW_KEY_LEFT_SHIFT)) {
		glm::vec2 direction = glm::vec2(0, 0);
		if (KEYBOARD::ifClamped(GLFW_KEY_W)) direction.y += 1.f;
		if (KEYBOARD::ifClamped(GLFW_KEY_S)) direction.y -= 1.f;
		if (KEYBOARD::ifClamped(GLFW_KEY_A)) direction.x -= 1.f;
		if (KEYBOARD::ifClamped(GLFW_KEY_D)) direction.x += 1.f;
		m_MainEntity->SetDirection(direction);
	}

	if (KEYBOARD::ifClamped(GLFW_KEY_Q)) {
		m_MainEntity->Rotate(duration/20);
	}
	if (KEYBOARD::ifClamped(GLFW_KEY_E)) {
		m_MainEntity->Rotate(-duration/20);
	}

	if (KEYBOARD::ifPressed(GLFW_KEY_F2)) {
		debug = !debug;
	}

	if (MOUSE::ifPressed(GLFW_MOUSE_BUTTON_LEFT)) {
		glm::vec2 sizeCam = Camera::getSize();
		glm::vec2 posCam = Camera::getCoords();
		glm::vec2 posmouse = MOUSE::getPosition();
		glm::vec2 posFromSizeCamera = posmouse * Camera::getSize() + glm::vec2(posCam.x-sizeCam.x/2, posCam.y - sizeCam.y/2);

		m_MainEntity->FollowWayTo(posFromSizeCamera);
	} 

	if (MOUSE::ifPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
		glm::vec2 sizeCam = Camera::getSize();
		glm::vec2 posCam = Camera::getCoords();
		glm::vec2 posmouse = MOUSE::getPosition();
		glm::vec2 posFromSizeCamera = posmouse * Camera::getSize() + glm::vec2(posCam.x - sizeCam.x / 2, posCam.y - sizeCam.y / 2);
		for (int i = 0; i < 8; i++) {
			std::shared_ptr<Projectile> bufferProjectile = std::make_shared<Projectile>(m_project->CopyProjectile());
			glm::vec2 direction = glm::normalize(posFromSizeCamera+glm::vec2(i,i) - m_MainEntity->GetPosition());

			bufferProjectile->SetPosition(m_MainEntity->GetPosition() + glm::vec2(direction.x * 8, direction.y * 8));
			bufferProjectile->SetDirection(direction);
			m_GSTree.addToTree(bufferProjectile);
		}
	}

	Entity::UpdateSyncIncrement();
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
