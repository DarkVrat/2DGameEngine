#pragma once

#include "Shape.h"
#include <memory>
#include <vector>

struct EPAResult {
	EPAResult(const bool& has = false, const glm::vec2& pen = glm::vec2(0, 0)) :hasCollision(has), penetrationVector(pen) {};

	bool hasCollision;
	glm::vec2 penetrationVector;
};

class Collision :public Shape {
public:
	Collision() : Shape() {};
	Collision(const Shape shape) :Shape(shape) {};

	EPAResult CheckCollision(const Collision other);
private:
	static bool IsSimplexColinear(std::vector<glm::vec2>& simplex, glm::vec2& direction);
	EPAResult ProcessEPA(const Collision other, std::vector<glm::vec2>& simplex);
};