#pragma once

#include "Shape.h"
#include <memory>
#include <vector>

#define ONLY_COLLISION false
#define WITH_VECTOR true

struct EPAResult {
	EPAResult(const bool& has = false, const glm::vec2& pen = glm::vec2(0, 0)) :hasCollision(has), penetrationVector(pen) {};

	bool hasCollision;
	glm::vec2 penetrationVector;
};

class Collision :public Shape {
public:
	Collision(const glm::vec2& position = glm::vec2(0, 0)) :Shape(position) {};
	Collision(const glm::vec2& position, const std::vector<glm::vec2>& points) :Shape(position, points) {};
	Collision(const Shape& shape) :Shape(shape) {};
	Collision(const Collision& collision);
	Collision(Collision&& collision) noexcept;

	Collision operator=(const Collision& collision);
	Collision operator=(Collision&& collision)noexcept;

	std::shared_ptr<Collision> copyCollision();

	EPAResult CheckCollision(const Collision& other,const bool& flag = WITH_VECTOR);
protected:
	static bool IsSimplexColinear(std::vector<glm::vec2>& simplex, glm::vec2& direction);
	EPAResult ProcessEPA(const Collision& other, std::vector<glm::vec2>& simplex);
};