#include "Collision.h"

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

Collision::Collision(Collision&& collision) noexcept{
	m_position = std::move(collision.m_position);
	m_points = std::move(collision.m_points);
}

std::shared_ptr<Collision> Collision::copyCollision(){
	return std::make_shared<Collision>(copyShape());
}

EPAResult Collision::CheckCollision(const Collision& other){
	glm::vec2 direction(1.0f, 0.0f);

	glm::vec2 supportA = this->Support(direction);
	glm::vec2 supportB = other.Support(-direction);

	std::vector<glm::vec2> simplex;
	simplex.push_back(supportA - supportB);

	direction = -simplex[0];

	while (true) {
		direction = glm::normalize(direction);
		supportA = this->Support(direction);
		supportB = other.Support(-direction);

		glm::vec2 newPoint = supportA - supportB;

		if (glm::dot(newPoint, direction) <= 0.0f) {
			return EPAResult();
		}

		simplex.push_back(newPoint);

		if (IsSimplexColinear(simplex, direction)) {
			return ProcessEPA(other, simplex);
		}
	}

	return EPAResult();
}

bool Collision::IsSimplexColinear(std::vector<glm::vec2>& simplex, glm::vec2& direction){
	int n = simplex.size();

	if (n == 2) {
		const glm::vec2& A = simplex[1];
		const glm::vec2& B = simplex[0];

		glm::vec3 AB = glm::vec3(B - A, 0.f);
		glm::vec3 AO = glm::vec3(-A, 0.f);
		glm::vec3 crossVec = glm::cross(glm::cross(AB, AO), AB);

		direction = glm::vec2(crossVec.x, crossVec.y);
		return false;
	}

	if (n == 3) {
		const glm::vec2& A = simplex[2];
		const glm::vec2& B = simplex[1];
		const glm::vec2& C = simplex[0];

		glm::vec3 AB = glm::vec3(B - A, 0.f);
		glm::vec3 AC = glm::vec3(C - A, 0.f);
		glm::vec3 AO = glm::vec3(-A, 0.f);
		glm::vec3 ABC = glm::cross(AB, AC);

		if (glm::dot(glm::cross(ABC, AC), AO) > 0.f) {
			glm::vec3 crossVec = glm::cross(glm::cross(AC, AO), AC);
			direction = glm::vec2(crossVec.x, crossVec.y);
			simplex.erase(simplex.begin()+1);
			return false;
		}

		if (glm::dot(glm::cross(AB, ABC), AO) > 0.f) {
			glm::vec3 crossVec = glm::cross(glm::cross(AB, AO), AB);
			direction = glm::vec2(crossVec.x, crossVec.y);
			simplex.erase(simplex.begin());
			return false;
		}

		return true;
	}

	return false;
}

EPAResult Collision::ProcessEPA(const Collision& other, std::vector<glm::vec2>& simplex) {
	int maxSimplex = this->m_points.size() * other.m_points.size();
	while (true) {
		size_t closestEdgeIndex{};
		float minDistance = std::numeric_limits<float>::max();

		for (size_t i = 0; i < simplex.size(); ++i) {
			const glm::vec2& A = simplex[i];
			const glm::vec2& B = simplex[(i + 1) % simplex.size()];

			glm::vec2 edge = B - A;
			glm::vec2 normal = glm::normalize(glm::vec2(edge.y, -edge.x));

			float distance = glm::dot(normal, A);

			if (distance < minDistance) {
				minDistance = distance;
				closestEdgeIndex = i;
			}
		}

		const glm::vec2& A = simplex[closestEdgeIndex];
		const glm::vec2& B = simplex[(closestEdgeIndex + 1) % simplex.size()];

		glm::vec2 edge = B - A;
		glm::vec2 normal = glm::normalize(glm::vec2(edge.y, -edge.x));
		glm::vec2 support = this->Support(normal) - other.Support(-normal);

		float distance = glm::dot(normal, support);

		if (distance - minDistance < 1e-4f || simplex.size()>=maxSimplex) {
			glm::vec2 penetrationVector = distance * normal; 
			return EPAResult(true, penetrationVector);
		}

		simplex.insert(simplex.begin() + closestEdgeIndex + 1, support);
	}
}
