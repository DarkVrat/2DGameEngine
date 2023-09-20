#include "GameSpaceTree.h"

#include "DebugRender.h"
#include <iostream>

Collision GameSpaceTree::m_collisionForCamera;
GameSpaceTree* GameSpaceTree::GlobalGST=new GameSpaceTree();

void GameSpaceTree::updatePositionCamera() {
	m_collisionForCamera.SetPosition(CAMERA::getCoords());
}

void GameSpaceTree::updateSizeCamera() {
	glm::vec2 size = CAMERA::getSize();
	std::vector<glm::vec2> points;
	points.push_back(glm::vec2(size.x, size.y));
	points.push_back(glm::vec2(size.x, -size.y));
	points.push_back(glm::vec2(-size.x, -size.y));
	points.push_back(glm::vec2(-size.x, size.y));
	m_collisionForCamera.ShapeIsPolygon(points);
}

GameSpaceTree::GameSpaceTree(const glm::vec2& size) {
	std::vector<glm::vec2> points;
	points.push_back(glm::vec2(-size.x / 2.f, -size.y / 2.f));
	points.push_back(glm::vec2(-size.x / 2.f, size.y / 2.f));
	points.push_back(glm::vec2(size.x / 2.f, size.y / 2.f));
	points.push_back(glm::vec2(size.x / 2.f, -size.y / 2.f));
	m_collision.SetPosition(glm::vec2(size.x / 2.f, size.y / 2.f));
	m_collision.ShapeIsPolygon(points);

	std::vector<glm::vec2> colliderHorizont = {	glm::vec2(-size.x / 2.f - 32.f, -16.f),
												glm::vec2(-size.x / 2.f - 32.f,  16.f),
												glm::vec2(size.x / 2.f + 32.f,  16.f),
												glm::vec2(size.x / 2.f + 32.f, -16.f) };
	std::vector<glm::vec2> colliderVertical = { glm::vec2(-16.f, -size.y / 2.f - 32.f),
												glm::vec2(-16.f,  size.y / 2.f + 32.f),
												glm::vec2(16.f,  size.y / 2.f + 32.f),
												glm::vec2(16.f, -size.y / 2.f - 32.f) };

	m_Colliders.emplace(std::make_shared<Collider>(glm::vec2(-16.f, size.y / 2.f), colliderVertical), 0);
	m_Colliders.emplace(std::make_shared<Collider>(glm::vec2(size.x + 16.f, size.y / 2.f), colliderVertical), 0);
	m_Colliders.emplace(std::make_shared<Collider>(glm::vec2(size.x / 2.f, -16.f), colliderHorizont), 0);
	m_Colliders.emplace(std::make_shared<Collider>(glm::vec2(size.x / 2.f, size.y + 16.f), colliderHorizont), 0);

	m_layer = 0;
}

GameSpaceTree::GameSpaceTree(const GameSpaceTree& GST) {
	*this = GST;
}

GameSpaceTree::GameSpaceTree(GameSpaceTree&& GST) noexcept {
	*this = std::move(GST);
}

GameSpaceTree::~GameSpaceTree(){
	if (m_split) {
		delete m_LeftBottomTree;
		delete m_LeftTopTree;
		delete m_RightBottomTree;
		delete m_RightTopTree;
	}
	else {
		m_Entitys.clear();
		m_Objects.clear();
		m_Colliders.clear();
		m_Triggers.clear();
	}
}

void GameSpaceTree::operator=(const GameSpaceTree& GST){
	m_collision = GST.m_collision;
	m_Entitys = GST.m_Entitys;
	m_Objects = GST.m_Objects;
	m_Colliders = GST.m_Colliders;
	m_Triggers = GST.m_Triggers;
	m_LeftBottomTree = GST.m_LeftBottomTree;
	m_LeftTopTree = GST.m_LeftTopTree;
	m_RightBottomTree = GST.m_RightBottomTree;
	m_RightTopTree = GST.m_RightTopTree;
	m_LeftNeighbour = GST.m_LeftNeighbour;
	m_RightNeighbour = GST.m_RightNeighbour;
	m_TopNeighbour = GST.m_TopNeighbour;
	m_BottomNeighbour = GST.m_BottomNeighbour;
	m_parrent = GST.m_parrent;
	m_layer = GST.m_layer;
	m_split = GST.m_split;
}

void GameSpaceTree::operator=(GameSpaceTree&& GST) noexcept {
	m_collision = std::move(GST.m_collision);
	m_Entitys = std::move(GST.m_Entitys);
	m_Objects = std::move(GST.m_Objects);
	m_Colliders = std::move(GST.m_Colliders);
	m_Triggers = std::move(GST.m_Triggers);
	m_LeftBottomTree = std::move(GST.m_LeftBottomTree);
	m_LeftTopTree = std::move(GST.m_LeftTopTree);
	m_RightBottomTree = std::move(GST.m_RightBottomTree);
	m_RightTopTree = std::move(GST.m_RightTopTree);
	m_LeftNeighbour = std::move(GST.m_LeftNeighbour);
	m_RightNeighbour = std::move(GST.m_RightNeighbour);
	m_TopNeighbour = std::move(GST.m_TopNeighbour);
	m_BottomNeighbour = std::move(GST.m_BottomNeighbour);
	m_parrent = std::move(GST.m_parrent);
	m_layer = std::move(GST.m_layer);
	m_split = std::move(GST.m_split);
}

void GameSpaceTree::Render(){
	if (m_split) {
		if (m_LeftBottomTree->m_collision.CheckCollision(m_collisionForCamera, ONLY_COLLISION).hasCollision) 
			m_LeftBottomTree->Render();
		if (m_LeftTopTree->m_collision.CheckCollision(m_collisionForCamera, ONLY_COLLISION).hasCollision) 
			m_LeftTopTree->Render();
		if (m_RightTopTree->m_collision.CheckCollision(m_collisionForCamera, ONLY_COLLISION).hasCollision) 
			m_RightTopTree->Render();
		if (m_RightBottomTree->m_collision.CheckCollision(m_collisionForCamera, ONLY_COLLISION).hasCollision) 
			m_RightBottomTree->Render();

		return;
	}

	//TODO: рендеринг объектов
}

void GameSpaceTree::DebugRender(){
	if (m_split) {
		if (m_LeftBottomTree->m_collision.CheckCollision(m_collisionForCamera, ONLY_COLLISION).hasCollision) 
			m_LeftBottomTree->DebugRender();
		if (m_LeftTopTree->m_collision.CheckCollision(m_collisionForCamera, ONLY_COLLISION).hasCollision) 
			m_LeftTopTree->DebugRender();
		if (m_RightTopTree->m_collision.CheckCollision(m_collisionForCamera, ONLY_COLLISION).hasCollision) 
			m_RightTopTree->DebugRender();
		if (m_RightBottomTree->m_collision.CheckCollision(m_collisionForCamera, ONLY_COLLISION).hasCollision) 
			m_RightBottomTree->DebugRender();

		return;
	}

	DebugRender::drawShape(m_collision);

	for (auto& entity : m_Entitys) {
		DebugRender::drawShape(*entity.first, glm::vec4(1, 0, 1, 1));
	}
	for (auto& object : m_Objects) {
		DebugRender::drawShape(*object.first, glm::vec4(0, 0, 1, 1));
	}
	for (auto& collider : m_Colliders) {
		DebugRender::drawShape(*collider.first, glm::vec4(0, 1, 1, 1));
	}
	for (auto& trigger : m_Triggers) {
		DebugRender::drawShape(*trigger.first, glm::vec4(1, 0, 0, 1));
	}

}

void GameSpaceTree::Update(const double& duration){
	if (m_split && m_LeftTopTree->m_collision.CheckCollision(m_collisionForCamera, ONLY_COLLISION).hasCollision)	m_LeftTopTree->Update(duration);
	if (m_split && m_LeftBottomTree->m_collision.CheckCollision(m_collisionForCamera, ONLY_COLLISION).hasCollision) m_LeftBottomTree->Update(duration);
	if (m_split && m_RightTopTree->m_collision.CheckCollision(m_collisionForCamera, ONLY_COLLISION).hasCollision)	m_RightTopTree->Update(duration);
	if (m_split && m_RightBottomTree->m_collision.CheckCollision(m_collisionForCamera, ONLY_COLLISION).hasCollision)m_RightBottomTree->Update(duration);
	if (m_split) return;
	

	for (auto& entity : m_Entitys) {
		entity.first->Update(duration);
	}

	for (auto it = m_Entitys.begin(); it != m_Entitys.end();) {
		auto& entity = *it;

		if (!m_collision.CheckCollision(*entity.first).hasCollision) {
			it++;

			if (ifTeleport(entity)) 
				TeleportEvent(entity);
			else 
				MovingEvent(entity);

			if (m_parrent != nullptr && m_parrent->link())
				return;
		}
		else if (entity.first->IsBreak()) {
			it++;
			GlobalGST->DeleteEvent(entity.first);
			if (m_parrent != nullptr && m_parrent->link())
				return;
		}
		else {
			Update(entity);
			updateSides(entity.second, entity.first);
			++it;
		}
	}
}

void GameSpaceTree::Update(std::pair<std::shared_ptr<Entity>, uint8_t> entityForUpdate) {
	for (auto& entity : m_Entitys) {
		if (entityForUpdate.first == entity.first) {
			break;
		}
		CollisionController::CheckCollision(*entityForUpdate.first, *entity.first);
	}
	for (auto& collider : m_Colliders) {
		CollisionController::CheckCollision(*entityForUpdate.first, *collider.first);
	}
	for (auto& object : m_Objects) {
		CollisionController::CheckCollision(*entityForUpdate.first, *object.first);
	}
	for (auto& trigger : m_Triggers) {
		CollisionController::CheckCollision(*entityForUpdate.first, *trigger.first);
		if (trigger.first->getStopWork()) m_Triggers.erase(trigger.first);
	}
}

GameSpaceTree::GameSpaceTree(const std::vector<glm::vec2>& points, const glm::vec2 leftBottomPoint, GameSpaceTree* parrent) {
	m_collision.SetPosition(leftBottomPoint);
	m_collision.ShapeIsPolygon(points);
	m_parrent = parrent;
	m_layer = parrent->m_layer + 1;
}

bool GameSpaceTree::split() {
	if (m_Entitys.size() <= MAX_ENTITY || m_layer >= MAX_LAYER) 
		return false;

	m_split = true;
	glm::vec2 position = m_collision.GetPosition();
	std::vector<glm::vec2> points = m_collision.GetPoints();
	for (auto& point : points) point = point / 2.f;

	m_LeftBottomTree = new GameSpaceTree(points, position + points[0], this);
	m_LeftTopTree = new GameSpaceTree(points, position + points[1], this);
	m_RightTopTree = new GameSpaceTree(points, position + points[2], this);
	m_RightBottomTree = new GameSpaceTree(points, position + points[3], this);

	m_LeftBottomTree->setNeighbours(m_LeftNeighbour, m_RightBottomTree, m_LeftTopTree, m_BottomNeighbour);
	m_LeftTopTree->setNeighbours(m_LeftNeighbour, m_RightTopTree, m_TopNeighbour, m_LeftBottomTree);
	m_RightTopTree->setNeighbours(m_LeftTopTree, m_RightNeighbour, m_TopNeighbour, m_RightBottomTree);
	m_RightBottomTree->setNeighbours(m_LeftBottomTree, m_RightNeighbour, m_RightTopTree, m_BottomNeighbour);

	for (auto& entity : m_Entitys) distributionObject(entity.first);
	for (auto& object : m_Objects) distributionObject(object.first);
	for (auto& collider : m_Colliders) distributionObject(collider.first);
	for (auto& trigger : m_Triggers) distributionObject(trigger.first);

	m_Entitys.clear();
	m_Objects.clear();
	m_Colliders.clear();
	m_Triggers.clear();

	return true;
}

bool GameSpaceTree::link() {
	if (!m_split || m_LeftBottomTree->m_split || m_LeftTopTree->m_split || m_RightTopTree->m_split || m_RightBottomTree->m_split ||
		m_LeftBottomTree->m_Entitys.size() + m_LeftTopTree->m_Entitys.size() + m_RightTopTree->m_Entitys.size() + m_RightBottomTree->m_Entitys.size() > MAX_ENTITY) {
		return false;
	}

	m_split = false;

	linkedMap(m_Entitys, m_LeftBottomTree->m_Entitys, m_LeftTopTree->m_Entitys, m_RightTopTree->m_Entitys, m_RightBottomTree->m_Entitys);
	linkedMap(m_Objects, m_LeftBottomTree->m_Objects, m_LeftTopTree->m_Objects, m_RightTopTree->m_Objects, m_RightBottomTree->m_Objects);
	linkedMap(m_Colliders, m_LeftBottomTree->m_Colliders, m_LeftTopTree->m_Colliders, m_RightTopTree->m_Colliders, m_RightBottomTree->m_Colliders);
	linkedMap(m_Triggers, m_LeftBottomTree->m_Triggers, m_LeftTopTree->m_Triggers, m_RightTopTree->m_Triggers, m_RightBottomTree->m_Triggers);

	for (auto& entity : m_Entitys) {setSides(entity.second, entity.first);}
	for (auto& objects : m_Objects)	{setSides(objects.second, objects.first);}
	for (auto& collider : m_Colliders) {setSides(collider.second, collider.first);}
	for (auto& trigger : m_Triggers) {setSides(trigger.second, trigger.first);}

	delete m_LeftBottomTree;
	delete m_LeftTopTree;
	delete m_RightTopTree;
	delete m_RightBottomTree;

	m_LeftBottomTree = nullptr;
	m_LeftTopTree = nullptr;
	m_RightTopTree = nullptr;
	m_RightBottomTree = nullptr;

	return true;
}

void GameSpaceTree::setNeighbours(GameSpaceTree* LeftNeighbour, GameSpaceTree* RightNeighbour, GameSpaceTree* TopNeighbour, GameSpaceTree* BottomNeighbour) {
	m_LeftNeighbour = LeftNeighbour;
	m_RightNeighbour = RightNeighbour;
	m_TopNeighbour = TopNeighbour;
	m_BottomNeighbour = BottomNeighbour;
}

void GameSpaceTree::DeleteEvent(std::shared_ptr<Entity> entity){
	if (m_split) {
		if (m_LeftBottomTree->m_collision.CheckCollision(*entity, ONLY_COLLISION).hasCollision)
			m_LeftBottomTree->DeleteEvent(entity);
		if (m_LeftTopTree->m_collision.CheckCollision(*entity, ONLY_COLLISION).hasCollision)
			m_LeftTopTree->DeleteEvent(entity);
		if (m_RightTopTree->m_collision.CheckCollision(*entity, ONLY_COLLISION).hasCollision)
			m_RightTopTree->DeleteEvent(entity);
		if (m_RightBottomTree->m_collision.CheckCollision(*entity, ONLY_COLLISION).hasCollision)
			m_RightBottomTree->DeleteEvent(entity);
		return;
	}

	m_Entitys.erase(entity);
}

void GameSpaceTree::TeleportEvent(std::pair<const std::shared_ptr<Entity>, uint8_t> entity){
	TeleportEventHelp(entity.first);
	GlobalGST->addToTree(entity.first);
}

void GameSpaceTree::TeleportEventHelp(const std::shared_ptr<Entity> entity){
	if (m_split) {
		m_LeftTopTree->TeleportEventHelp(entity);
		m_RightTopTree->TeleportEventHelp(entity);
		m_LeftBottomTree->TeleportEventHelp(entity);
		m_RightBottomTree->TeleportEventHelp(entity);
		return;
	}

	auto it = m_Entitys.find(entity);
	if (it != m_Entitys.end()) {
		uint8_t buffer = it->second;
		m_Entitys.erase(entity);
		if (buffer & SIDES_TOP) m_TopNeighbour->TeleportEventHelp(entity);
		if (buffer & SIDES_LEFT) m_LeftNeighbour->TeleportEventHelp(entity);
		if (buffer & SIDES_RIGHT) m_RightNeighbour->TeleportEventHelp(entity);
		if (buffer & SIDES_BOTTOM) m_BottomNeighbour->TeleportEventHelp(entity);
	}
}

void GameSpaceTree::MovingEvent(std::pair<const std::shared_ptr<Entity>, uint8_t> entity){
	m_Entitys.erase(entity.first);
	if (entity.second & SIDES_TOP) m_TopNeighbour->MovingEventHelp(entity.first);
	if (entity.second & SIDES_LEFT) m_LeftNeighbour->MovingEventHelp(entity.first);
	if (entity.second & SIDES_RIGHT) m_RightNeighbour->MovingEventHelp(entity.first);
	if (entity.second & SIDES_BOTTOM) m_BottomNeighbour->MovingEventHelp(entity.first);
}

void GameSpaceTree::MovingEventHelp(const std::shared_ptr<Entity> entity){
	if (m_split) {
		if (m_LeftBottomTree->m_collision.CheckCollision(*entity, ONLY_COLLISION).hasCollision)
			m_LeftBottomTree->MovingEventHelp(entity);
		if (m_LeftTopTree->m_collision.CheckCollision(*entity, ONLY_COLLISION).hasCollision)
			m_LeftTopTree->MovingEventHelp(entity);
		if (m_RightTopTree->m_collision.CheckCollision(*entity, ONLY_COLLISION).hasCollision)
			m_RightTopTree->MovingEventHelp(entity);
		if (m_RightBottomTree->m_collision.CheckCollision(*entity, ONLY_COLLISION).hasCollision)
			m_RightBottomTree->MovingEventHelp(entity);
		return;
	}

	auto& it = m_Entitys.find(entity);
	if (it != m_Entitys.end()) {
		it->second = 0;
		updateSides(it->second, it->first);
	}
	else {
		auto& newEntity = m_Entitys.emplace(entity, 0).first;
		setSides(newEntity->second, newEntity->first);
	}
}

bool GameSpaceTree::ifTeleport(std::pair<const std::shared_ptr<Entity>, uint8_t> entity){
	if (entity.second) {
		if ((entity.second & SIDES_TOP) && m_TopNeighbour->m_collision.CheckCollision(*entity.first, ONLY_COLLISION).hasCollision) return false;
		if ((entity.second & SIDES_LEFT) && m_LeftNeighbour->m_collision.CheckCollision(*entity.first, ONLY_COLLISION).hasCollision) return false;
		if ((entity.second & SIDES_RIGHT) && m_RightNeighbour->m_collision.CheckCollision(*entity.first, ONLY_COLLISION).hasCollision) return false;
		if ((entity.second & SIDES_BOTTOM) && m_BottomNeighbour->m_collision.CheckCollision(*entity.first, ONLY_COLLISION).hasCollision) return false;
	}
	return true;
}

void GameSpaceTree::addToTree(const std::shared_ptr<Entity>& entity){
	if (!m_split) {
		auto& sides = m_Entitys.emplace(entity, 0).first->second;
		if (!split())
			setSides(sides, entity);
	}
	else
		distributionObject<Entity>(entity);
}

void GameSpaceTree::addToTree(const std::shared_ptr<Object>& object) {
	if (!m_split) {
		auto& sides = m_Objects.emplace(object, 0).first->second;
		setSides(sides, object);
	}
	else
		distributionObject<Object>(object);
}

void GameSpaceTree::addToTree(const std::shared_ptr<Collider>& collider) {
	if (!m_split) {
		auto& sides = m_Colliders.emplace(collider, 0).first->second;
		setSides(sides, collider);
	}
	else
		distributionObject<Collider>(collider);
}

void GameSpaceTree::addToTree(const std::shared_ptr<Trigger>& trigger) {
	if (!m_split) {
		auto& sides = m_Triggers.emplace(trigger, 0).first->second;
		setSides(sides, trigger);
	}
	else
		distributionObject<Trigger>(trigger);
}
