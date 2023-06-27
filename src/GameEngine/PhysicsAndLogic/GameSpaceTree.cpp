#include "GameSpaceTree.h"

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
}

void GameSpaceTree::Render(){
}

void GameSpaceTree::DebugRender()
{
}

void GameSpaceTree::Update(const double& duration)
{
}

GameSpaceTree::GameSpaceTree(const std::vector<glm::vec2>& points, const glm::vec2 leftBottomPoint, GameSpaceTree* parrent) {
	m_collision.SetPosition(leftBottomPoint);
	m_collision.ShapeIsPolygon(points);
	m_parrent = parrent;
}

void GameSpaceTree::split() {
	glm::vec2 position = m_collision.GetPosition();
	std::vector<glm::vec2> points = m_collision.GetPoints();
	for (auto& point : points) {
		point = point / 2.f;
	}

	m_LeftBottomTree = new GameSpaceTree(points, position + points[0], this);
	m_LeftTopTree = new GameSpaceTree(points, position + points[1], this);
	m_RightTopTree = new GameSpaceTree(points, position + points[2], this);
	m_RightBottomTree = new GameSpaceTree(points, position + points[3], this);

	m_LeftBottomTree->setNeighbours(m_LeftNeighbour, m_RightBottomTree, m_LeftTopTree, m_BottomNeighbour);
	m_LeftTopTree->setNeighbours(m_LeftNeighbour, m_RightTopTree, m_TopNeighbour, m_LeftBottomTree);
	m_RightTopTree->setNeighbours(m_LeftTopTree, m_RightNeighbour, m_TopNeighbour, m_RightBottomTree);
	m_RightBottomTree->setNeighbours(m_LeftBottomTree, m_RightNeighbour, m_RightTopTree, m_BottomNeighbour);

	distributionVector<Entity>(m_Entitys);
	distributionVector<Object>(m_Objects);
	distributionVector<Collider>(m_Colliders);
	distributionVector<Trigger>(m_Triggers);

	if (m_LeftBottomTree->m_collision.CheckCollision(*m_MainEntity).hasCollision) m_LeftBottomTree->m_MainEntity = m_MainEntity;
	if (m_LeftTopTree->m_collision.CheckCollision(*m_MainEntity).hasCollision) m_LeftTopTree->m_MainEntity = m_MainEntity;
	if (m_RightTopTree->m_collision.CheckCollision(*m_MainEntity).hasCollision) m_RightTopTree->m_MainEntity = m_MainEntity;
	if (m_RightBottomTree->m_collision.CheckCollision(*m_MainEntity).hasCollision) m_RightBottomTree->m_MainEntity = m_MainEntity;

	m_MainEntity = nullptr;
	m_Entitys.clear();
	m_Objects.clear();
	m_Colliders.clear();
	m_Triggers.clear();
}

void GameSpaceTree::link() {
	if (m_LeftBottomTree->m_MainEntity != nullptr) m_MainEntity = m_LeftBottomTree->m_MainEntity;
	else if (m_LeftTopTree->m_MainEntity != nullptr) m_MainEntity = m_LeftTopTree->m_MainEntity;
	else if (m_RightTopTree->m_MainEntity != nullptr) m_MainEntity = m_RightTopTree->m_MainEntity;
	else m_MainEntity = m_RightBottomTree->m_MainEntity;

	linkedVectot<Entity>(m_Entitys, m_LeftBottomTree->m_Entitys, m_LeftTopTree->m_Entitys, m_RightTopTree->m_Entitys, m_RightBottomTree->m_Entitys);
	linkedVectot<Object>(m_Objects, m_LeftBottomTree->m_Objects, m_LeftTopTree->m_Objects, m_RightTopTree->m_Objects, m_RightBottomTree->m_Objects);
	linkedVectot<Collider>(m_Colliders, m_LeftBottomTree->m_Colliders, m_LeftTopTree->m_Colliders, m_RightTopTree->m_Colliders, m_RightBottomTree->m_Colliders);
	linkedVectot<Trigger>(m_Triggers, m_LeftBottomTree->m_Triggers, m_LeftTopTree->m_Triggers, m_RightTopTree->m_Triggers, m_RightBottomTree->m_Triggers);

	delete m_LeftBottomTree;
	delete m_LeftTopTree;
	delete m_RightTopTree;
	delete m_RightBottomTree;

	m_LeftBottomTree = nullptr;
	m_LeftTopTree = nullptr;
	m_RightTopTree = nullptr;
	m_RightBottomTree = nullptr;
}

void GameSpaceTree::setNeighbours(GameSpaceTree* LeftNeighbour, GameSpaceTree* RightNeighbour, GameSpaceTree* TopNeighbour, GameSpaceTree* BottomNeighbour) {
	m_LeftNeighbour = LeftNeighbour;
	m_RightNeighbour = RightNeighbour;
	m_TopNeighbour = TopNeighbour;
	m_BottomNeighbour = BottomNeighbour;
}

void GameSpaceTree::addMainEntity(const std::shared_ptr<Entity>& entity){
	if (m_LeftBottomTree == nullptr) {
		m_MainEntity = entity;
		return;
	}

	if (m_LeftBottomTree->m_collision.CheckCollision(*entity).hasCollision) m_LeftBottomTree->addMainEntity(entity);
	if (m_LeftTopTree->m_collision.CheckCollision(*entity).hasCollision) m_LeftTopTree->addMainEntity(entity);
	if (m_RightTopTree->m_collision.CheckCollision(*entity).hasCollision) m_RightTopTree->addMainEntity(entity);
	if (m_RightBottomTree->m_collision.CheckCollision(*entity).hasCollision) m_RightBottomTree->addMainEntity(entity);
}

void GameSpaceTree::addToTree(const std::shared_ptr<Entity>& entity){
	if (m_LeftBottomTree == nullptr) {
		m_Entitys.push_back(entity);
		if (m_Entitys.size() > 16) {
			split();
		}
		return;
	}

	if (m_LeftBottomTree->m_collision.CheckCollision(*entity).hasCollision) m_LeftBottomTree->addToTree(entity);
	if (m_LeftTopTree->m_collision.CheckCollision(*entity).hasCollision) m_LeftTopTree->addToTree(entity);
	if (m_RightTopTree->m_collision.CheckCollision(*entity).hasCollision) m_RightTopTree->addToTree(entity);
	if (m_RightBottomTree->m_collision.CheckCollision(*entity).hasCollision) m_RightBottomTree->addToTree(entity);
}

void GameSpaceTree::addToTree(const std::shared_ptr<Object>& object) {
	if (m_LeftBottomTree == nullptr) {
		m_Objects.push_back(object);
		return;
	}

	if (m_LeftBottomTree->m_collision.CheckCollision(*object).hasCollision) m_LeftBottomTree->addToTree(object);
	if (m_LeftTopTree->m_collision.CheckCollision(*object).hasCollision) m_LeftTopTree->addToTree(object);
	if (m_RightTopTree->m_collision.CheckCollision(*object).hasCollision) m_RightTopTree->addToTree(object);
	if (m_RightBottomTree->m_collision.CheckCollision(*object).hasCollision) m_RightBottomTree->addToTree(object);
}

void GameSpaceTree::addToTree(const std::shared_ptr<Collider>& collider) {
	if (m_LeftBottomTree == nullptr) {
		m_Colliders.push_back(collider);
		return;
	}

	if (m_LeftBottomTree->m_collision.CheckCollision(*collider).hasCollision) m_LeftBottomTree->addToTree(collider);
	if (m_LeftTopTree->m_collision.CheckCollision(*collider).hasCollision) m_LeftTopTree->addToTree(collider);
	if (m_RightTopTree->m_collision.CheckCollision(*collider).hasCollision) m_RightTopTree->addToTree(collider);
	if (m_RightBottomTree->m_collision.CheckCollision(*collider).hasCollision) m_RightBottomTree->addToTree(collider);
}

void GameSpaceTree::addToTree(const std::shared_ptr<Trigger>& trigger) {
	if (m_LeftBottomTree == nullptr) {
		m_Triggers.push_back(trigger);
		return;
	}

	if (m_LeftBottomTree->m_collision.CheckCollision(*trigger).hasCollision) m_LeftBottomTree->addToTree(trigger);
	if (m_LeftTopTree->m_collision.CheckCollision(*trigger).hasCollision) m_LeftTopTree->addToTree(trigger);
	if (m_RightTopTree->m_collision.CheckCollision(*trigger).hasCollision) m_RightTopTree->addToTree(trigger);
	if (m_RightBottomTree->m_collision.CheckCollision(*trigger).hasCollision) m_RightBottomTree->addToTree(trigger);
}

void GameSpaceTree::addMainEntity(const Entity& entity) { addMainEntity(std::make_shared<Entity>(entity)); }
void GameSpaceTree::addToTree(const Entity& entity) { addToTree(std::make_shared<Entity>(entity)); }
void GameSpaceTree::addToTree(const Object& object) { addToTree(std::make_shared<Object>(object)); }
void GameSpaceTree::addToTree(const Collider& collider) { addToTree(std::make_shared<Collider>(collider)); }
void GameSpaceTree::addToTree(const Trigger& trigger) { addToTree(std::make_shared<Trigger>(trigger)); }
