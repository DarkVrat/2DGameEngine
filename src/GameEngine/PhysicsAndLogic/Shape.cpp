#include "Shape.h"

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

Shape::Shape(const glm::vec2& position) :m_position(position) {
	m_points.push_back(glm::vec2(0, 0));
}

Shape::Shape(const glm::vec2& position, const std::vector<glm::vec2>& points) : m_position(position), m_points(points) {
	updateCircumradius();
}

Shape::Shape(const Shape& shape){
	m_points = shape.m_points;
	m_position = shape.m_position;
	m_circumradius = shape.m_circumradius;
}

Shape::Shape(Shape&& shape) noexcept{
	m_points = std::move(shape.m_points);
	m_position = std::move(shape.m_position);
	m_circumradius = std::move(shape.m_circumradius);
}

Shape Shape::operator=(const Shape& shape){
	m_points = shape.m_points;
	m_position = shape.m_position;
	m_circumradius = shape.m_circumradius;
	return *this;
}

Shape Shape::operator=(Shape&& shape) noexcept{
	m_points = std::move(shape.m_points);
	m_position = std::move(shape.m_position);
	m_circumradius = std::move(shape.m_circumradius);
	return *this;
}

Shape Shape::copyShape() const{
	return Shape(*this);
}

void Shape::ShapeIsPoint(){
	m_points.clear();
	m_points.push_back(glm::vec2(0, 0));
	m_circumradius = 0.f;
}

void Shape::ShapeIsPolygon(const std::vector<glm::vec2>& points){
	SetPoints(points);
}

void Shape::ShapeIsCircle(const float& radius, const uint8_t& numSegments){
	if (numSegments < 2) return ShapeIsPoint();
	m_points.clear();
	const float angleIncrement = 2.0f * 3.1415926f / numSegments;
	m_circumradius = radius;

	for (int i = 0; i < numSegments; ++i) {
		float angle = i * angleIncrement;
		float x = radius * cos(angle);
		float y = radius * sin(angle);
		m_points.push_back(glm::vec2(x, y));
	}
}

void Shape::Rotate(const float& gradus){
	float radians = glm::radians(gradus);
	for (auto& point : m_points) {
		point = glm::rotate(point, radians);
	}
}

void Shape::Scale(const float& factor){
	for (auto& point : m_points) {
		point *= factor;
	}
	updateCircumradius();
}

void Shape::Scale(const float& factor, const float& direction){
	Rotate(direction);
	for (auto& point : m_points) {
		point.x *= factor;
	}
	Rotate(-direction);
	updateCircumradius();
}

void Shape::Move(const glm::vec2& vec){
	m_position += vec;
}

void Shape::updateCircumradius(){
	for (auto vec : m_points) {
		float len = glm::length(vec);
		if (len > m_circumradius) {
			m_circumradius = len;
		}
	}
}

glm::vec2 Shape::GetPosition() const { return m_position; }
void Shape::SetPosition(const glm::vec2& position) { m_position = position; }

std::vector<glm::vec2> Shape::GetPoints() const { return m_points; }
void Shape::SetPoints(const std::vector<glm::vec2>& points) { 
	m_points = points; 
	updateCircumradius();
}

glm::vec2 Shape::Support(const glm::vec2& direction) const{
	float maxDotProduct = glm::dot(m_points[0], direction);
	glm::vec2 supportPoint = m_points[0];

	for (const auto& point : m_points) {
		float dotProduct = glm::dot(point, direction);
		if (dotProduct > maxDotProduct) {
			maxDotProduct = dotProduct;
			supportPoint = point;
		}
	}

	return supportPoint + m_position;
}

std::vector<glm::vec2> Shape::Render() const{
	std::vector<glm::vec2> points;
	for (auto& point : m_points) {
		points.push_back(point + m_position);
	}
	return points;
}
