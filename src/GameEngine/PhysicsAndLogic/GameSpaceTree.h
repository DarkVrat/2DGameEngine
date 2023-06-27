#pragma once

#include "../Objects/Camera.h"
#include "../Objects/Collider.h"
#include "../Objects/Entity.h"
#include "../Objects/Object.h"
#include "../Objects/Trigger.h"

class GameSpaceTree {
public:
	GameSpaceTree(const glm::vec2& size);
	 
	void Render();
	void DebugRender();
	void Update(const double& duration);

	void addMainEntity(const Entity& entity);
	void addToTree(const Entity& entity);
	void addToTree(const Object& object);
	void addToTree(const Collider& collider);
	void addToTree(const Trigger& trigger);

	static void updatePositionCamera();
	static void updateSizeCamera();
private:
	GameSpaceTree(const std::vector<glm::vec2>& points, const glm::vec2 leftBottomPoint, GameSpaceTree* parrent);

	void addMainEntity(const std::shared_ptr<Entity>& entity);
	void addToTree(const std::shared_ptr<Entity>& entity);
	void addToTree(const std::shared_ptr<Object>& object);
	void addToTree(const std::shared_ptr<Collider>& collider);
	void addToTree(const std::shared_ptr<Trigger>& trigger);

	template<class T>
	void distributionVector(std::vector<std::shared_ptr<T>>& vector);
	void setNeighbours(GameSpaceTree* LeftNeighbour, GameSpaceTree* RightNeighbour, GameSpaceTree* TopNeighbour, GameSpaceTree* BottomNeighbour);
	
	template<class T>
	void linkedVectot(std::vector<std::shared_ptr<T>>& vectorTo, std::vector<std::shared_ptr<T>>& vector0, std::vector<std::shared_ptr<T>>& vector1, std::vector<std::shared_ptr<T>>& vector2, std::vector<std::shared_ptr<T>>& vector3);

	void split();
	void link();

	Collision& getCollision() { return m_collision; }

	Collision m_collision;

	std::shared_ptr<Entity> m_MainEntity;
	std::vector<std::shared_ptr<Entity>> m_Entitys;
	std::vector<std::shared_ptr<Object>> m_Objects;
	std::vector<std::shared_ptr<Collider>> m_Colliders;
	std::vector<std::shared_ptr<Trigger>> m_Triggers;

	GameSpaceTree* m_LeftBottomTree = nullptr;
	GameSpaceTree* m_LeftTopTree = nullptr;
	GameSpaceTree* m_RightBottomTree = nullptr;
	GameSpaceTree* m_RightTopTree = nullptr;

	GameSpaceTree* m_LeftNeighbour = nullptr;
	GameSpaceTree* m_RightNeighbour = nullptr;
	GameSpaceTree* m_TopNeighbour = nullptr;
	GameSpaceTree* m_BottomNeighbour = nullptr;

	GameSpaceTree* m_parrent = nullptr;

	static Collision m_collisionForCamera;
};

template<class T>
inline void GameSpaceTree::distributionVector(std::vector<std::shared_ptr<T>>& vector){
	static_assert(std::is_base_of<Collision, T>::value, "T must be derived from Collision");
	for (auto& obj : vector) {
		if (m_LeftBottomTree->getCollision().CheckCollision(*obj).hasCollision) m_LeftBottomTree->addToTree(obj);
		if (m_LeftTopTree->getCollision().CheckCollision(*obj).hasCollision)	m_LeftTopTree->addToTree(obj);
		if (m_RightTopTree->getCollision().CheckCollision(*obj).hasCollision) m_RightTopTree->addToTree(obj);
		if (m_RightBottomTree->getCollision().CheckCollision(*obj).hasCollision) m_RightBottomTree->addToTree(obj);
	}
}

template<class T>
inline void GameSpaceTree::linkedVectot(std::vector<std::shared_ptr<T>>& vectorTo, std::vector<std::shared_ptr<T>>& vector0, std::vector<std::shared_ptr<T>>& vector1, std::vector<std::shared_ptr<T>>& vector2, std::vector<std::shared_ptr<T>>& vector3){
	static_assert(std::is_base_of<Collision, T>::value, "T must be derived from Collision");
	vectorTo.insert(vectorTo.end(), vector0.begin(), vector0.end());
	vectorTo.insert(vectorTo.end(), vector1.begin(), vector1.end());
	vectorTo.insert(vectorTo.end(), vector2.begin(), vector2.end());
	vectorTo.insert(vectorTo.end(), vector3.begin(), vector3.end());
}
