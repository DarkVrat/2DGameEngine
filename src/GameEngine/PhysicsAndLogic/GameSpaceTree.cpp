#include "GameSpaceTree.h"

#include "DebugRender.h"

#define MAX_ENTITY 15

Collision GameSpaceTree::m_collisionForCamera;

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
	points.push_back(glm::vec2(0, 0));
	points.push_back(glm::vec2(0, size.y));
	points.push_back(glm::vec2(size.x, size.y));
	points.push_back(glm::vec2(size.x, 0));
	m_collision.SetPosition(glm::vec2(0, 0));
	m_collision.ShapeIsPolygon(points);

	m_Colliders.insert(std::make_shared<Collider>(glm::vec2(0, 0), std::vector<glm::vec2>({ points[0], points[1] })));
	m_Colliders.insert(std::make_shared<Collider>(glm::vec2(0, 0), std::vector<glm::vec2>({ points[1], points[2] })));
	m_Colliders.insert(std::make_shared<Collider>(glm::vec2(0, 0), std::vector<glm::vec2>({ points[2], points[3] })));
	m_Colliders.insert(std::make_shared<Collider>(glm::vec2(0, 0), std::vector<glm::vec2>({ points[3], points[0] })));
}

GameSpaceTree::GameSpaceTree(const GameSpaceTree& GST) {
	*this = GST;
}

GameSpaceTree::GameSpaceTree(GameSpaceTree&& GST) noexcept {
	*this = std::move(GST);
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
}

void GameSpaceTree::operator=(GameSpaceTree&& GST) noexcept{
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
}

void GameSpaceTree::Render(){
	if (m_LeftBottomTree != nullptr) {
		if (m_LeftBottomTree->m_collision.CheckCollision(m_collisionForCamera).hasCollision) m_LeftBottomTree->Render();
		if (m_LeftTopTree->m_collision.CheckCollision(m_collisionForCamera).hasCollision) m_LeftTopTree->Render();
		if (m_RightTopTree->m_collision.CheckCollision(m_collisionForCamera).hasCollision) m_RightTopTree->Render();
		if (m_RightBottomTree->m_collision.CheckCollision(m_collisionForCamera).hasCollision) m_RightBottomTree->Render();

		return;
	}

	//TODO: рендеринг объектов
}

void GameSpaceTree::DebugRender(){
	if (m_LeftBottomTree != nullptr) {
		if (m_LeftBottomTree->m_collision.CheckCollision(m_collisionForCamera).hasCollision) m_LeftBottomTree->DebugRender();
		if (m_LeftTopTree->m_collision.CheckCollision(m_collisionForCamera).hasCollision) m_LeftTopTree->DebugRender();
		if (m_RightTopTree->m_collision.CheckCollision(m_collisionForCamera).hasCollision) m_RightTopTree->DebugRender();
		if (m_RightBottomTree->m_collision.CheckCollision(m_collisionForCamera).hasCollision) m_RightBottomTree->DebugRender();

		return;
	}

	DebugRender::drawShape(m_collision);

	for (auto& entity : m_Entitys) {
		DebugRender::drawShape(*entity, glm::vec4(0, 1, 0, 1));
	}
	for (auto& object : m_Objects) {
		DebugRender::drawShape(*object, glm::vec4(0, 0, 1, 1));
	}
	for (auto& collider : m_Colliders) {
		DebugRender::drawShape(*collider, glm::vec4(0, 1, 1, 1));
	}
	for (auto& trigger : m_Triggers) {
		DebugRender::drawShape(*trigger, glm::vec4(1, 0, 0, 1));
	}

}

void GameSpaceTree::Update(const double& duration){
	if (m_LeftBottomTree != nullptr && m_LeftBottomTree->m_collision.CheckCollision(m_collisionForCamera).hasCollision) m_LeftBottomTree->Update(duration);
	if (m_LeftTopTree != nullptr && m_LeftTopTree->m_collision.CheckCollision(m_collisionForCamera).hasCollision) m_LeftTopTree->Update(duration);
	if (m_RightTopTree != nullptr && m_RightTopTree->m_collision.CheckCollision(m_collisionForCamera).hasCollision) m_RightTopTree->Update(duration);
	if (m_RightBottomTree != nullptr && m_RightBottomTree->m_collision.CheckCollision(m_collisionForCamera).hasCollision) m_RightBottomTree->Update(duration);
	if (m_RightBottomTree != nullptr) return;
	

	for (auto it = m_Entitys.begin(); it != m_Entitys.end();) {
		auto& entity = *it;

		if (m_LeftNeighbour != nullptr && m_LeftNeighbour->m_collision.CheckCollision(*entity).hasCollision)
			m_LeftNeighbour->addToTree(entity);

		if (m_RightNeighbour != nullptr && m_RightNeighbour->m_collision.CheckCollision(*entity).hasCollision)
			m_RightNeighbour->addToTree(entity);

		if (m_TopNeighbour != nullptr && m_TopNeighbour->m_collision.CheckCollision(*entity).hasCollision)
			m_TopNeighbour->addToTree(entity);

		if (m_BottomNeighbour != nullptr && m_BottomNeighbour->m_collision.CheckCollision(*entity).hasCollision)
			m_BottomNeighbour->addToTree(entity);

		if (!m_collision.CheckCollision(*entity).hasCollision) {
			it = m_Entitys.erase(it);
			GameSpaceTree* parrent = m_parrent;
			m_parrent->link();
			break;
		}

		Update(entity, duration);

		++it;
	}
}

void GameSpaceTree::Update(std::shared_ptr<Entity> entityForUpdate, const double& duration){
	for (auto& entity : m_Entitys) {
		if (entityForUpdate == entity) {
			break;
		}
		entityForUpdate->CheckCollision(*entity);
	}
	for (auto& collider : m_Colliders) {
		entityForUpdate->CheckCollision(*collider);
	}
	for (auto& object : m_Objects) {
		entityForUpdate->CheckCollision(*object);
	}
	for (auto& trigger: m_Triggers) {
		entityForUpdate->CheckCollision(*trigger, duration);
		if (trigger->getStopWork()) m_Triggers.erase(trigger);
	}
}

GameSpaceTree::GameSpaceTree(const std::vector<glm::vec2>& points, const glm::vec2 leftBottomPoint, GameSpaceTree* parrent) {
	m_collision.SetPosition(leftBottomPoint);
	m_collision.ShapeIsPolygon(points);
	m_parrent = parrent;
}

void GameSpaceTree::split() {
	if (m_Entitys.size() <= MAX_ENTITY) 
		return;

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

	for (auto& entity : m_Entitys) distributionObject<Entity>(entity);
	for (auto& object : m_Objects) distributionObject<Object>(object);
	for (auto& collider : m_Colliders) distributionObject<Collider>(collider);
	for (auto& trigger : m_Triggers) distributionObject<Trigger>(trigger);

	m_Entitys.clear();
	m_Objects.clear();
	m_Colliders.clear();
	m_Triggers.clear();
}

bool GameSpaceTree::link() {
	if (m_LeftBottomTree->m_LeftBottomTree != nullptr || 
		m_LeftTopTree->m_LeftTopTree != nullptr ||
		m_RightTopTree->m_RightTopTree != nullptr ||
		m_RightBottomTree->m_RightBottomTree != nullptr ||
		m_LeftBottomTree->m_Entitys.size() + m_LeftTopTree->m_Entitys.size() + m_RightBottomTree->m_Entitys.size() + m_RightTopTree->m_Entitys.size() > MAX_ENTITY ) {
		return false;
	}

	linkedSet<Entity>(m_Entitys, m_LeftBottomTree->m_Entitys, m_LeftTopTree->m_Entitys, m_RightTopTree->m_Entitys, m_RightBottomTree->m_Entitys);
	linkedSet<Object>(m_Objects, m_LeftBottomTree->m_Objects, m_LeftTopTree->m_Objects, m_RightTopTree->m_Objects, m_RightBottomTree->m_Objects);
	linkedSet<Collider>(m_Colliders, m_LeftBottomTree->m_Colliders, m_LeftTopTree->m_Colliders, m_RightTopTree->m_Colliders, m_RightBottomTree->m_Colliders);
	linkedSet<Trigger>(m_Triggers, m_LeftBottomTree->m_Triggers, m_LeftTopTree->m_Triggers, m_RightTopTree->m_Triggers, m_RightBottomTree->m_Triggers);

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

void GameSpaceTree::addToTree(const std::shared_ptr<Entity>& entity){
	if (m_LeftBottomTree == nullptr) {
		m_Entitys.insert(entity);
		split();
	}
	else
		distributionObject<Entity>(entity);
}

void GameSpaceTree::addToTree(const std::shared_ptr<Object>& object) {
	if (m_LeftBottomTree == nullptr) 
		m_Objects.insert(object);
	else
		distributionObject<Object>(object);
}

void GameSpaceTree::addToTree(const std::shared_ptr<Collider>& collider) {
	if (m_LeftBottomTree == nullptr) 
		m_Colliders.insert(collider);
	else
		distributionObject<Collider>(collider);
}

void GameSpaceTree::addToTree(const std::shared_ptr<Trigger>& trigger) {
	if (m_LeftBottomTree == nullptr) 
		m_Triggers.insert(trigger);
	else
		distributionObject<Trigger>(trigger);
}

void GameSpaceTree::addToTree(const Entity& entity) { addToTree(std::make_shared<Entity>(entity)); }
void GameSpaceTree::addToTree(const Object& object) { addToTree(std::make_shared<Object>(object)); }
void GameSpaceTree::addToTree(const Collider& collider) { addToTree(std::make_shared<Collider>(collider)); }
void GameSpaceTree::addToTree(const Trigger& trigger) { addToTree(std::make_shared<Trigger>(trigger)); }
