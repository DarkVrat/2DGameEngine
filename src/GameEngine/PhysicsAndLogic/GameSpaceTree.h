#pragma once

#include <map>
#include "../Objects/Camera.h"
#include "../Objects/Collider.h"
#include "../Objects/Entity.h"
#include "../Objects/Object.h"
#include "../Objects/Trigger.h"
#include "../PhysicsAndLogic/CollisionController.h"

#define SIDES_LEFT 1
#define SIDES_RIGHT 2
#define SIDES_TOP 4
#define SIDES_BOTTOM 8

class GameSpaceTree {
public:
	GameSpaceTree(const glm::vec2& size=glm::vec2(0,0));
	GameSpaceTree(const GameSpaceTree& GST);
	GameSpaceTree(GameSpaceTree&& GST)noexcept;

	void operator=(const GameSpaceTree& GST);
	void operator=(GameSpaceTree&& GST)noexcept;

	void Render();
	void DebugRender();
	void Update(const double& duration);

	void addToTree(const Entity& entity);
	void addToTree(const Object& object);
	void addToTree(const Collider& collider);
	void addToTree(const Trigger& trigger);

	void addToTree(const std::shared_ptr<Entity>& entity);
	void addToTree(const std::shared_ptr<Object>& object);
	void addToTree(const std::shared_ptr<Collider>& collider);
	void addToTree(const std::shared_ptr<Trigger>& trigger);

	static void updatePositionCamera();
	static void updateSizeCamera();
private:
	GameSpaceTree(const std::vector<glm::vec2>& points, const glm::vec2 leftBottomPoint, GameSpaceTree* parrent);

	void Update(std::shared_ptr<Entity> entity, const double& duration);
	void setNeighbours(GameSpaceTree* LeftNeighbour, GameSpaceTree* RightNeighbour, GameSpaceTree* TopNeighbour, GameSpaceTree* BottomNeighbour);

	template<class T>
	void distributionObject(const std::shared_ptr<T>& obj);
	template<class T, class T2>
	void linkedMap(std::map<std::shared_ptr<T>, T2>& setTo, std::map<std::shared_ptr<T>, T2>& set0, std::map<std::shared_ptr<T>, T2>& set1, std::map<std::shared_ptr<T>, T2>& set2, std::map<std::shared_ptr<T>, T2>& set3);
	template<class T>
	void setSides(uint8_t& sides, const std::shared_ptr<T>& obj);
	template<class T>
	void setSidesByVec2(uint8_t& sides, const std::shared_ptr<T>& obj, const glm::vec2& direction);

	bool split();
	bool link();

	Collision m_collision;

	std::map<std::shared_ptr<Entity>, std::pair<glm::vec2, uint8_t>> m_Entitys;
	std::map<std::shared_ptr<Object>, uint8_t> m_Objects;
	std::map<std::shared_ptr<Collider>, uint8_t> m_Colliders;
	std::map<std::shared_ptr<Trigger>, uint8_t> m_Triggers;

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
inline void GameSpaceTree::distributionObject(const std::shared_ptr<T>& obj){
	static_assert(std::is_base_of<Collision, T>::value, "T must be derived from Collision");
	if (m_LeftBottomTree->m_collision.CheckCollision(*obj, ONLY_COLLISION).hasCollision) 
		m_LeftBottomTree->addToTree(obj);
	if (m_LeftTopTree->m_collision.CheckCollision(*obj, ONLY_COLLISION).hasCollision) 
		m_LeftTopTree->addToTree(obj);
	if (m_RightTopTree->m_collision.CheckCollision(*obj, ONLY_COLLISION).hasCollision) 
		m_RightTopTree->addToTree(obj);
	if (m_RightBottomTree->m_collision.CheckCollision(*obj, ONLY_COLLISION).hasCollision) 
		m_RightBottomTree->addToTree(obj);
}


template<class T, class T2>
inline void GameSpaceTree::linkedMap(std::map<std::shared_ptr<T>, T2>& setTo, std::map<std::shared_ptr<T>, T2>& set0, std::map<std::shared_ptr<T>, T2>& set1, std::map<std::shared_ptr<T>, T2>& set2, std::map<std::shared_ptr<T>, T2>& set3){
	static_assert(std::is_base_of<Collision, T>::value, "T must be derived from Collision");
	setTo.insert(set0.begin(), set0.end());
	setTo.insert(set1.begin(), set1.end());
	setTo.insert(set2.begin(), set2.end());
	setTo.insert(set3.begin(), set3.end());
}

template<class T>
inline void GameSpaceTree::setSides(uint8_t& sides, const std::shared_ptr<T>& obj){
	static_assert(std::is_base_of<Collision, T>::value, "T must be derived from Collision");
	sides = 0;
	if (m_LeftNeighbour != nullptr && m_LeftNeighbour->m_collision.CheckCollision(*obj, ONLY_COLLISION).hasCollision) 
		sides |= SIDES_LEFT;
	if (m_RightNeighbour != nullptr && m_RightNeighbour->m_collision.CheckCollision(*obj, ONLY_COLLISION).hasCollision) 
		sides |= SIDES_RIGHT;
	if (m_TopNeighbour != nullptr && m_TopNeighbour->m_collision.CheckCollision(*obj, ONLY_COLLISION).hasCollision) 
		sides |= SIDES_TOP;
	if (m_BottomNeighbour != nullptr && m_BottomNeighbour->m_collision.CheckCollision(*obj, ONLY_COLLISION).hasCollision) 
		sides |= SIDES_BOTTOM;
}

template<class T>
inline void GameSpaceTree::setSidesByVec2(uint8_t& sides, const std::shared_ptr<T>& obj, const glm::vec2& direction){
	static_assert(std::is_base_of<Collision, T>::value, "T must be derived from Collision");

	if (direction.x > 0.0000001f) {
		if (!(sides & SIDES_RIGHT) && m_RightNeighbour != nullptr && m_RightNeighbour->m_collision.CheckCollision(*obj, ONLY_COLLISION).hasCollision) {
			m_RightNeighbour->addToTree(obj);
			sides |= SIDES_RIGHT;
		}
		if ((sides & SIDES_LEFT) && m_LeftNeighbour != nullptr && !m_LeftNeighbour->m_collision.CheckCollision(*obj, ONLY_COLLISION).hasCollision)
			sides &= ~SIDES_LEFT;
	}
	else if (direction.x < -0.0000001f) {
		if (!(sides & SIDES_LEFT) && m_LeftNeighbour != nullptr && m_LeftNeighbour->m_collision.CheckCollision(*obj, ONLY_COLLISION).hasCollision) {
			m_LeftNeighbour->addToTree(obj);
			sides |= SIDES_LEFT;
		}
		if ((sides & SIDES_RIGHT) && m_RightNeighbour != nullptr && !m_RightNeighbour->m_collision.CheckCollision(*obj, ONLY_COLLISION).hasCollision)
			sides &= ~SIDES_RIGHT;
	}

	if (direction.y > 0.0000001f) {
		if (!(sides & SIDES_TOP) && m_TopNeighbour != nullptr && m_TopNeighbour->m_collision.CheckCollision(*obj, ONLY_COLLISION).hasCollision) {
			m_TopNeighbour->addToTree(obj);
			sides |= SIDES_TOP;
		}
		if ((sides & SIDES_BOTTOM) && m_BottomNeighbour != nullptr && !m_BottomNeighbour->m_collision.CheckCollision(*obj, ONLY_COLLISION).hasCollision)
			sides &= ~SIDES_BOTTOM;
	}
	else if (direction.y < -0.0000001f) {
		if (!(sides & SIDES_BOTTOM) && m_BottomNeighbour != nullptr && m_BottomNeighbour->m_collision.CheckCollision(*obj, ONLY_COLLISION).hasCollision) {
			m_BottomNeighbour->addToTree(obj);
			sides |= SIDES_BOTTOM;
		}
		if ((sides & SIDES_TOP) && m_TopNeighbour != nullptr && !m_TopNeighbour->m_collision.CheckCollision(*obj, ONLY_COLLISION).hasCollision)
			sides &= ~SIDES_TOP;
	}
}
