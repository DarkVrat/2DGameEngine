#pragma once

#include <map>
#include "../Objects/Camera.h"
#include "../Objects/Collider.h"
#include "../Objects/Entity.h"
#include "../Objects/Object.h"
#include "../Objects/Trigger.h"

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
	template<class T>
	void linkedMap(std::map<std::shared_ptr<T>, uint8_t>& setTo, std::map<std::shared_ptr<T>, uint8_t>& set0, std::map<std::shared_ptr<T>, uint8_t>& set1, std::map<std::shared_ptr<T>, uint8_t>& set2, std::map<std::shared_ptr<T>, uint8_t>& set3);

	void split();
	bool link();

	Collision m_collision;

	std::map<std::shared_ptr<Entity>, uint8_t> m_Entitys;
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
	if (m_LeftBottomTree->m_collision.CheckCollision(*obj).hasCollision) m_LeftBottomTree->addToTree(obj);
	if (m_LeftTopTree->m_collision.CheckCollision(*obj).hasCollision) m_LeftTopTree->addToTree(obj);
	if (m_RightTopTree->m_collision.CheckCollision(*obj).hasCollision) m_RightTopTree->addToTree(obj);
	if (m_RightBottomTree->m_collision.CheckCollision(*obj).hasCollision) m_RightBottomTree->addToTree(obj);
}


template<class T>
inline void GameSpaceTree::linkedMap(std::map<std::shared_ptr<T>, uint8_t>& setTo, std::map<std::shared_ptr<T>, uint8_t>& set0, std::map<std::shared_ptr<T>, uint8_t>& set1, std::map<std::shared_ptr<T>, uint8_t>& set2, std::map<std::shared_ptr<T>, uint8_t>& set3){
	static_assert(std::is_base_of<Collision, T>::value, "T must be derived from Collision");
	setTo.insert(set0.begin(), set0.end());
	setTo.insert(set1.begin(), set1.end());
	setTo.insert(set2.begin(), set2.end());
	setTo.insert(set3.begin(), set3.end());
}
