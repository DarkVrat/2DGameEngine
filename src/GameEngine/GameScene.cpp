#include "GameScene.h"

#include "../Renderer/MapRender.h"
#include "../Control/KeyboardControl.h"
#include "../Control/MouseControl.h"
#include "PhysicsAndLogic/WaySearch.h"
#include "Objects/Camera.h"
#include "PhysicsAndLogic/DebugRender.h"
#include <glm/gtc/matrix_transform.hpp>

GameScene::GameScene(){
	
}

void GameScene::init(const std::string& gameScene){
	std::string jsonPath = "res/Scenes/" + gameScene + ".json";

	clear();

	m_Resources.load(jsonPath);

	glm::vec2 sezeMap = MAP::getSize();
	GameSpaceTree::GlobalGST = new GameSpaceTree(sezeMap);

	EntityData dat1(1.f, 20.f, 128.f, 0.7f, 50.f);
	std::vector<glm::vec2> vecPol;
	vecPol.push_back(glm::vec2(8, 8));
	vecPol.push_back(glm::vec2(8, -8));
	vecPol.push_back(glm::vec2(-8, -8));
	vecPol.push_back(glm::vec2(-8, 8));
	m_MainEntity = std::make_shared<Character>(dat1, glm::vec2(504, 504), vecPol);
	m_MainEntity->setSpecial(true);
	GameSpaceTree::GlobalGST->addToTree(m_MainEntity);
	GameSpaceTree::GlobalGST->updatePositionCamera();
	GameSpaceTree::GlobalGST->updateSizeCamera();
	CAMERA::setFollowingEntity(m_MainEntity);
	CAMERA::setCoords(glm::vec2(504, 504));
	CAMERA::setSettings(glm::vec2(32, 32), 0.0003, 0.0008); 

	TriggerData tData(true, true, [](Entity& entity) {entity.SetPosition(glm::vec2(504, 504)); });
	GameSpaceTree::GlobalGST->addToTree(std::make_shared<Trigger>(tData, glm::vec2(8, 8), vecPol));

	std::function<void(Projectile&, Character&)> functionP = [](Projectile& p, Character& c) {
		c.AddImpulse(glm::vec2(p.getDirection().x*1000, p.getDirection().y * 1000)); 
		c.addDamage(0.7f);
	};

	ProjectileMoveController moveScript;
	moveScript.addMoveInstruction([](Projectile& p, const double& d) {p.Rotate(glm::degrees(glm::atan(p.getDirection().y, p.getDirection().x))); });
	moveScript.addMoveInstruction([](Projectile& p, const double& d) {p.UpdateAsEntity(d); p.Rotate(d); }, 10000);

	m_project = std::make_shared<Projectile>(ProjectileData(true, false, false, false, 1.f, 1, functionP, moveScript), EntityData(1, 1, 128, 0, 1), glm::vec2(-1, -1));
	std::vector<glm::vec2> vecProj;
	vecProj.push_back(glm::vec2(-1, 1));
	vecProj.push_back(glm::vec2(-1, -1));
	vecProj.push_back(glm::vec2(15, 0));
	m_project->ShapeIsPolygon(vecProj);
} 
 
void GameScene::render(){
	
	if (debug) {
		DebugRender::updateUniform();
		GameSpaceTree::GlobalGST->DebugRender();
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
	GameSpaceTree::GlobalGST->updatePositionCamera();

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

		EntityData dat1(1.f, 20.f, 128.f, 0.7f, 50.f);
		std::vector<glm::vec2> vecPol;
		vecPol.push_back(glm::vec2(8, 8));
		vecPol.push_back(glm::vec2(8, -8));
		vecPol.push_back(glm::vec2(-8, -8));
		vecPol.push_back(glm::vec2(-8, 8));
		std::shared_ptr<Character> ent = std::make_shared<Character>(dat1, posFromSizeCamera, vecPol);
		ent->SetTeam(2);
		GameSpaceTree::GlobalGST->addToTree(ent);
	} 

	if (MOUSE::ifPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
		glm::vec2 sizeCam = Camera::getSize();
		glm::vec2 posCam = Camera::getCoords();
		glm::vec2 posmouse = MOUSE::getPosition();
		glm::vec2 posFromSizeCamera = posmouse * Camera::getSize() + glm::vec2(posCam.x - sizeCam.x / 2, posCam.y - sizeCam.y / 2);
		for (int i = 0; i < 1; i++) {
			std::shared_ptr<Projectile> bufferProjectile = std::make_shared<Projectile>(m_project->CopyProjectile());
			glm::vec2 direction = glm::normalize(posFromSizeCamera+glm::vec2(i,i) - m_MainEntity->GetPosition());

			bufferProjectile->SetPosition(m_MainEntity->GetPosition() + glm::vec2(direction.x * 8, direction.y * 8));
			bufferProjectile->SetDirection(direction);
			GameSpaceTree::GlobalGST->addToTree(bufferProjectile);
		}
	}

	Entity::UpdateSyncIncrement();
	GameSpaceTree::GlobalGST->Update(duration);
}

void GameScene::events(){
	float s = MOUSE::getScroll().y;
	if (std::abs(s) > 0) {
		CAMERA::setSize(CAMERA::getSize().x * (20-s)/20);
		GameSpaceTree::GlobalGST->updateSizeCamera();
	}
}

void GameScene::clear(){
	delete GameSpaceTree::GlobalGST;
}
